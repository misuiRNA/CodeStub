#ifndef __INCLUDE_FLAGE_PROCMANUAL__
#define __INCLUDE_FLAGE_PROCMANUAL__

#include "ElfHandler.h"

extern "C" typedef void (*StubFunctiong)(...);

class ProcManual {
public:
    ProcManual(const ElfHandler& handler);
    void execSymble(const char* name) const;
    void dumpFunctions() const;
    void dumpGlobalVariables() const;

private:
    size_t parseBaseAddr();

private:
    const ElfHandler& handler;
    size_t baseAddr;

    const static char* referSymName;
    const static size_t referSymAddr;
};

ProcManual CreateProcManual();

#endif
