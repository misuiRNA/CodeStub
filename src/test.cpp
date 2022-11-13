#include <link.h>
#include <string>
#include <vector>

using namespace std;
int retrieve_symbolnames(struct dl_phdr_info* info, size_t info_size, void* symbol_names_vector) 
{
    // vector<string>* symbol_names = reinterpret_cast<vector<string>*>(symbol_names_vector);
    printf("Name: %s (%d segments)\n", info->dlpi_name, info->dlpi_phnum);
    /* Iterate over all headers of the current shared lib
     * (first call is for the executable itself) */
    for (size_t headerIndex = 0; headerIndex < info->dlpi_phnum; headerIndex++)
    {
        const ElfW(Phdr) &elfHeader = info->dlpi_phdr[headerIndex];
        printf("    => header index: %lu, type: 0x%X\n", headerIndex, elfHeader.p_type);
        /* Further processing is only needed if the dynamic section is reached */
        // if (info->dlpi_phdr[header_index].p_type == PT_DYNAMIC)
        {

            /* Get a pointer to the first entry of the dynamic section.
             * It's address is the shared lib's address + the virtual address */
            ElfW(Dyn) *elfSegment = (ElfW(Dyn)*)(info->dlpi_addr +  elfHeader.p_vaddr);

            /* Iterate over all entries of the dynamic section until the
             * end of the symbol table is reached. This is indicated by
             * an entry with d_tag == DT_NULL.
             *
             * Only the following entries need to be processed to find the
             * symbol names:
             *  - DT_HASH   -> second word of the hash is the number of symbols
             *  - DT_STRTAB -> pointer to the beginning of a string table that
             *                 contains the symbol names
             *  - DT_SYMTAB -> pointer to the beginning of the symbols table
             */
            printf("     --> segment tag: %lX\n", elfSegment->d_tag);
            ElfW(Word) sym_cnt = 0;
            char* strtab = nullptr;
            char* sym_name = nullptr;
            ElfW(Word) *hash = nullptr;
            ElfW(Sym) *symbolTable = nullptr;

            while(elfSegment->d_tag != DT_NULL)
            {
                if (elfSegment->d_tag == DT_SYMENT)
                {
                    sym_cnt = elfSegment->d_un.d_val;
                    printf("      |--> 1 symbol count dt: %u\n", sym_cnt);
                }
                else if (elfSegment->d_tag == DT_HASH)
                {
                    // int* hash = (int*)elfSegment->d_un.d_ptr;
                    // printf("     --> hash addr: %p, value = 0x%X\n", hash, *hash);
                    printf("      |--> 4 hash\n");
                }
                else if (elfSegment->d_tag == DT_STRTAB)
                {
                    /* Get the pointer to the string table */
                    strtab = (char*)elfSegment->d_un.d_ptr;
                    printf("      |--> 2 string addr: %p\n", strtab);
                }
                else if (elfSegment->d_tag == DT_SYMTAB)
                {
                    symbolTable = (ElfW(Sym*))elfSegment->d_un.d_ptr;
                    printf("      |--> 3 symbol size = %u\n", sym_cnt);
                    /* Iterate over the symbol table */
                    // for (ElfW(Word) sym_index = 0; sym_index < sym_cnt; sym_index++)
                    // {
                    //     /* get the name of the i-th symbol.
                    //      * This is located at the address of st_name
                    //      * relative to the beginning of the string table. */
                    //     Elf64_Word nameIndex = symbolTable[sym_index].st_name;
                    //     printf("      |--> symbol name index = %d\n", nameIndex);
                    //     sym_name = &strtab[nameIndex];
                    //     printf("       --> symbol name = %s, type = %d\n", sym_name, symbolTable[sym_index].st_info);

                    //     // symbol_names->push_back(string(sym_name));
                    // }
                }

                /* move pointer to the next entry */
                elfSegment++;
            }
        }
    }

    /* Returning something != 0 stops further iterations,
     * since only the first entry, which is the executable itself, is needed
     * 1 is returned after processing the first entry.
     *
     * If the symbols of all loaded dynamic libs shall be found,
     * the return value has to be changed to 0.
     */
    return 1;

}

int method01() {
    printf("=================== %s ===========\n", __FUNCTION__);
    return 0;
}

int method02() {
    printf("=================== %s ===========\n", __FUNCTION__);
    return 0;
}

int globalCount = 0;

int main()
{
    method01();
    method02();
    vector<string> symbolNames;
    int res = dl_iterate_phdr(retrieve_symbolnames, &symbolNames);
    printf("=================== res is %d, elf list size is %lu  ===========\n", res, symbolNames.size());
    // for (auto it : symbolNames) {
    //     printf("---> %s\n", it.c_str());
    // }
    // printf("=================== done ===========\n");
    return 0;
}