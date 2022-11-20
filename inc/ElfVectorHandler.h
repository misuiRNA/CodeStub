#include <vector>


template<typename Type>
class ElfVectorHandler {
    std::vector<Type> entries;

public:
    ElfVectorHandler(FILE* fp, size_t offset, size_t num) {
        Type* buff = new Type[num];
        fseek(fp, offset, SEEK_SET);
        fread(buff, num, sizeof(Type), fp);

        entries.clear();
        entries.reserve(num);
        for (size_t idx = 0; idx < num; ++idx) {
            entries.emplace_back(buff[idx]);
        }

        delete[] buff;
    }

    ElfVectorHandler(const ElfVectorHandler& oth) = delete;
    ElfVectorHandler& operator=(const ElfVectorHandler& oth) = delete;

    std::vector<Type> take() {
        return entries;
    }
};
