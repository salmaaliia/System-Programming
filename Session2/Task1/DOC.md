# ELF File Compilation and Analysis

## Compilation Commands

1. **Compile `a.c` into an object file named `a.o`:**

   ```bash
   gcc -c a.c -o a.o

2. **Compile `b.c` into an object file named `b.o`:**
   ```bash
   gcc -c b.c -o b.o

3. **Link `a.o` and `b.o` into an executable named `myfile`:**
   ```bash
   gcc a.o b.o -o myfile

4. **Run the `myfile` executable:**
   ```bash
   ./myfile

# Symbol Table of Object Files

## Symbol Table of `a.o`

 **Use the following command to view the symbol table of `a.o`:**
 ```bash
  readelf -s a.o
**Output**
```less
File: a.o

Symbol table '.symtab' contains 9 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS a.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 .data
     4: 0000000000000004     4 OBJECT  LOCAL  DEFAULT    3 l.0
     5: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    3 s
     6: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    4 k
     7: 0000000000000000    48 FUNC    GLOBAL DEFAULT    1 main
     8: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND func

## Symbol Table of `b.o`

 **Use the following command to view the symbol table of `b.o`:**
 ```bash
  readelf -s b.o
**Output**
```less
File: b.o

Symbol table '.symtab' contains 11 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS b.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 .data
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 .rodata
     5: 0000000000000008     4 OBJECT  LOCAL  DEFAULT    3 temp.0
     6: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    3 c
     7: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    4 d
     8: 0000000000000004     4 OBJECT  GLOBAL DEFAULT    3 f
     9: 0000000000000000   144 FUNC    GLOBAL DEFAULT    1 func
    10: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf





