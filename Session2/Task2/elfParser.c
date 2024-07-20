#include <stdio.h>
#include <elf.h>
#include <string.h>
#include "machines.h"

void print_e_ident(unsigned char* e_ident)
{
    char os[50];
     switch (e_ident[7]) {
        case 0:
            strcpy(os, "UNIX - System V");
            break;
        case 1:
            strcpy(os, "HP-UX");
            break;
        case 3:
            strcpy(os, "NetBSD");
            break;
        case 4:
            strcpy(os, "Linux");
            break;
        default:
            strcpy(os, "Other");
            break;
    }

    printf("  Magic: ");
    for(int i = 0; i < EI_NIDENT; i++)
    {
	 printf("%02x ", e_ident[i]);
    }
    printf("\n");

    printf("  Class:				 %s\n", (e_ident[4] == 1)? "ELF32" : "ELF64");
    printf("  Data:					 2's complement, %s\n", (e_ident[5] == 1)? "little endian" : "big  endian");
    printf("  Version:				 %s\n", (e_ident[6] == 1)? "1 (current)" : "Unkwon");
    printf("  OS/ABI:				 %s\n", os);
    printf("  ABI Version:				 %u\n", e_ident[8]);


}
int main(int argc, char** argv)
{
   FILE* elfFile = fopen(argv[1], "r");
   
   if(elfFile == NULL)
   {
	return 1;
   }

   Elf64_Ehdr elfHeader;

   fread(&elfHeader, sizeof(elfHeader), 1, elfFile);
   
   printf("ELF Header:\n");
   
   print_e_ident(elfHeader.e_ident);
   printf("  Type:                             	 %s\n", 
        (elfHeader.e_type == 1) ? "REL (Relocatable file)" :
        (elfHeader.e_type == 2) ? "EXEC (Executable file)" :
        (elfHeader.e_type == 3) ? "DYN (Shared object file)" :
        (elfHeader.e_type == 4) ? "CORE (Core file)" : "Unknown");
   printf("  Machine:		                 %s\n", get_machine_type_description(elfHeader.e_machine));
   printf("  Version:                          	 0x%x\n", elfHeader.e_version);
   printf("  Entry point address:              	 0x%lx\n", elfHeader.e_entry);
   printf("  Start of program headers:         	 %lu (bytes into file)\n", elfHeader.e_phoff);
   printf("  Start of section headers:         	 %lu (bytes into file)\n", elfHeader.e_shoff);
   printf("  Flags:                            	 0x%x\n", elfHeader.e_flags);
   printf("  Size of this header:              	 %u (bytes)\n", elfHeader.e_ehsize);
   printf("  Size of program headers:          	 %u (bytes)\n", elfHeader.e_phentsize);
   printf("  Number of program headers:         	 %u\n", elfHeader.e_phnum);
   printf("  Size of section headers:           	 %u (bytes)\n", elfHeader.e_shentsize);
   printf("  Number of section headers:        	 %u\n", elfHeader.e_shnum);
   printf("  Section header string table index:	 %u\n", elfHeader.e_shstrndx);

   fclose(elfFile);

   return 0;
}