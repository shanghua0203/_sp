// c4.c - C in four functions

// char, int, and pointer types
// if, while, return, and expression statements
// just enough features to allow self-compilation and a bit more

// Written by Robert Swierczek
// 修改者: 陳鍾誠 (模組化並加上中文註解)

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#define int int64_t // 64 bit 電腦

int *e;  // current position in emitted code (e: 目前機器碼指標, le: 上一行機器碼指標)
int debug = 1, src=0;

// opcodes (機器碼的 op)
enum { LLA ,IMM ,JMP ,JSR ,BZ  ,BNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PSH ,
       OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,
       OPEN,READ,CLOS,PRTF,MALC,FREE,MSET,MCMP,EXIT };

int run(int *pc, int *bp, int *sp) { // 虛擬機 => pc: 程式計數器, sp: 堆疊暫存器, bp: 框架暫存器
  int a, cycle; // a: 累積器, cycle: 執行指令數
  int i, *t;    // temps

  cycle = 0;
  while (1) {
    i = *pc++; ++cycle;
    if (debug) {
      printf("%d> %.4s", cycle,
        &"LLA ,IMM ,JMP ,JSR ,BZ  ,BNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PSH ,"
         "OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,"
         "OPEN,READ,CLOS,PRTF,MALC,FREE,MSET,MCMP,EXIT,"[i * 5]);
      if (i <= ADJ) printf(" %d\n", *pc); else printf("\n");
    }
    if      (i == LLA) a = (int)(bp + *pc++);                             // load local address 載入區域變數
    else if (i == IMM) a = *pc++;                                         // load global address or immediate 載入全域變數或立即值
    else if (i == JMP) pc = (int *)*pc;                                   // jump               躍躍指令
    else if (i == JSR) { *--sp = (int)(pc + 1); pc = (int *)*pc; }        // jump to subroutine 跳到副程式
    else if (i == BZ)  pc = a ? pc + 1 : (int *)*pc;                      // branch if zero     if (a==0) goto m[pc]
    else if (i == BNZ) pc = a ? (int *)*pc : pc + 1;                      // branch if not zero if (a!=0) goto m[pc]
    else if (i == ENT) { *--sp = (int)bp; bp = sp; sp = sp - *pc++; }     // enter subroutine   進入副程式
    else if (i == ADJ) sp = sp + *pc++;                                   // stack adjust       調整堆疊
    else if (i == LEV) { sp = bp; bp = (int *)*sp++; pc = (int *)*sp++; } // leave subroutine   離開副程式
    else if (i == LI)  a = *(int *)a;                                     // load int           載入整數
    else if (i == LC)  a = *(char *)a;                                    // load char          載入字元
    else if (i == SI)  *(int *)*sp++ = a;                                 // store int          儲存整數
    else if (i == SC)  a = *(char *)*sp++ = a;                            // store char         儲存字元
    else if (i == PSH) *--sp = a;                                         // push               推入堆疊

    else if (i == OR)  a = *sp++ |  a; // a = a OR *sp
    else if (i == XOR) a = *sp++ ^  a; // a = a XOR *sp
    else if (i == AND) a = *sp++ &  a; // ...
    else if (i == EQ)  a = *sp++ == a;
    else if (i == NE)  a = *sp++ != a;
    else if (i == LT)  a = *sp++ <  a;
    else if (i == GT)  a = *sp++ >  a;
    else if (i == LE)  a = *sp++ <= a;
    else if (i == GE)  a = *sp++ >= a;
    else if (i == SHL) a = *sp++ << a;
    else if (i == SHR) a = *sp++ >> a;
    else if (i == ADD) a = *sp++ +  a;
    else if (i == SUB) a = *sp++ -  a;
    else if (i == MUL) a = *sp++ *  a;
    else if (i == DIV) a = *sp++ /  a;
    else if (i == MOD) a = *sp++ %  a;

    else if (i == OPEN) a = open((char *)sp[1], *sp); // 開檔
    else if (i == READ) a = read(sp[2], (char *)sp[1], *sp); // 讀檔
    else if (i == CLOS) a = close(*sp); // 關檔
    else if (i == PRTF) { t = sp + pc[1]; a = printf((char *)t[-1], t[-2], t[-3], t[-4], t[-5], t[-6]); } // printf("....", a, b, c, d, e)
    else if (i == MALC) a = (int)malloc(*sp); // 分配記憶體
    else if (i == FREE) free((void *)*sp); // 釋放記憶體
    else if (i == MSET) a = (int)memset((char *)sp[2], sp[1], *sp); // 設定記憶體
    else if (i == MCMP) a = memcmp((char *)sp[2], (char *)sp[1], *sp); // 比較記憶體
    else if (i == EXIT) { printf("exit(%d) cycle = %d\n", *sp, cycle); return *sp; } // EXIT 離開
    else { printf("unknown instruction = %d! cycle = %d\n", i, cycle); return -1; } // 錯誤處理
  }
}

// tokens and classes (operators last and in precedence order) (按優先權順序排列)
enum { // token : 0-127 直接用該字母表達， 128 以後用代號。
  Num = 128, Fun, Sys, Glo, Loc, Id,
  Char, Else, Enum, If, Int, Return, Sizeof, While,
  Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};

// types (支援型態，只有 int, char, pointer)
enum { CHAR, INT, PTR };

#define POOL_SIZE 256*1024

char source[POOL_SIZE];
int code[POOL_SIZE], data[POOL_SIZE], stack[POOL_SIZE];
int *datap = data;

char *p, *lp; // current position in source code (p: 目前原始碼指標, lp: 上一行原始碼指標)

int *e, *le,  // current position in emitted code (e: 目前機器碼指標, le: 上一行機器碼指標)
    loc,      // local variable offset (區域變數的位移)
    line,     // current line number (目前行號)
    src,      // print source and assembly flag (印出原始碼)
    debug;    // print executed instructions (印出執行指令 -- 除錯模式)

typedef struct token_t {
   int tk, class, type, val;
   char *name;
} token_t;

token_t sym[POOL_SIZE];   // currently parsed identifier (id: 目前的 id)

void sym_dump(token_t *sym) {
  token_t *id = sym;
  while (id->name) {
    printf("sym[%d]: %8.4s tk=%d class=%d type=%d val=%d\n", id-sym, id->name, id->tk, id->class, id->type, id->val);
    id++;
  }
}

token_t next() // 詞彙解析 lexer
{
  token_t t = {0}; // 初始化 token

  while (t.tk = *p) {
    t.name = p;
    ++p;
    if (t.tk == '\n') { // 換行
      if (src) {
        printf("%d: %.*s", line, p - lp, lp); // 印出該行
        lp = p; // lp = p = 新一行的原始碼開頭
        while (le < e) { // 印出上一行的所有目的碼
          printf("%8.4s", &"LLA ,IMM ,JMP ,JSR ,BZ  ,BNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PSH ,"
                           "OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,"
                           "OPEN,READ,CLOS,PRTF,MALC,FREE,MSET,MCMP,EXIT,"[*++le * 5]);
          if (*le <= ADJ) printf(" %d\n", *++le); else printf("\n"); // LLA ,IMM ,JMP ,JSR ,BZ  ,BNZ ,ENT ,ADJ 有一個參數。
        }
      }
      ++line;
    }
    else if (t.tk == '#') { // 取得 #include <stdio.h> 這類的一整行
      while (*p != 0 && *p != '\n') ++p;
    }
    else if ((t.tk >= 'a' && t.tk <= 'z') || (t.tk >= 'A' && t.tk <= 'Z') || t.tk == '_') { // 取得變數名稱
      t.name = p-1; // 變數名稱
      while ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_')
        p++;
      token_t *id = sym;
      while (id->name) { // 檢查符號是否存在
        if (!memcmp(id->name, t.name, p - t.name)) { t.tk = id->tk; return *id; } // 有找到的話就傳回
        id++; // 找下一個符號
      }
      t.tk = Id; // token = id->Tk = Id
      *id = t;
      return t;
    }
    else if (t.tk >= '0' && t.tk <= '9') { // 取得數字串
      int ival;
      if (ival = t.tk - '0') { while (*p >= '0' && *p <= '9') ival = ival * 10 + *p++ - '0'; } // 十進位
      else if (*p == 'x' || *p == 'X') { // 十六進位
        while ((t.tk = *++p) && ((t.tk >= '0' && t.tk <= '9') || (t.tk >= 'a' && t.tk <= 'f') || (t.tk >= 'A' && t.tk <= 'F'))) // 16 進位
          ival = ival * 16 + (t.tk & 15) + (t.tk >= 'A' ? 9 : 0);
      }
      else { while (*p >= '0' && *p <= '7') ival = ival * 8 + *p++ - '0'; } // 八進位
      t.tk = Num; // token = Number
      t.val = ival;
      return t;
    }
    else if (t.tk == '/') {
      if (*p == '/') { // 註解
        ++p;
        while (*p != 0 && *p != '\n') ++p; // 略過註解
      }
      else { // 除法
        t.tk = Div;
        return t;
      }
    }
    else if (t.tk == '\'' || t.tk == '"') { // 字元或字串
      char *pp = datap;
      while (*p != 0 && *p != t.tk) {
        int ival;
        if ((ival = *p++) == '\\') {
          if ((ival = *p++) == 'n') ival = '\n'; // 處理 \n 的特殊情況
        }
        if (t.tk == '"') *datap++ = ival; // 把字串塞到資料段裏
        t.val = ival;
      }
      ++p;
      if (t.tk == '"') t.val = (int)pp; else t.tk = Num; // (若是字串) ? (ival = 字串 (在資料段中的) 指標) : (字元值)
      return t;
    } // 以下為運算元 =+-!<>|&^%*[?~, ++, --, !=, <=, >=, ||, &&, ~  ;{}()],:
    else if (t.tk == '=') { if (*p == '=') { ++p; t.tk = Eq; } else t.tk = Assign; return t; }
    else if (t.tk == '+') { if (*p == '+') { ++p; t.tk = Inc; } else t.tk = Add; return t; }
    else if (t.tk == '-') { if (*p == '-') { ++p; t.tk = Dec; } else t.tk = Sub; return t; }
    else if (t.tk == '!') { if (*p == '=') { ++p; t.tk = Ne; } return t; }
    else if (t.tk == '<') { if (*p == '=') { ++p; t.tk = Le; } else if (*p == '<') { ++p; t.tk = Shl; } else t.tk = Lt; return t; }
    else if (t.tk == '>') { if (*p == '=') { ++p; t.tk = Ge; } else if (*p == '>') { ++p; t.tk = Shr; } else t.tk = Gt; return t; }
    else if (t.tk == '|') { if (*p == '|') { ++p; t.tk = Lor; } else t.tk = Or; return t; }
    else if (t.tk == '&') { if (*p == '&') { ++p; t.tk = Lan; } else t.tk = And; return t; }
    else if (t.tk == '^') { t.tk = Xor; return t; }
    else if (t.tk == '%') { t.tk = Mod; return t; }
    else if (t.tk == '*') { t.tk = Mul; return t; }
    else if (t.tk == '[') { t.tk = Brak; return t; }
    else if (t.tk == '?') { t.tk = Cond; return t; }
    else if (t.tk == '~' || t.tk == ';' || t.tk == '{' || t.tk == '}' || t.tk == '(' || t.tk == ')' || t.tk == ']' || t.tk == ',' || t.tk == ':') return t;
  }
  return t;
}

int lex() {
  while (1) {
    token_t t = next();
    if (t.tk == 0) break;
    printf("p=%p tk=%d sym=%8.4s\n", p, t.tk, t.name);
  }
}

/*
int compile() {

}
*/

int main(int32_t argc, char *argv[]) // 主程式
{
  --argc; ++argv;
  if (argc > 0 && **argv == '-' && (*argv)[1] == 's') { src = 1; --argc; ++argv; }
  if (argc > 0 && **argv == '-' && (*argv)[1] == 'd') { debug = 1; --argc; ++argv; }
  if (argc < 1) { printf("usage: c4 [-s] [-d] file ...\n"); return -1; }

  memset(source, 0, POOL_SIZE);
  memset(code, 0, POOL_SIZE);
  memset(data, 0, POOL_SIZE);
  memset(stack, 0, POOL_SIZE);
  memset(sym, 0, POOL_SIZE*sizeof(token_t));

  int *pc, *bp, *sp;
  pc = e = le = code;
  p = lp = source;
  int fd = open(*argv, 0);
  if (fd < 0) { printf("could not open(%s)\n", *argv); return -1; }
  int len = read(fd, source, POOL_SIZE-1);
  if (len <= 0) { printf("read() returned %d\n", len); return -1; }
  lex();
  sym_dump(sym);
/*
  pc = e;
// 1: int main()
// 2: {
// 3:   printf("hello, world\n");
  *e++ = ENT; *e++ = 0;
  *e++ = IMM; *e++ = (int) "hello, world\n";
  *e++ = PSH;
  *e++ = PRTF;
  *e++ = ADJ; *e++ = 1;
// 4:   exit();
  *e++ = EXIT;
// 5: }
*/
  // bp = sp = (int *)((int)stack + POOL_SIZE); // setup stack
  // return run(pc, bp, sp);
}
