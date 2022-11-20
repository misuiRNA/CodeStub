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

void retrieveShdr(ElfParser& handler) {
    const char* referSymName = "method02";
    const size_t referSymAddr = (size_t)&method02;
    const ElfW(Sym)* referSym = handler.findSym(referSymName);
    if (referSym == nullptr) {
        printf("not found symble named '%s'!\n", referSymName);
        return;
    }
    size_t procBaseAddr = referSymAddr - referSym->st_value;


    const char* symName = "method03";
    // const char* symName = "variable01";
    const ElfW(Sym)* sym = handler.findSym(symName);
    if (sym == nullptr) {
        printf("not found symble named '%s'!\n", symName);
        return;
    }
    const char type = ELF32_ST_TYPE(sym->st_info);
    if (type == STT_FUNC) {
        StubFunctiong func = (StubFunctiong)(procBaseAddr + sym->st_value);
        func();
    } else if (type == STT_OBJECT) {
        printf("%s: %d\n", symName, *(int*)(procBaseAddr + sym->st_value));
    }
}

extern "C" void method03();
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
    variable01 = 1;
    globalInteger = 5;

    char buf[1024] = {0};
    int n = readlink("/proc/self/exe", buf, sizeof(buf));
    if( n > 0 && n < sizeof(buf)) {
        printf("process elf file: %s\n" , buf);
    }
    ElfParser handler(buf);
    retrieveShdr(handler);
    // printStrTable(strSize, strTable);
    return 0;
}