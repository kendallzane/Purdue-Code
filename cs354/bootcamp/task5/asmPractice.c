// Compile with gcc -m32 asmPractice.c -o asmPractice.out
#include <stdio.h>
#include <stdlib.h>

void foo(const char* str1, const char* str2, int var1, int var2)
{
    printf("%s-%s-%d-%d\n", str1, str2, var1, var2);
}

int main()
{
    int a = 2;
    int b = 1;
    printf("a is %d, b is %d\n", a, b);
    // TODO (student): swap a and b using inline assembly
	asm(
	"xchg %1, %0" 
	:"=r" (a), "=r" (b)
	: "r" (b),  "r" (a)
	:);
	

    printf("a is %d, b is %d\n", a, b);

    const char * bar;
    const char * baz;
    bar = "bar";
    baz = "baz";
    // TODO (student): Call `foo` here using only inline assembly
	
	/*asm(
	"call foo %bar, %baz, %b, %a"
	:
	: [bar]"r" (bar), [baz]"r" (baz), [b]"r" (b), [a]"r" (a)
	:);
		
}
