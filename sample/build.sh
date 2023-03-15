CODE_DIR="../code"

g++ ${CODE_DIR}/src/ElfHandler.cpp \
    ${CODE_DIR}/src/ProcManual.cpp \
    ${CODE_DIR}/src/TerminalHandler.cpp \
    ${CODE_DIR}/src/FunctionInvoker.cpp\
    ProccessManual.cpp \
    ExternFunctions.cpp  \
    -std=c++11 \
    -I${CODE_DIR}/inc \
    -O0 \
    -o test.exe
