#include "ProcManual.h"

char __PROC_REFER_BASE_ADDRESS_SYMBLE__ = 0xFE;

const char* ProcManual::referSymName = "__PROC_REFER_BASE_ADDRESS_SYMBLE__";
const size_t ProcManual::referSymAddr = (size_t)&__PROC_REFER_BASE_ADDRESS_SYMBLE__;

ProcManual::ProcManual(const ElfHandler& handler)
: handler(handler)
, baseAddr(0)
{
    baseAddr = parseBaseAddr();
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
        // TODO 把外部传入的参数作为入参
        func(0xFF, 1);
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

size_t ProcManual::parseBaseAddr() {
    const ElfW(Sym)* referSym = handler.findSym(referSymName);
    if (referSym == nullptr) {
        printf("not found symble named '%s'!\n", referSymName);
        return (size_t)NULL;
    }

    size_t addr = referSymAddr - referSym->st_value;
    return addr;
}
