#include <link.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <string.h>
#include "ElfParser.h"

extern "C" {
    typedef void (*StubFunctiong)();
    void method01();
    int method02();
}

void retrieveShdr(size_t shdrNum, ElfW(Shdr)* shdrs, ElfParser& handler) {
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
    ElfParser handler(buf);

    const ElfW(Ehdr)& ehdr = handler.getEhdr(); 

    const ElfW(Phdr)* phdrs = handler.getPhdrs();
    const ElfW(Shdr)* shdrs = handler.getShdrs();

    const ElfW(Ehdr)* pehdr = &ehdr;

    // retrievePhdr(ehdr.e_phnum, phdrs, fp);
    retrieveShdr(ehdr.e_shnum, const_cast<ElfW(Shdr)*>(shdrs), handler);
    // printStrTable(strSize, strTable);
    return 0;
}