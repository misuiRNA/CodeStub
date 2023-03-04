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

extern "C" int method04(int arg1, int arg2) {
    printf("======> call method '%s', args: %d, %d\n", __FUNCTION__, arg1, arg2);
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
            ManualCommand command = ManualCommand::ParseCommand(input.c_str());
            manual.execSymble(command.name, command.args);
        }
    }
    return 0;
}