#include <link.h>
#include <string>
#include "ElfBuffHandler.h"


class ElfParser {
    ElfW(Ehdr) ehdr;
    ElfBuffHandler<ElfW(Phdr)> phdrsHandler;
    ElfBuffHandler<ElfW(Shdr)> shdrsHandler;
    ElfBuffHandler<char> strTableHandler;
    ElfBuffHandler<char> symStrTableHandler;
    ElfBuffHandler<ElfW(Sym)> symsHandler;

public:
    ElfParser(const char* fileName);
    const ElfW(Ehdr)& getEhdr() const { return ehdr; }
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
