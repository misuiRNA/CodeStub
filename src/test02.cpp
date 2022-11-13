#include <link.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <string.h>

void printPhdrHeader(const Elf64_Phdr& header) {
    printf("======== Elf64_Phdr start ========\n");
    printf("    |- p_type   = %d\n", header.p_type);
    printf("    |- p_flags  = %d\n", header.p_flags);
    printf("    |- p_offset = %lu\n", header.p_offset);
    printf("    |- p_vaddr  = %p\n", (void*)header.p_vaddr);
    printf("    |- p_paddr  = %p\n", (void*)header.p_paddr);
    printf("    |- p_filesz = %lu\n", header.p_filesz);
    printf("    |- p_memsz  = %lu\n", header.p_memsz);
    printf("    |- p_align  = %lu\n", header.p_align);
    printf("======== Elf64_Phdr end ========\n");
}

using namespace std;
int retrieveSymbolNames(struct dl_phdr_info* info, size_t info_size, void* symbol_names_vector) 
{
    printf("Name: %s (%d segments)\n", info->dlpi_name, info->dlpi_phnum);
    for (size_t headerIndex = 0; headerIndex < info->dlpi_phnum; headerIndex++)
    {
        const ElfW(Phdr) &elfHeader = info->dlpi_phdr[headerIndex];
        // printPhdrHeader(elfHeader);
        printf("    => header index: %lu, type: 0x%X\n", headerIndex, elfHeader.p_type);
        ElfW(Dyn) *elfSegment = (ElfW(Dyn)*)(info->dlpi_addr +  elfHeader.p_paddr);
        printf("     --> segment tag: %lX\n", elfSegment->d_tag);

        ElfW(Word) sym_cnt = 0;
        char* strtab = nullptr;
        char* sym_name = nullptr;
        ElfW(Word) *hash = nullptr;
        ElfW(Sym) *symbolTable = nullptr;

        // size_t strSize = 1261;
        size_t strSize = 1000;

        while(elfSegment->d_tag != DT_NULL)
        {
            if (elfSegment->d_tag == DT_STRSZ)
            {
                strSize = elfSegment->d_un.d_val;
                printf("      |--> 1 string table size: %lu\n", strSize);
            }
            else if (elfSegment->d_tag == DT_STRTAB)
            {
                printf("      |--> 2 string table size: %lu\n", strSize);
                printf("      |-->string: ");
                // strtab = (char*)elfSegment->d_un.d_ptr;
                strtab = (char*)&elfHeader;
                for (size_t index = 0; index < strSize; ++index)
                {
                    if (*(strtab + index) == 0)
                    {
                        printf(",    ");
                    }
                    else 
                    {
                        printf("%c", *(strtab + index));
                    }
                }
                printf("\n");
            }
            elfSegment++;
        }
    }
    return 1;
}


size_t SectionNameStrSize = 0;
char* SectionNameStrTable = nullptr;

void printStrTable(size_t strSize, char* strTable) {
    printf("======== print string table begin ========\n");
    printf(" string size: %lu\n", strSize);
    printf(" string  addr: %p\n", strTable);
    printf(" string items: ");
    for (size_t strIdx = 0; strIdx < strSize; ++strIdx) {
        if (*(strTable + strIdx) == 0) {
            printf("\n  --> ");
        } else {
            printf("%c", *(strTable + strIdx));
        }
    }
    printf("\n");
    printf("======== print string table end ========\n");
}

void retrievePhdr(size_t phdrNum, ElfW(Phdr)* phdrs, FILE* fp) {
    for (size_t phdrIdx = 0; phdrIdx < phdrNum; ++phdrIdx) {
        ElfW(Phdr)& phdr = phdrs[phdrIdx];
        printf("==> header phdrIdx: %lu, type: 0x%X\n", phdrIdx, phdr.p_type);

        ElfW(Dyn*) dyns = new ElfW(Dyn)[phdr.p_filesz];
        fseek(fp, phdr.p_offset, SEEK_SET);
        fread(dyns, phdr.p_filesz, sizeof(char), fp);

        size_t strSize = 285;
        char* strTab = nullptr;
        for (ElfW(Dyn*) dyn = dyns; dyn->d_tag != DT_NULL; ++dyn) {
            if (dyn->d_tag == DT_STRSZ) {
                strSize = dyn->d_un.d_val;
                printf("      |--> 1 string table size: %lu\n", strSize);
            } else if (dyn->d_tag == DT_STRTAB) {
                printf("      |--> 2 string table size: %lu\n", strSize);
                // strTab = (char*)dyn->d_un.d_ptr;
                strTab = new char[strSize];
                fseek(fp, dyn->d_un.d_ptr, SEEK_SET);
                fread(strTab, strSize, sizeof(char), fp);
                printStrTable(strSize, strTab);
                delete[] strTab;
                strTab = nullptr;
            }
        }
    }
}

void retrieveShdr(size_t shdrNum, ElfW(Shdr)* shdrs, FILE* fp) {
    size_t strNum = 0;
    char* strTable = nullptr;
    for (size_t shdrIdx = 0; shdrIdx < shdrNum; ++shdrIdx) {
        ElfW(Shdr)& shdr = shdrs[shdrIdx];
        printf("==> header phdrIdx: %lu, type: 0x%X\n", shdrIdx, shdr.sh_type);

        if (shdr.sh_type == SHT_STRTAB && strcmp(&SectionNameStrTable[shdr.sh_name], ".strtab") == 0) {
            printf("################################ 001\n");
            strNum = shdr.sh_size;
            strTable = new char[strNum];
            fseek(fp, shdr.sh_offset, SEEK_SET);
            fread(strTable, strNum, sizeof(char), fp);
            // printf("### string table name: %s\n", &SectionNameStrTable[shdr.sh_name]);
            // printStrTable(strNum, strTable);
        }
    }

    for (size_t shdrIdx = 0; shdrIdx < shdrNum; ++shdrIdx) {
        ElfW(Shdr)& shdr = shdrs[shdrIdx];
        if (shdr.sh_type == SHT_SYMTAB) {
            printf("################################ 002\n");
            size_t symNum = shdr.sh_size / sizeof(ElfW(Sym));
            ElfW(Sym)* syms = new ElfW(Sym)[symNum];
            fseek(fp, shdr.sh_offset, SEEK_SET);
            fread(syms, symNum, sizeof(ElfW(Sym)), fp);
            for (size_t symIdx = 0; symIdx < symNum; ++symIdx) {
                ElfW(Sym)& sym = syms[symIdx];
                printf("======== print symbol begin ========\n");
                printf(" name: %s\n", &strTable[sym.st_name]);
                printf("======== print symbol end ========\n");
            }

            delete[] syms;
            syms = nullptr;
        }
    }

    delete[] strTable;
    strTable = nullptr;
}

extern void method03();
extern int globalInteger;

int variable01 = 0;

int method01() {
    printf("=================== %s ===========\n", __FUNCTION__);
    return 0;
}

int method02() {
    printf("=================== %s ===========\n", __FUNCTION__);
    return 0;
}

int globalCount = 0;

int main() {
    method01();
    method02();
    method03();
    variable01 = 1;
    globalInteger = 5;

    char buf[1024] = {0};
    int n = readlink("/proc/self/exe", buf, sizeof(buf));
    if( n > 0 && n < sizeof(buf)) {
        printf("process elf file: %s\n" , buf);
    }

    FILE *fp = fopen(buf, "r");
    if (!fp) {
        printf("Failed to open file: %s\n", buf);
        return 0;
    }

    ElfW(Ehdr) ehdr; 
    fread(&ehdr, 1, sizeof(ehdr), fp);

    // read phdrs
    ElfW(Phdr)* phdrs = new ElfW(Phdr)[ehdr.e_phnum];
    fseek(fp, ehdr.e_phoff, SEEK_SET);
    fread(phdrs, ehdr.e_phnum, sizeof(ElfW(Phdr)), fp);

    // read shdrs
    ElfW(Shdr)* shdrs = new ElfW(Shdr)[ehdr.e_shnum];
    fseek(fp, ehdr.e_shoff, SEEK_SET);
    fread(shdrs, ehdr.e_shnum, sizeof(ElfW(Shdr)), fp);

    // read section names string table
    size_t strSize = shdrs[ehdr.e_shstrndx].sh_size;
    char* strTable = new char[strSize];
    fseek(fp, shdrs[ehdr.e_shstrndx].sh_offset, SEEK_SET);
    fread(strTable, strSize, sizeof(char), fp);
    
    SectionNameStrSize = strSize;
    SectionNameStrTable = strTable;

    // retrievePhdr(ehdr.e_phnum, phdrs, fp);
    retrieveShdr(ehdr.e_shnum, shdrs, fp);
    // printStrTable(strSize, strTable);

    // release resource
    delete[] strTable;
    strTable = nullptr;
    SectionNameStrTable = strTable;

    return 0;
}