#include <link.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <string.h>
#include "ElfHandler.h"
#include "ProcManual.h"

char __PROC_REFER_BASE_ADDRESS_SYMBLE__ = 0xFE;

extern "C" void method03();
extern int globalInteger;

int variable01 = 0;
int globalCount = 0;

extern "C" void method01() {
    printf("======> call method '%s'\n", __FUNCTION__);
}

extern "C" int method02() {
    printf("======> call method '%s'\n", __FUNCTION__);
    return 0;
}

int main() {
    variable01 = 1;
    globalInteger = 5;

    char elfPath[1024] = {0};
    int n = readlink("/proc/self/exe", elfPath, sizeof(elfPath));
    if( n > 0 && n < sizeof(elfPath)) {
        printf("process elf file: %s\n" , elfPath);
    }

    const char* referSymName = "__PROC_REFER_BASE_ADDRESS_SYMBLE__";
    const size_t referSymAddr = (size_t)&__PROC_REFER_BASE_ADDRESS_SYMBLE__;

    ElfHandler handler(elfPath);
    ProcManual manual(handler, referSymName, referSymAddr);
    manual.execSymble("method03");
    manual.execSymble("variable01");
    manual.execSymble("globalInteger");
    manual.execSymble("method01");
    return 0;
}