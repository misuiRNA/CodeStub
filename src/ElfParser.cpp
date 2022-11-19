#include "ElfParser.h"
#include <string.h>

static ElfW(Ehdr) ParseEhdr(FILE* fp) {
    ElfW(Ehdr) ehdr;
    fread(&ehdr, 1, sizeof(ehdr), fp);
    return ehdr;
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
    phdrsHandler = ElfBuffHandler<ElfW(Phdr)>(fp, ehdr.e_phoff, ehdr.e_phnum);
    shdrsHandler = ElfBuffHandler<ElfW(Shdr)>(fp, ehdr.e_shoff, ehdr.e_shnum);

    const ElfW(Shdr)& secNameTableShdr = findSectionNameStrTableShdr();
    strTableHandler = ElfBuffHandler<char>(fp, secNameTableShdr.sh_offset, secNameTableShdr.sh_size);

    const ElfW(Shdr)& symNameTableShdr = findSymbleNameStrTableShdr();
    symStrTableHandler = ElfBuffHandler<char>(fp, symNameTableShdr.sh_offset, symNameTableShdr.sh_size);

    const ElfW(Shdr)& symTableShdr = findSymbleTableShdr();
    symsHandler = ElfBuffHandler<ElfW(Sym)>(fp, symTableShdr.sh_offset, symTableShdr.sh_size / sizeof(ElfW(Sym)));
}

const ElfW(Shdr)& ElfParser::findSymbleNameStrTableShdr() const {
    for (size_t shdrIdx = 0; shdrIdx < ehdr.e_shnum; ++shdrIdx) {
        const ElfW(Shdr)& shdr = getShdrs()[shdrIdx];
        // TODO optimize the check condition
        if (shdr.sh_type == SHT_STRTAB && strcmp(&getStrTable()[shdr.sh_name], ".strtab") == 0) {
            return getShdrs()[shdrIdx];
        }
    }
}

const ElfW(Shdr)& ElfParser::findSymbleTableShdr() const {
    for (size_t shdrIdx = 0; shdrIdx < ehdr.e_shnum; ++shdrIdx) {
        const ElfW(Shdr)& shdr = getShdrs()[shdrIdx];
        if (shdr.sh_type == SHT_SYMTAB) {
            return getShdrs()[shdrIdx];
        }
    }
}

const ElfW(Shdr)& ElfParser::findSectionNameStrTableShdr() const {
    return getShdrs()[ehdr.e_shstrndx];
}

size_t ElfParser::getSymSize() const {
    for (size_t shdrIdx = 0; shdrIdx < ehdr.e_shnum; ++shdrIdx) {
        const ElfW(Shdr)& shdr = getShdrs()[shdrIdx];
        if (shdr.sh_type == SHT_SYMTAB) {
            return getShdrs()[shdrIdx].sh_size / sizeof(ElfW(Sym));
        }
    }
}
