#include "IOHandler.h"
#include <iostream>
#include <algorithm>
#include <sstream>

std::string ReadLine() {
    const static size_t INPUT_BUFF_SIZE = 64;
    char static inputBuff[INPUT_BUFF_SIZE] = { 0 };
    printf("ms:> ");
    std::cin.getline(inputBuff, INPUT_BUFF_SIZE);    // scanf("%s", s);
    return std::string(inputBuff);
}

static std::vector<std::string> splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

ManualCommand ManualCommand::ParseCommand(const char* input) {
    std::string inputStr(input);
    inputStr.erase(std::remove_if(inputStr.begin(), inputStr.end(), [](unsigned char c) {
        return std::isspace(c);
    }), inputStr.end());

    std::vector<std::string> tokens = splitString(inputStr, ',');
    if (tokens.size() <= 0)
    {
        return {.name = "", .args = std::vector<int>()};
    }
    const char* name = tokens[0].c_str();

    std::vector<int> args(tokens.size() - 1);
    for (int index = 0; index < args.size(); ++index) {
        args[index] = std::stoi(tokens[index + 1]);
    }
    return {.name = name, .args = args};
}
