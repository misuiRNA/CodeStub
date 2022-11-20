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

void ProcessSymble(const char* name, const ElfParser& handler, size_t procBaseAddr) {
    const ElfW(Sym)* sym = handler.findSym(name);
    if (sym == nullptr) {
        printf("not found symble named '%s'!\n", name);
        return;
    }
    const char symType = ELF32_ST_TYPE(sym->st_info);
    if (symType == STT_FUNC) {
        StubFunctiong func = (StubFunctiong)(procBaseAddr + sym->st_value);
        func();
    } else if (symType == STT_OBJECT) {
        printf("%s: %d\n", name, *(int*)(procBaseAddr + sym->st_value));
    }
}

void StubProc(ElfParser& handler) {
    const char* referSymName = "method02";
    const size_t referSymAddr = (size_t)&method02;
    const ElfW(Sym)* referSym = handler.findSym(referSymName);
    if (referSym == nullptr) {
        printf("not found symble named '%s'!\n", referSymName);
        return;
    }
    size_t procBaseAddr = referSymAddr - referSym->st_value;

    ProcessSymble("method03", handler, procBaseAddr);
    ProcessSymble("variable01", handler, procBaseAddr);
    ProcessSymble("globalInteger", handler, procBaseAddr);
    ProcessSymble("method01", handler, procBaseAddr);
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
    StubProc(handler);
    return 0;
}