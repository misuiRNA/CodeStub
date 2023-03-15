g++ code/src/ElfHandler.cpp \
    code/src/ProcManual.cpp \
    code/src/TerminalHandler.cpp \
    code/src/FunctionInvoker.cpp\
    test/Test_ProcManual.cpp \
    test/Test_ExternSym.cpp  \
    -std=c++11 \
    -Icode/inc \
    -O0 \
    -o test.exe
