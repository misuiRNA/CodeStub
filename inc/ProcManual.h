#ifndef __INCLUDE_FLAGE_PROCMANUAL__
#define __INCLUDE_FLAGE_PROCMANUAL__

#include "ElfHandler.h"

extern "C" typedef void (*StubFunctiong)();

class ProcManual {
    const ElfHandler& handler;
    size_t baseAddr;

public:
    ProcManual(const ElfHandler& handler, const char* referSymName, size_t referSymAddr);
    void execSymble(const char* name) const;
    void dumpFunctions() const;
    void dumpGlobalVariables() const;

private:
    size_t parseBaseAddr(const char* referSymName, size_t referSymAddr);
};

#endif
