#include <stdio.h>

extern "C" int method03(int a, int b) {
    printf("##################### call method %s, a = %d, b = %d\n", __FUNCTION__, a, b);
    return 0;
}

int globalInteger = 1;