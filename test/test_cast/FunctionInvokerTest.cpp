#include <gtest/gtest.h>
#include "FunctionInvoker.h"
#include <vector>

int assertVersion = 0;

void MockFunc_ParamVoid() {
    assertVersion = assertVersion + 1;
}

void MockFunc_Add2Num(int a, int b) {
    assertVersion = assertVersion + a + b;
}


TEST(FunctionInvokerTest, test_invoke_function_with_void_param) {
    assertVersion = 0;
    incokeFunction((void*)&MockFunc_ParamVoid, std::vector<int>{});
    EXPECT_EQ(1, assertVersion);
    assertVersion = 0;
}

TEST(FunctionInvokerTest, test_invoke_function_with_2int_param) {
    assertVersion = 0;
    incokeFunction((void*)&MockFunc_Add2Num, std::vector<int>{2, 3});
    EXPECT_EQ(2 + 3, assertVersion);
    assertVersion = 0;
}

