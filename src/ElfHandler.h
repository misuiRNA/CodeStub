#include <link.h>
#include <string>


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

    BuffType* get() {
        return buffArray;
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


class ElfHandler {
    ElfW(Ehdr) ehdr;
    ElfBuffHandler<ElfW(Phdr)> phdrsHandler;
    ElfBuffHandler<ElfW(Shdr)> shdrsHandler;
    ElfBuffHandler<char> strTableHandler;
    ElfBuffHandler<char> symStrTableHandler;
    ElfBuffHandler<ElfW(Sym)> symsHandler;

public:
    ElfHandler(const char* fileName);
    const ElfW(Ehdr)& getEhdr() const {return ehdr; }
    const ElfW(Phdr)* getPhdrs() const { return phdrsHandler.getConst(); }
    const ElfW(Shdr)* getShdrs() const { return shdrsHandler.getConst(); }
    const char* getStrTable() const { return strTableHandler.getConst(); }
    const char* getSymStrTable() const { return symStrTableHandler.getConst(); }
    const ElfW(Sym)* getSymbles() const { return symsHandler.getConst(); }

    size_t getStrTableSize() { return getShdrs()[ehdr.e_shstrndx].sh_size; }

private:
    const ElfW(Shdr)& findSectionNameStrTableShdr() const;
    const ElfW(Shdr)& findSymbleNameStrTableShdr() const;
    const ElfW(Shdr)& findSymbleTableShdr() const;
};
