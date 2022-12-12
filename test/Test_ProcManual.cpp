#include <link.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "ElfHandler.h"
#include "ProcManual.h"
#include "IOHandler.h"

extern "C" void method03(int a, int b);
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
    const static ProcManual manual = GetProcManualInstance();
    while (true)
    {
        std::string input = ReadLine();

        if (input == "exit") {
            printf("exited!!!\n");
            break;
        }

        if (input == "showSymbles") {
            manual.dumpFunctions();
            manual.dumpGlobalVariables();
        } else {
            manual.execSymble(input.c_str());
        }
    }
    return 0;
}