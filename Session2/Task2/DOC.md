# ELF Parser Commands and Output

## Compilation Commands

1. **Compile `elfParser.c` and `machines.c` into an executable named `elfParser`:**

   ```bash
   gcc elfParser.c machines.c -o elfParser
   
2. **Compile `f.c` into an object file named `f.o`:**
   
   ```bash
   gcc f.c -o f.o

3. **Run the `elfParser` executable with `f` as the argument:**
   ```bash
   ./elfParser f

## ELF Header Information
**The output of the `elfParser` program when executed with the `f` file is as follows:**
```less
ELF Header:
  Magic: 7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                     ELF64
  Data:                      2's complement, little endian
  Version:                   1 (current)
  OS/ABI:                    UNIX - System V
  ABI Version:               0
  Type:                      DYN (Shared object file)
  Machine:                   AMD x86-64
  Version:                   0x1
  Entry point address:       0x1060
  Start of program headers:  64 (bytes into file)
  Start of section headers:  13968 (bytes into file)
  Flags:                     0x0
  Size of this header:       64 (bytes)
  Size of program headers:   56 (bytes)
  Number of program headers: 13
  Size of section headers:   64 (bytes)
  Number of section headers: 31
  Section header string table index: 30

