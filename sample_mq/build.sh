CODE_DIR="../code"

g++ MessageQueue.cpp \
    -std=c++11 \
    -I${CODE_DIR}/inc \
    -O0 \
    -o testServer.exe


g++ MessageQueueClient.cpp \
    -std=c++11 \
    -I${CODE_DIR}/inc \
    -O0 \
    -o testClient.exe
