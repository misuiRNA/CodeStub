#include "MessageQueue.h"

int main() {
    MessageQueue queue(MessageQueue::Server);
    int count = 5;
    while (count-- > 0) {
        printf("--- waitting...\n");
        Message msg = queue.take();
        printf("[%d]->: %s\n", count, msg.text);
    }
    return 1;
}
