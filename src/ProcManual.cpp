#include "ProcManual.h"

ProcManual::ProcManual(const ElfHandler& handler, const char* referSymName, size_t referSymAddr)
: handler(handler)
, baseAddr(0)
{
    baseAddr = parseBaseAddr(referSymName, referSymAddr);
}

void ProcManual::execSymble(const char* name) const {
    const ElfW(Sym)* sym = handler.findSym(name);
    if (sym == nullptr) {
        printf("not found symble named '%s'!\n", name);
        return;
    }

    const char symType = ELF32_ST_TYPE(sym->st_info);
    if (symType == STT_FUNC) {
        StubFunctiong func = (StubFunctiong)(baseAddr + sym->st_value);
        func();
    } else if (symType == STT_OBJECT) {
        printf("%s: %d\n", name, *(int*)(baseAddr + sym->st_value));
    }
}

void ProcManual::dumpFunctions() const {
    printf("################ print functions start ################\n");
    // TODO do print function symbles
    printf("################ print functions end ################\n");
}

void ProcManual::dumpGlobalVariables() const {
    printf("################ print variables start ################\n");
    // TODO do print variable symbles
    printf("################ print variables end ################\n");
}

size_t ProcManual::parseBaseAddr(const char* referSymName, size_t referSymAddr) {
    const ElfW(Sym)* referSym = handler.findSym(referSymName);
    if (referSym == nullptr) {
        printf("not found symble named '%s'!\n", referSymName);
        return (size_t)NULL;
    }

    size_t addr = referSymAddr - referSym->st_value;
    return addr;
}
