#include <gtest/gtest.h>
#include "FunctionInvoker.h"
#include <vector>

struct FunctionInvokerTest : public ::testing::Test {
    static int assertValue;

    static int assertMember_01;
    static int assertMember_02;
    static int assertMember_03;
    static int assertMember_04;
    static int assertMember_05;
    static int assertMember_06;

    void SetUp() override {
        assertValue = 0;
        assertMember_01 = 0;
        assertMember_02 = 0;
        assertMember_03 = 0;
        assertMember_04 = 0;
        assertMember_05 = 0;
        assertMember_06 = 0;
    }

    void TearDown() override {
        assertValue = 0;
    }

};
int FunctionInvokerTest::assertValue = 0;
int FunctionInvokerTest::assertMember_01 = 0;
int FunctionInvokerTest::assertMember_02 = 0;
int FunctionInvokerTest::assertMember_03 = 0;
int FunctionInvokerTest::assertMember_04 = 0;
int FunctionInvokerTest::assertMember_05 = 0;
int FunctionInvokerTest::assertMember_06 = 0;

static void MockFunc_Increase() {
    FunctionInvokerTest::assertValue += 1;
}

static void MockFunc_Add2Num(int left, int right) {
    FunctionInvokerTest::assertValue = left + right;
}

static void MockFunc_Store3Num(int a1, int a2, int a3) {
    FunctionInvokerTest::assertMember_01 = a1;
    FunctionInvokerTest::assertMember_02 = a2;
    FunctionInvokerTest::assertMember_03 = a3;
}

static void MockFunc_Store5Num(int a1, int a2, int a3, int a4, int a5) {
    FunctionInvokerTest::assertMember_01 = a1;
    FunctionInvokerTest::assertMember_02 = a2;
    FunctionInvokerTest::assertMember_03 = a3;
    FunctionInvokerTest::assertMember_04 = a4;
    FunctionInvokerTest::assertMember_05 = a5;
}

static void MockFunc_Store6Num(int a1, int a2, int a3, int a4, int a5, int a6) {
    FunctionInvokerTest::assertMember_01 = a1;
    FunctionInvokerTest::assertMember_02 = a2;
    FunctionInvokerTest::assertMember_03 = a3;
    FunctionInvokerTest::assertMember_04 = a4;
    FunctionInvokerTest::assertMember_05 = a5;
    FunctionInvokerTest::assertMember_06 = a6;
}

TEST_F(FunctionInvokerTest, test_invoke_function_without_param) {
    incokeFunction((void*)&MockFunc_Increase, std::vector<int>{});
    ASSERT_EQ(1, assertValue);
}

TEST_F(FunctionInvokerTest, test_invoke_function_with_2int_param) {
    incokeFunction((void*)&MockFunc_Add2Num, std::vector<int>{2, 3});
    ASSERT_EQ(5, assertValue);
}

TEST_F(FunctionInvokerTest, test_invoke_function_with_5int) {
    incokeFunction((void*)&MockFunc_Store5Num, std::vector<int>{99, 98, 97, 96, 95});
    ASSERT_EQ(99, assertMember_01);
    ASSERT_EQ(98, assertMember_02);
    ASSERT_EQ(97, assertMember_03);
    ASSERT_EQ(96, assertMember_04);
    ASSERT_EQ(95, assertMember_05);
}

TEST_F(FunctionInvokerTest, test_invoke_function_with_6int_will_use_top5) {
    incokeFunction((void*)&MockFunc_Store5Num, std::vector<int>{99, 98, 97, 96, 95, 94});
    ASSERT_EQ(99, assertMember_01);
    ASSERT_EQ(98, assertMember_02);
    ASSERT_EQ(97, assertMember_03);
    ASSERT_EQ(96, assertMember_04);
    ASSERT_EQ(95, assertMember_05);
    ASSERT_EQ(0, assertMember_06);
}

TEST_F(FunctionInvokerTest, test_invoke_function_with_5int_given_2int_will_use_top2) {
    incokeFunction((void*)&MockFunc_Store3Num, std::vector<int>{99, 98});
    ASSERT_EQ(99, assertMember_01);
    ASSERT_EQ(98, assertMember_02);
    ASSERT_EQ(0, assertMember_03);
    ASSERT_EQ(0, assertMember_04);
    ASSERT_EQ(0, assertMember_05);
    ASSERT_EQ(0, assertMember_06);
}

TEST_F(FunctionInvokerTest, test_invoke_function_with_3int_given_5int_will_use_top3) {
    incokeFunction((void*)&MockFunc_Store3Num, std::vector<int>{99, 98, 97, 96, 95});
    ASSERT_EQ(99, assertMember_01);
    ASSERT_EQ(98, assertMember_02);
    ASSERT_EQ(97, assertMember_03);
    ASSERT_EQ(0, assertMember_04);
    ASSERT_EQ(0, assertMember_05);
}

