#include <stdio.h>
#include <link.h>


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
