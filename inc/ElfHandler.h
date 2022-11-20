#ifndef __INCLUDE_FLAGE_ELFHANDLER__
#define __INCLUDE_FLAGE_ELFHANDLER__

#include <link.h>
#include <string>
#include <vector>
#include <string>
#include <map>

class ElfHandler {
    using ShOffset = size_t;
    using string = std::string;

    ElfW(Ehdr) ehdr;
    std::vector<ElfW(Phdr)> phdrs;
    std::vector<ElfW(Shdr)> shdrs;
    std::vector<ElfW(Sym)> syms;
    
    std::map<ShOffset, std::string> strTable;
    std::map<ShOffset, std::string> symStrTable;

public:
    ElfHandler(const char* fileName);
    const ElfW(Ehdr)& getEhdr() const { return ehdr; }
    const std::vector<ElfW(Phdr)>& listPhdrs() const { return phdrs; }
    const std::vector<ElfW(Shdr)>& listShdrs() const { return shdrs; }
    const std::vector<ElfW(Sym)>& listSyms() const { return syms; }

    const ElfW(Sym)* findSym(const string& name) const;

private:
    const ElfW(Shdr)& findSectionNameStrTableShdr() const;
    const ElfW(Shdr)& findSymbleNameStrTableShdr() const;
    const ElfW(Shdr)& findSymbleTableShdr() const;
};

#endif
