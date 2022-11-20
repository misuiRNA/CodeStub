#ifndef __INCLUDE_FLAGE_ELFELEMENTHOLDER__
#define __INCLUDE_FLAGE_ELFELEMENTHOLDER__

#include <vector>


template<typename Type>
class ElfElementHolder {
    Type* buffArray;
    char* count;

public:
    ElfElementHolder() : buffArray(nullptr), count(nullptr) { }

    ElfElementHolder(FILE* fp, size_t offset, size_t num) : buffArray(new Type[num]), count(new char(0)) {
        fseek(fp, offset, SEEK_SET);
        fread(buffArray, num, sizeof(Type), fp);
        increaseCount();
    }

    ElfElementHolder(const ElfElementHolder& oth) {
        this->buffArray = oth.buffArray;
        this->count = oth.count;
        increaseCount();
    }

    ElfElementHolder& operator=(const ElfElementHolder& oth) {
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

    ~ElfElementHolder() {
        decreaseCount();
        if (shouldRelease()) {
            releae();
        }
    }

    const Type* get() const {
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

#endif
