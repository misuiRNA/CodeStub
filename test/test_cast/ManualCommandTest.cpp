#include <gtest/gtest.h>
#include "TerminalHandler.h"

struct ManualCommandTest : public ::testing::Test {

};

TEST_F(ManualCommandTest, test_function_without_param) {
    std::string input = "testFunc_001";
    TerminalCommand command = TerminalCommand::ParseCommand(input.c_str());
    EXPECT_STREQ("testFunc_001", command.name);
    EXPECT_EQ(std::vector<int> {}, command.args);

    input = "testFunc_002, ";
    command = TerminalCommand::ParseCommand(input.c_str());
    EXPECT_STREQ("testFunc_002", command.name);
    EXPECT_EQ(std::vector<int> {}, command.args);
}

TEST_F(ManualCommandTest, test_function_with_2int) {
    std::string input = "testFunc_001, 1, 5";
    TerminalCommand command = TerminalCommand::ParseCommand(input.c_str());
    EXPECT_STREQ("testFunc_001", command.name);
    EXPECT_EQ((std::vector<int> {1, 5}), command.args);

    input = "testFunc_002, 22233, 55589, ";
    command = TerminalCommand::ParseCommand(input.c_str());
    EXPECT_STREQ("testFunc_002", command.name);
    EXPECT_EQ((std::vector<int> {22233, 55589}), command.args);
}

TEST_F(ManualCommandTest, test_function_with_5int) {
    std::string input = "testFunc_001, 1, 5, 8, 9, 36";
    TerminalCommand command = TerminalCommand::ParseCommand(input.c_str());
    EXPECT_STREQ("testFunc_001", command.name);
    EXPECT_EQ((std::vector<int> {1, 5, 8, 9, 36}), command.args);
}

TEST_F(ManualCommandTest, test_function_with_2float_will_parse_2int) {
    std::string input = "testFunc_001, 1.3, 57.125";
    TerminalCommand command = TerminalCommand::ParseCommand(input.c_str());
    EXPECT_STREQ("testFunc_001", command.name);
    EXPECT_EQ((std::vector<int> {1, 57}), command.args);
}

TEST_F(ManualCommandTest, test_function_with_intWithBlank) {
    std::string input = "testFunc_001, 1 38, 5 8 7 9";
    TerminalCommand command = TerminalCommand::ParseCommand(input.c_str());
    EXPECT_STREQ("testFunc_001", command.name);
    EXPECT_EQ((std::vector<int> {138, 5879}), command.args);
}

TEST_F(ManualCommandTest, test_function_with_illegalChar_will_ignore) {
    std::string input = "testFunc_001, 1 +38, 5 8p7 9";
    TerminalCommand command = TerminalCommand::ParseCommand(input.c_str());
    EXPECT_STREQ("testFunc_001", command.name);
    EXPECT_EQ((std::vector<int> {1, 58}), command.args);
}
