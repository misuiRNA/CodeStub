#ifndef __INCLUDE_FLAG_MESSAGEQUEUE_
#define __INCLUDE_FLAG_MESSAGEQUEUE_

#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>


struct Message {
    static constexpr long MSG_TYPE_PLAIN_TEXT = 1;

    Message()
    : msgType(MSG_TYPE_PLAIN_TEXT)
    , text{0}
    { }

    const long msgType;
    char text[512];
};

class MessageQueue {
public:
    enum Type {
        Server,
        Client
    };

    MessageQueue(Type type) : m_type(type) {
        key_t key = genKey();
        int mode = (type == Server ? IPC_CREAT | 0666 : IPC_EXCL | 0666);
        int queueId = msgget(key, mode);
        if (queueId == -1) {
            perror("failed to create message queue!");
        }
        m_queueId = queueId;
        // error branch
    }

    ~MessageQueue() {
        if (m_type == Type::Server) {
            msgctl(m_queueId, IPC_RMID, NULL);
            m_queueId = -1;
        }
    }

    Message take() {
        Message msg;
        if (msgrcv(m_queueId, &msg, sizeof(msg.text), msg.msgType, 0) == -1) {
            perror("msgrcv error!");
            // error branch
        }
        return msg;
    }

    bool push(const Message& msg) {
        if (msgsnd(m_queueId, &msg, sizeof(msg.text), 0) == -1) {
            perror("msgsnd error!");
             return false;
        }
        return true;
    }

private:

    key_t genKey() const {
        static const std::string path = ".";
        static const int ID = 'm';

        key_t key = ftok(path.c_str(), ID);
        return key;
    }

private:
    int m_queueId;
    Type m_type;
};

#endif