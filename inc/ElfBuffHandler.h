#include <vector>


template<typename BuffType>
class ElfBuffHandler {
    BuffType* buffArray;
    char* count;

public:
    ElfBuffHandler() : buffArray(nullptr), count(nullptr) { }

    ElfBuffHandler(FILE* fp, size_t offset, size_t num) : buffArray(new BuffType[num]), count(new char(0)) {
        fseek(fp, offset, SEEK_SET);
        fread(buffArray, num, sizeof(BuffType), fp);
        increaseCount();
    }

    ElfBuffHandler(const ElfBuffHandler& oth) {
        this->buffArray = oth.buffArray;
        this->count = oth.count;
        increaseCount();
    }

    ElfBuffHandler& operator=(const ElfBuffHandler& oth) {
        if (this == &oth) {
            return *this;
        }
        decreaseCount();
        if (shouldRelease()) {
            releae();
        }

        this->buffArray = oth.buffArray;
        this->count = oth.count;
        increaseCount();
        return *this;
    }

    ~ElfBuffHandler() {
        decreaseCount();
        if (shouldRelease()) {
            releae();
        }
    }

    const BuffType* getConst() const {
        return buffArray;
    }

private:
    void increaseCount() {
        if (count == nullptr) {
            return;
        }
        ++(*count);
    }

    void decreaseCount() {
        if (count == nullptr) {
            return;
        }
        --(*count);
    }

    bool shouldRelease() {
        return (count == nullptr) || (*count <= 0);
    }

    void releae() {
        delete[] buffArray;
        buffArray = nullptr;

        delete count;
        count = nullptr;
    }
};
