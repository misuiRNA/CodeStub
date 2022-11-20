#include <link.h>
#include <string>
#include <vector>
#include <string>
#include <map>

class ElfParser {
    using ShOffset = size_t;
    using string = std::string;

    ElfW(Ehdr) ehdr;
    std::vector<ElfW(Phdr)> phdrs;
    std::vector<ElfW(Shdr)> shdrs;
    std::vector<ElfW(Sym)> syms;
    
    std::map<ShOffset, std::string> strTable;
    std::map<ShOffset, std::string> symStrTable;

public:
    ElfParser(const char* fileName);
    const ElfW(Ehdr)& getEhdr() const { return ehdr; }
    const std::vector<ElfW(Phdr)>& listPhdrs() const { return phdrs; }
    const std::vector<ElfW(Shdr)>& listShdrs() const { return shdrs; }
    const std::vector<ElfW(Sym)>& listSyms() const { return syms; }
    const std::map<ShOffset, std::string>& getStrTable1() const { return strTable; }
    const std::map<ShOffset, std::string>& getSymStrTable1() const { return symStrTable; }

    size_t getStrTableSize() { return shdrs[ehdr.e_shstrndx].sh_size; }
    size_t getSymSize() const;

    const ElfW(Sym)* findSym(const string& name) const;

private:
    const ElfW(Shdr)& findSectionNameStrTableShdr() const;
    const ElfW(Shdr)& findSymbleNameStrTableShdr() const;
    const ElfW(Shdr)& findSymbleTableShdr() const;
};
