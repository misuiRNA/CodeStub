g++ src/ElfHandler.cpp src/ProcManual.cpp src/IOHandler.cpp src/FunctionInvoker.cpp\
    test/Test_ProcManual.cpp test/Test_ExternSym.cpp  \
    -std=c++11 \
    -Iinc \
    -O0 \
    -o test.exe
