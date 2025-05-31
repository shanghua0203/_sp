sun.c
-
1. 執行**sum.c**  
   ```
    gcc sum.c 
    ---> a.exe
   ```
2. 產生組合語言.s檔案  
   ```
    gcc -S sum.c  
    ---> sum.s
   ```
3. 產生目的檔案.o檔  
   ```
    gcc -c sum.c  
    ---> sum.o
   ```
4. 將目標檔案反編譯  
   ```
    objdump -d 0326sum.o
   ```
   輸出:

   ```
    sum.o:     file format pe-x86-64


    Disassembly of section .text:

    0000000000000000 <sum>:
    0:   55                      push   %rbp
    1:   48 89 e5                mov    %rsp,%rbp
    4:   48 83 ec 10             sub    $0x10,%rsp
    8:   89 4d 10                mov    %ecx,0x10(%rbp)
    b:   c7 45 fc 00 00 00 00    movl   $0x0,-0x4(%rbp)
    12:   c7 45 f8 00 00 00 00    movl   $0x0,-0x8(%rbp)
    19:   eb 0a                   jmp    25 <sum+0x25>
    1b:   8b 45 f8                mov    -0x8(%rbp),%eax
    1e:   01 45 fc                add    %eax,-0x4(%rbp)
    21:   83 45 f8 01             addl   $0x1,-0x8(%rbp)
    25:   8b 45 f8                mov    -0x8(%rbp),%eax
    28:   3b 45 10                cmp    0x10(%rbp),%eax
    2b:   7e ee                   jle    1b <sum+0x1b>
    2d:   8b 45 fc                mov    -0x4(%rbp),%eax
    30:   48 83 c4 10             add    $0x10,%rsp
    34:   5d                      pop    %rbp
    35:   c3                      retq

    0000000000000036 <main>:
    36:   55                      push   %rbp
    37:   48 89 e5                mov    %rsp,%rbp
    3a:   48 83 ec 30             sub    $0x30,%rsp
    3e:   e8 00 00 00 00          callq  43 <main+0xd>
    43:   b9 0a 00 00 00          mov    $0xa,%ecx
    48:   e8 b3 ff ff ff          callq  0 <sum>
    4d:   89 45 fc                mov    %eax,-0x4(%rbp)
    50:   8b 45 fc                mov    -0x4(%rbp),%eax
    53:   89 c2                   mov    %eax,%edx
    55:   48 8d 0d 00 00 00 00    lea    0x0(%rip),%rcx        # 5c <main+0x26>
    5c:   e8 00 00 00 00          callq  61 <main+0x2b>
    61:   b8 00 00 00 00          mov    $0x0,%eax
    66:   48 83 c4 30             add    $0x30,%rsp
    6a:   5d                      pop    %rbp
    6b:   c3                      retq
    6c:   90                      nop
    6d:   90                      nop
    6e:   90                      nop
    6f:   90                      nop
   ```
5. 將目標檔案的標頭印出  
   ```
    objdump -h sum.o
   ```
   輸出：
   ```
    sum.o:     file format pe-x86-64

    Sections:
    Idx Name          Size      VMA               LMA               File off  Algn
     0 .text         00000070  0000000000000000  0000000000000000  0000012c  2**4
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
    1 .data         00000000  0000000000000000  0000000000000000  00000000  2**4
                  ALLOC, LOAD, DATA
    2 .bss          00000000  0000000000000000  0000000000000000  00000000  2**4
                  ALLOC
    3 .xdata        00000018  0000000000000000  0000000000000000  0000019c  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
    4 .pdata        00000018  0000000000000000  0000000000000000  000001b4  2**2
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
    5 .rdata        00000010  0000000000000000  0000000000000000  000001cc  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
    6 .rdata$zzz    00000040  0000000000000000  0000000000000000  000001dc  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
   ```