#include<stdio.h>
int power(int n){
    int r,i;
    r=1;
    i=0;
    while(i<n){
        r=r*2;
        i=i+1;
    }
    return r;
}
int main(){
    printf("power(%d)=%d\n",3,power(3));
    return 0;
}