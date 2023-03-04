#include <stdio.h>
#include "FunctionInvoker.h"


void func001() {
    printf("#### No.00\n");
}

void func002(int a) {
    printf("#### No.01 %d\n", a);
}

void func003(int a, int b) {
    printf("#### No.02 %d, %d\n", a, b);
}

void func004(int a, int b = -2, int c = -2) {
    printf("#### No.03 %d, %d, %d\n", a, b, c);
}

void func005(int a, int b, int c, int d) {
    printf("#### No.04 %d, %d, %d, %d\n", a, b, c, d);
}

template<typename... Args>
std::vector<int> makeParams(Args... args) {
    std::vector<int> res = {args...};
    return res;
}

int main_() {
    incokeFunction((void*)&func001, makeParams());
    incokeFunction((void*)&func002, makeParams(1, 3, 9));
    incokeFunction((void*)&func003, makeParams(1, 2));
    incokeFunction((void*)&func004, makeParams(199));
    incokeFunction((void*)&func005, makeParams(1, 2, 3, 4, 5));
}
