#include <link.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <string.h>
#include "ElfHandler.h"


void printPhdrHeader(const Elf64_Phdr& header) {
    printf("======== Elf64_Phdr start ========\n");
    printf("    |- p_type   = %d\n", header.p_type);
    printf("    |- p_flags  = %d\n", header.p_flags);
    printf("    |- p_offset = %lu\n", header.p_offset);
    printf("    |- p_vaddr  = %p\n", (void*)header.p_vaddr);
    printf("    |- p_paddr  = %p\n", (void*)header.p_paddr);
    printf("    |- p_filesz = %lu\n", header.p_filesz);
    printf("    |- p_memsz  = %lu\n", header.p_memsz);
    printf("    |- p_align  = %lu\n", header.p_align);
    printf("======== Elf64_Phdr end ========\n");
}

using namespace std;

size_t SectionNameStrSize = 0;
const char* SectionNameStrTable = nullptr;

void printStrTable(size_t strSize, char* strTable) {
    printf("======== print string table begin ========\n");
    printf(" string size: %lu\n", strSize);
    printf(" string  addr: %p\n", strTable);
    printf(" string items: ");
    for (size_t strIdx = 0; strIdx < strSize; ++strIdx) {
        if (*(strTable + strIdx) == 0) {
            printf("\n  --> ");
        } else {
            printf("%c", *(strTable + strIdx));
        }
    }
    printf("\n");
    printf("======== print string table end ========\n");
}

void retrievePhdr(size_t phdrNum, ElfW(Phdr)* phdrs, FILE* fp) {
    for (size_t phdrIdx = 0; phdrIdx < phdrNum; ++phdrIdx) {
        ElfW(Phdr)& phdr = phdrs[phdrIdx];
        printf("==> header phdrIdx: %lu, type: 0x%X\n", phdrIdx, phdr.p_type);

        ElfW(Dyn*) dyns = new ElfW(Dyn)[phdr.p_filesz];
        fseek(fp, phdr.p_offset, SEEK_SET);
        fread(dyns, phdr.p_filesz, sizeof(char), fp);

        size_t strSize = 285;
        char* strTab = nullptr;
        for (ElfW(Dyn*) dyn = dyns; dyn->d_tag != DT_NULL; ++dyn) {
            if (dyn->d_tag == DT_STRSZ) {
                strSize = dyn->d_un.d_val;
                printf("      |--> 1 string table size: %lu\n", strSize);
            } else if (dyn->d_tag == DT_STRTAB) {
                printf("      |--> 2 string table size: %lu\n", strSize);
                // strTab = (char*)dyn->d_un.d_ptr;
                strTab = new char[strSize];
                fseek(fp, dyn->d_un.d_ptr, SEEK_SET);
                fread(strTab, strSize, sizeof(char), fp);
                printStrTable(strSize, strTab);
                delete[] strTab;
                strTab = nullptr;
            }
        }
    }
}

extern "C" {
    typedef void (*StubFunctiong)();
    void method01();
    int method02();
}

void retrieveShdr(size_t shdrNum, ElfW(Shdr)* shdrs, ElfHandler& handler) {
    const char* strTable = handler.getSymStrTable();
    for (size_t shdrIdx = 0; shdrIdx < shdrNum; ++shdrIdx) {
        ElfW(Shdr)& shdr = shdrs[shdrIdx];

        if (shdr.sh_type == SHT_SYMTAB) {
            printf("################################ 002\n");
            size_t symNum = shdr.sh_size / sizeof(ElfW(Sym));
            const ElfW(Sym)* syms = handler.getSymbles();

            size_t procBaseAddr = 0;
            for (size_t symIdx = 0; symIdx < symNum; ++symIdx) {
                const ElfW(Sym)& sym = syms[symIdx];
                const char type = ELF32_ST_TYPE(sym.st_info);
                const char binding = ELF32_ST_BIND(sym.st_info);
                
                if (binding == STB_GLOBAL && (type == STT_FUNC || type == STT_OBJECT)) {
                    const char* name = &strTable[sym.st_name];
                    printf(" type: %-4d, binding: %-4d, name: %s\n", type, binding, name);
                    if (strcmp(name, "method02") == 0) {
                        procBaseAddr = (size_t)&method02 - sym.st_value;
                    }
                }
            }

            printf("======== print symbol begin ========\n");
            for (size_t symIdx = 0; symIdx < symNum; ++symIdx) {
                const ElfW(Sym)& sym = syms[symIdx];
                const char type = ELF32_ST_TYPE(sym.st_info);
                const char binding = ELF32_ST_BIND(sym.st_info);
                
                if (binding == STB_GLOBAL && (type == STT_FUNC || type == STT_OBJECT)) {
                    const char* name = &strTable[sym.st_name];
                    printf(" type: %-4d, binding: %-4d, name: %s\n", type, binding, name);
                    if (type == STT_FUNC) {
                        if (strcmp(name, "method02") == 0) {
                            StubFunctiong func = (StubFunctiong)(procBaseAddr + sym.st_value);
                            func();
                        }
                    }
                }
            }
            printf("======== print symbol end ========\n");
        }
    }
}

extern void method03();
extern int globalInteger;

int variable01 = 0;

extern "C" void method01() {
    printf("=================== %s ===========\n", __FUNCTION__);
    // return 0;
}

extern "C" int method02() {
    printf("=================== %s ===========\n", __FUNCTION__);
    return 0;
}

int globalCount = 0;

int main() {
    method01();
    method02();
    method03();
    variable01 = 1;
    globalInteger = 5;

    char buf[1024] = {0};
    int n = readlink("/proc/self/exe", buf, sizeof(buf));
    if( n > 0 && n < sizeof(buf)) {
        printf("process elf file: %s\n" , buf);
    }
    ElfHandler handler(buf);

    const ElfW(Ehdr)& ehdr = handler.getEhdr(); 

    const ElfW(Phdr)* phdrs = handler.getPhdrs();
    const ElfW(Shdr)* shdrs = handler.getShdrs();

    // read section names string table
    
    SectionNameStrSize = handler.getStrTableSize();;
    SectionNameStrTable = handler.getStrTable();

    const ElfW(Ehdr)* pehdr = &ehdr;

    // retrievePhdr(ehdr.e_phnum, phdrs, fp);
    retrieveShdr(ehdr.e_shnum, const_cast<ElfW(Shdr)*>(shdrs), handler);
    // printStrTable(strSize, strTable);
    return 0;
}