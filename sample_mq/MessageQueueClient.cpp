#include "MessageQueue.h"
#include <cstring>


int main() {
    MessageQueue queue(MessageQueue::Client);
    Message msg;
    std::strcpy(msg.text, "Hello from parent process!");
    queue.push(msg);
    return 1;
}
