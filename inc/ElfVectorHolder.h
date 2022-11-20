#ifndef __INCLUDE_FLAGE_ELFVECTORHOLDER__
#define __INCLUDE_FLAGE_ELFVECTORHOLDER__

#include <vector>


template<typename Type>
class ElfVectorHolder {
    std::vector<Type> entries;

public:
    ElfVectorHolder(FILE* fp, size_t offset, size_t num) {
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

    ElfVectorHolder(const ElfVectorHolder& oth) = delete;
    ElfVectorHolder& operator=(const ElfVectorHolder& oth) = delete;

    std::vector<Type> take() {
        return entries;
    }
};

#endif
