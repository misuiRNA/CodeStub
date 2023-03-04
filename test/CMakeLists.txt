cmake_minimum_required(VERSION 3.14)
project(code_test)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)
set(CODE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

include_directories(
    ${CODE_ROOT_DIR}/inc
)

file(GLOB SRC_DIR
    ${CODE_ROOT_DIR}/src/*.cpp
    test_cast/FunctionInvokerTest.cpp
)

add_executable(code_test ${SRC_DIR})
target_link_libraries(code_test GTest::gtest_main)

include(
    GoogleTest
)
gtest_add_tests(TARGET code_test)
