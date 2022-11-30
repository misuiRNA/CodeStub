#include <link.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "ElfHandler.h"
#include "ProcManual.h"

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

std::string ReadLine() {
    const static size_t INPUT_BUFF_SIZE = 64;
    char static inputBuff[INPUT_BUFF_SIZE] = { 0 };
    printf("ms:> ");
    std::cin.getline(inputBuff, INPUT_BUFF_SIZE);    // scanf("%s", s);
    return std::string(inputBuff);
}

void ExecCommand(const std::string& command) {
    const static ProcManual manual = CreateProcManual();
    printf("%s\n", command.c_str());
    manual.execSymble(command.c_str());
}

int main() {
    while (true)
    {
        std::string input = ReadLine();

        if (input == "exit") {
            printf("exited!!!\n");
            break;
        }

        ExecCommand(input);
    }
    return 0;
}