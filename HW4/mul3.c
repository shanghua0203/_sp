#include <stdio.h>

// 宣告 mul3 是外部函數
int mul3(int a, int b, int c);

int main()
{
    printf("mul3(3,2,5) = %d\n", mul3(3, 2, 5));
    return 0;
}
