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

const char* ReadLine() {
    const static size_t INPUT_BUFF_SIZE = 64;
    char static inputBuff[INPUT_BUFF_SIZE] = { 0 };
    printf("misui> ");
    std::cin.getline(inputBuff, INPUT_BUFF_SIZE);    // scanf("%s", s);
}

int main() {
    printf("please enter commands...\n");

    while (true)
    {
        const char* input = ReadLine();
        printf("%s", input);
        printf("\n");
    }
    


    variable01 = 1;
    globalInteger = 5;

    char elfPath[1024] = {0};
    int n = readlink("/proc/self/exe", elfPath, sizeof(elfPath));
    if( n > 0 && n < sizeof(elfPath)) {
        printf("process elf file: %s\n" , elfPath);
    }

    ElfHandler handler(elfPath);
    ProcManual manual(handler);
    manual.execSymble("method03");
    manual.execSymble("variable01");
    manual.execSymble("globalInteger");
    manual.execSymble("method01");
    manual.execSymble("method01");
    manual.execSymble("method01");

    return 0;
}