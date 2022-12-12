#include "IOHandler.h"
#include <iostream>

std::string ReadLine() {
    const static size_t INPUT_BUFF_SIZE = 64;
    char static inputBuff[INPUT_BUFF_SIZE] = { 0 };
    printf("ms:> ");
    std::cin.getline(inputBuff, INPUT_BUFF_SIZE);    // scanf("%s", s);
    return std::string(inputBuff);
}