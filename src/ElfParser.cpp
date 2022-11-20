#include "ElfParser.h"
#include <string.h>

static ElfW(Ehdr) ParseEhdr(FILE* fp) {
    ElfW(Ehdr) ehdr;
    fread(&ehdr, 1, sizeof(ehdr), fp);
    return ehdr;
}

template<typename Type>
static std::vector<Type> Parse(FILE* fp, size_t offset, size_t num) {
    Type* buff = new Type[num];
    fseek(fp, offset, SEEK_SET);
    fread(buff, num, sizeof(Type), fp);

    std::vector<Type> entries;
    entries.clear();
    entries.reserve(num);
    for (size_t idx = 0; idx < num; ++idx) {
        entries.emplace_back(buff[idx]);
    }

    delete[] buff;
    return entries;
}

typedef size_t ShOffset;
static std::map<ShOffset, std::string> parseStrTable(FILE* fp, size_t offset, size_t size) {
    char* buff = new char[size];
    fseek(fp, offset, SEEK_SET);
    fread(buff, size, sizeof(char), fp);

    std::map<ShOffset, std::string> strTable;
    strTable.clear();
    size_t cursor = 0;
    for (size_t strIdx = 0; strIdx < size; ++strIdx) {
        if (buff[strIdx] == 0) {
            strTable.emplace(cursor, buff + cursor);
            cursor = strIdx + 1;
        }
    }
    return strTable;
}

ElfParser::ElfParser(const char* fileName) {
    /**
     * TODO 在函数外面访问fp为什么会跑挂？和fopen的机制有关？
     * fopen 的内存是在堆上还是栈上？或者不同的实现中内存分配不一样？
    */
    FILE *fp = fopen(fileName, "r");
    if (!fp) {
        printf("Failed to open file: %s\n", fileName);
        return;
    }
    ehdr = ParseEhdr(fp);
    phdrs = Parse<ElfW(Phdr)>(fp, ehdr.e_phoff, ehdr.e_phnum);
    shdrs = Parse<ElfW(Shdr)>(fp, ehdr.e_shoff, ehdr.e_shnum);

    const ElfW(Shdr)& secNameTableShdr = findSectionNameStrTableShdr();
    strTable = parseStrTable(fp, secNameTableShdr.sh_offset, secNameTableShdr.sh_size);

    const ElfW(Shdr)& symNameTableShdr = findSymbleNameStrTableShdr();
    symStrTable = parseStrTable(fp, symNameTableShdr.sh_offset, symNameTableShdr.sh_size);

    const ElfW(Shdr)& symTableShdr = findSymbleTableShdr();
    syms = Parse<ElfW(Sym)>(fp, symTableShdr.sh_offset, symTableShdr.sh_size / sizeof(ElfW(Sym)));
}

const ElfW(Sym)* ElfParser::findSym(const string& name) const {
    for (auto& sym : syms) {
        const char type = ELF32_ST_TYPE(sym.st_info);
        const char binding = ELF32_ST_BIND(sym.st_info);
        
        if (binding == STB_GLOBAL && (type == STT_FUNC || type == STT_OBJECT)) {
            // 一部分符号的name是字符串表中字符串的截断形式，map无法查找，暂时不关注这些符号
            if (symStrTable.find(sym.st_name) == symStrTable.end()) {
                continue;
            }
            if (symStrTable.at(sym.st_name) == name) {
                return &sym;
            }
        }
    }
    return nullptr;
}

const ElfW(Shdr)& ElfParser::findSymbleNameStrTableShdr() const {
    for (size_t shdrIdx = 0; shdrIdx < ehdr.e_shnum; ++shdrIdx) {
        const ElfW(Shdr)& shdr = shdrs[shdrIdx];
        // TODO optimize the check condition
        const std::map<ShOffset, std::string>& strTab = getStrTable1();
        if (shdr.sh_type == SHT_STRTAB && strcmp(strTab.at(shdr.sh_name).c_str(), ".strtab") == 0) {
            return shdrs[shdrIdx];
        }
    }
}

const ElfW(Shdr)& ElfParser::findSymbleTableShdr() const {
    for (size_t shdrIdx = 0; shdrIdx < ehdr.e_shnum; ++shdrIdx) {
        const ElfW(Shdr)& shdr = shdrs[shdrIdx];
        if (shdr.sh_type == SHT_SYMTAB) {
            return shdrs[shdrIdx];
        }
    }
}

const ElfW(Shdr)& ElfParser::findSectionNameStrTableShdr() const {
    return shdrs[ehdr.e_shstrndx];
}

size_t ElfParser::getSymSize() const {
    for (size_t shdrIdx = 0; shdrIdx < ehdr.e_shnum; ++shdrIdx) {
        const ElfW(Shdr)& shdr = shdrs[shdrIdx];
        if (shdr.sh_type == SHT_SYMTAB) {
            return shdrs[shdrIdx].sh_size / sizeof(ElfW(Sym));
        }
    }
}
