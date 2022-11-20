#include <link.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <string.h>
#include "ElfHandler.h"

extern "C" {
    typedef void (*StubFunctiong)();
    void method01();
    int method02();
}


char __PROC_REFER_BASE_ADDRESS_SYMBLE__ = 0xFE;


static size_t CalculateProccBaseAddress(const char* referSymName, size_t referSymAddr, const ElfHandler& handler) {
    const ElfW(Sym)* referSym = handler.findSym(referSymName);
    if (referSym == nullptr) {
        printf("not found symble named '%s'!\n", referSymName);
        return (size_t)NULL;
    }

    size_t procBaseAddr = referSymAddr - referSym->st_value;
    return procBaseAddr;
}


static void ProcessSymble(const char* name, const ElfHandler& handler, size_t procBaseAddr) {
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

void ManualProcess(const ElfHandler& handler) {
    const char* referSymName = "__PROC_REFER_BASE_ADDRESS_SYMBLE__";
    const size_t referSymAddr = (size_t)&__PROC_REFER_BASE_ADDRESS_SYMBLE__;
    size_t procBaseAddr = CalculateProccBaseAddress(referSymName, referSymAddr, handler);

    ProcessSymble("method03", handler, procBaseAddr);
    ProcessSymble("variable01", handler, procBaseAddr);
    ProcessSymble("globalInteger", handler, procBaseAddr);
    ProcessSymble("method01", handler, procBaseAddr);
}

extern "C" void method03();
extern int globalInteger;

int variable01 = 0;

extern "C" void method01() {
    printf("======> call method '%s'\n", __FUNCTION__);
}

extern "C" int method02() {
    printf("======> call method '%s'\n", __FUNCTION__);
    return 0;
}

int globalCount = 0;

int main() {
    variable01 = 1;
    globalInteger = 5;

    char elfPath[1024] = {0};
    int n = readlink("/proc/self/exe", elfPath, sizeof(elfPath));
    if( n > 0 && n < sizeof(elfPath)) {
        printf("process elf file: %s\n" , elfPath);
    }

    ElfHandler handler(elfPath);
    ManualProcess(handler);
    return 0;
}