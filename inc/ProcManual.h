#ifndef __INCLUDE_FLAGE_PROCMANUAL__
#define __INCLUDE_FLAGE_PROCMANUAL__

#include <vector>
#include "ElfHandler.h"

class ProcManual {
public:
    ProcManual(const ElfHandler& handler);
    void execSymble(const char* name, const std::vector<int>& args) const;
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

ProcManual GetProcManualInstance();

#endif
