#ifndef __INCLUDE_FLAGE_IOHANDLER__
#define __INCLUDE_FLAGE_IOHANDLER__

#include <string>
#include <vector>

std::string ReadLine();

struct ManualCommand {
    const char* name;
    std::vector<int> args;

    static ManualCommand ParseCommand(const char* input);
};

#endif
