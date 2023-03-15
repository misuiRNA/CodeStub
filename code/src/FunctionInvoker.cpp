#include <iostream>
#include <vector>
#include "FunctionInvoker.h"

namespace {
    typedef void (*Function_v)();
    typedef void (*Function_1i)(int);
    typedef void (*Function_2i)(int, int);
    typedef void (*Function_3i)(int, int, int);
    typedef void (*Function_4i)(int, int, int, int);
    typedef void (*Function_5i)(int, int, int, int, int);
}

void incokeFunction(void* function, const std::vector<int>& args) {
    switch (args.size()) {
        case 0:
            ((Function_v)function)();
            break;
        case 1:
            ((Function_1i)function)(args[0]);
            break;
        case 2:
            ((Function_2i)function)(args[0], args[1]);
            break;
        case 3:
            ((Function_3i)function)(args[0], args[1], args[2]);
            break;
        case 4:
            ((Function_4i)function)(args[0], args[1], args[2], args[3]);
            break;
        default:
            ((Function_5i)function)(args[0], args[1], args[2], args[3], args[4]);
            break;
    }
}
