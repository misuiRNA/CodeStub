#include <gtest/gtest.h>
#include "ProcManual.h"

struct ProcManualTest : public ::testing::Test {
    const static ProcManual manual;
    static int assertValue;

    void SetUp() override {
        assertValue = 0;
    }

    void TearDown() override {
        assertValue = 0;
    }

};
const ProcManual ProcManualTest::manual = GetProcManualInstance();
int ProcManualTest::assertValue = 0;


extern "C" void MockFunc_Increase() {
    ProcManualTest::assertValue += 1;
}

extern "C" void MockFunc_Add2Num(int left, int right) {
    ProcManualTest::assertValue = left + right;
}

TEST_F(ProcManualTest, test_call_function_without_param) {
    manual.execSymble("MockFunc_Increase", std::vector<int>{});
     ASSERT_EQ(1, assertValue);
}

TEST_F(ProcManualTest, test_call_function_with_2int_param) {
    manual.execSymble("MockFunc_Add2Num", std::vector<int>{88, 99});
     ASSERT_EQ(187, assertValue);
}
