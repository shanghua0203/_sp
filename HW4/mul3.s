    .globl mul3         # 宣告全域符號
    .text

mul3:
    pushl %ebp          # 儲存 base pointer
    movl %esp, %ebp     # 建立新的 stack frame

    movl 8(%ebp), %eax      # a 傳入到 %eax
    imull 12(%ebp), %eax    # eax = eax * b
    imull 16(%ebp), %eax    # eax = eax * c

    popl %ebp           # 還原 stack frame
    ret                 # 回傳結果存在 %eax 中
