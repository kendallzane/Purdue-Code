// compile with gcc -g debuggingPractice.c -o debuggingPractice.out
#include<stdio.h>
#include<stdlib.h>

void foo()
{
    printf("In foo\n");
}

void bar()
{
    printf("In bar\n");
    exit(0);
}

int main()
{
    int i = 0;
    while(i < 20) {
        printf("i is %#X\n", i);
        i++;
    }
    foo();
}
