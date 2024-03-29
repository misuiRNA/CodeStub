#include "ProcManual.h"
#include <unistd.h>
#include "FunctionInvoker.h"

char __PROC_REFER_BASE_ADDRESS_SYMBLE__ = 0xFE;

const char* ProcManual::referSymName = "__PROC_REFER_BASE_ADDRESS_SYMBLE__";
const size_t ProcManual::referSymAddr = (size_t)&__PROC_REFER_BASE_ADDRESS_SYMBLE__;

ProcManual::ProcManual(const ElfHandler& handler)
: handler(handler)
, baseAddr(0)
{
    baseAddr = parseBaseAddr();
}

void ProcManual::execSymble(const char* name, const std::vector<int>& args) const {
    const ElfW(Sym)* sym = handler.findSym(name);
    if (sym == nullptr) {
        printf("not found symble named '%s'!\n", name);
        return;
    }

    const char symType = ELF64_ST_TYPE(sym->st_info);
    if (symType == STT_FUNC) {
        void* funcPtr = (void*)(baseAddr + sym->st_value);
        incokeFunction(funcPtr, args);
    } else if (symType == STT_OBJECT) {
        printf("%s: %d\n", name, *(int*)(baseAddr + sym->st_value));
    }
}

void ProcManual::dumpFunctions() const {
    printf("################ print functions start ################\n");
    printf(" - %-13s | %-13s | %-16s | %s\n", "bind", "type", "addr", "name");
    const std::vector<ElfW(Sym)>& symList = handler.listSyms();
    for (auto sym : symList) {
        const unsigned char symType = ELF64_ST_TYPE(sym.st_info);
        const unsigned char binding = ELF64_ST_BIND(sym.st_info);
        if (symType != STT_FUNC || binding != STB_GLOBAL) {
            continue;
        }
        const std::string& symName = handler.getSymName(sym);
        const std::string& symBindStr = handler.getSymBindStr(sym);
        const std::string& symTypeStr = handler.getSymTypeStr(sym);
        printf(" - %-13s | %-13s | 0x%-16lx | %s\n", symBindStr.c_str(), symTypeStr.c_str(), sym.st_value, symName.c_str());
    }
    printf("################ print functions end ################\n");
}

void ProcManual::dumpGlobalVariables() const {
    printf("################ print variables start ################\n");
    printf(" - %-13s | %-13s | %-16s | %s\n", "bind", "type", "addr", "name");
    const std::vector<ElfW(Sym)>& symList = handler.listSyms();
    for (auto sym : symList) {
        const unsigned char symType = ELF64_ST_TYPE(sym.st_info);
        const unsigned char binding = ELF64_ST_BIND(sym.st_info);
        if (symType != STT_OBJECT || binding != STB_GLOBAL) {
            continue;
        }
        const std::string& symName = handler.getSymName(sym);
        const std::string& symBindStr = handler.getSymBindStr(sym);
        const std::string& symTypeStr = handler.getSymTypeStr(sym);
        printf(" - %-13s | %-13s | 0x%-16lx | %s\n", symBindStr.c_str(), symTypeStr.c_str(), sym.st_value, symName.c_str());
    }
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

ProcManual GetProcManualInstance() {
    char elfPath[1024] = {0};
    int n = readlink("/proc/self/exe", elfPath, sizeof(elfPath));
    if( n > 0 && n < sizeof(elfPath)) {
        printf("process elf file: %s\n" , elfPath);
    }
    static ElfHandler handler(elfPath);    // TODO optimize
    // ProcManual manual(handler);
    return handler;
}
