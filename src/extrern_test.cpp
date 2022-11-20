#include <stdio.h>

extern "C" int method03() {
    printf("=================== %s ===========\n", __FUNCTION__);
    return 0;
}

int globalInteger = 1;