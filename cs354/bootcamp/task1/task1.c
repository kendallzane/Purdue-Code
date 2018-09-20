#include <stdio.h>
void foo();
void bar();

int main()
{
  int y = 5;

  if (y < 2) { 
    printf("TEST PASSED\n");
  }

  for (int x=0; x<5; x++) {
    printf("HELLO WORLD!\n");
  }
  
   
  foo();
  return 0;
}

void foo()
{
  printf("THIS IS FOO\n");
}

void bar()
{
  printf("THIS IS BAR\n");
}
