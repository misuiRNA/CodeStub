#include <stdio.h>
#include <string>
#include <string.h>

using std::string;

char* ReadCommand() {
        static const unsigned int MAX_INPUT_SIZE = 128;
        static char input[MAX_INPUT_SIZE];
        fgets(input, MAX_INPUT_SIZE, stdin);
        input[strlen(input) - 1] = '\0';
        return input;
}


int main() {
    static const string EXIT_CODE = "exit";

    bool running = true;
    while (running) {
        printf("> ");
        char* command = ReadCommand();

        // do something
        if (command == EXIT_CODE) {
            running = false;
        }
        

        printf("string len is: %lu, value is: %s\n", strlen(command), command);
    }
}

extern "C" void TestStub() {
    printf("===> hello, Test Stub Successed!!!\n");
}

// https://docs.oracle.com/cd/E24847_01/html/E22196/chapter6-93046.html#chapter6-48031
