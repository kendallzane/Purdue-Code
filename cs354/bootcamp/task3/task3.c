#include <stdio.h>
#include <stdlib.h>

struct simple_struct_1
{
  long long a;
  char b;
  long c;
};

struct simple_struct_2
{
  char d;
  struct simple_struct_2 *e;
  short f;
};

struct simple_struct
{
  long long ssn;
  char gender;
  struct inner_struct_1 {
    char *address;
    short age;
  }in;
};

int main()
{
	struct simple_struct_1 simple1;
	struct simple_struct_2 simple2;
	struct simple_struct simple3;
	
	printf("\n---Printing: simple_struct_1---\n\n");
	
	printf("Address of simple_struct_1.a: 		%p\n", &(simple1.a));
	printf("Address of simple_struct_1.b: 		%p\n", &(simple1.b));
	printf("Address of simple_struct_1.c: 		%p\n\n", &(simple1.c));
	
	printf("Size of simple_struct_1: 		%ld\n", sizeof(simple1));
	printf("Size of simple_struct_1.a: 		%ld\n", sizeof(simple1.a));
	printf("Size of simple_struct_1.b: 		%ld\n", sizeof(simple1.b));
	printf("Size of simple_struct_1.c: 		%ld\n\n", sizeof(simple1.c));
	
	int * pa = (int *) &(simple1.a);
	int * pb = (int *) &(simple1.b);
	int * pc = (int *) &(simple1.c);

	unsigned long int atob = (pb - pa) * sizeof(int) - sizeof(simple1.a);
	unsigned long int btoc = (pc - pb) * sizeof(int) - sizeof(simple1.b);
	
	
	printf("Padding between a and b: 		%ld\n", atob);
	printf("Padding between b and c: 		%ld\n", btoc);
	printf("Padding after c: 			%ld\n\n", sizeof(simple1) - sizeof(simple1.a) - sizeof(simple1.b) - sizeof(simple1.c) - atob - btoc);
	
	
	
	printf("---Printing: simple_struct_2---\n\n");
	
	printf("Address of simple_struct_2.d: 		%p\n", &(simple2.d));
	printf("Address of simple_struct_2.e: 		%p\n", &(simple2.e));
	printf("Address of simple_struct_2.f: 		%p\n\n", &(simple2.f));
	
	printf("Size of simple_struct_2: 		%ld\n", sizeof(simple2));
	printf("Size of simple_struct_2.d: 		%ld\n", sizeof(simple2.d));
	printf("Size of simple_struct_2.e: 		%ld\n", sizeof(simple2.e));
	printf("Size of simple_struct_2.f: 		%ld\n\n", sizeof(simple2.f));
	
	int * pd = (int *) &(simple2.d);
	int * pe = (int *) &(simple2.e);
	int * pf = (int *) &(simple2.f);

	unsigned long int dtoe = (pe - pd) * sizeof(int) - sizeof(simple2.d);
	unsigned long int etof = (pf - pe) * sizeof(int) - sizeof(simple2.e);
	
	
	printf("Padding between d and e: 		%ld\n", dtoe);
	printf("Padding between e and f: 		%ld\n", etof);
	printf("Padding after f: 			%ld\n\n", sizeof(simple2) - sizeof(simple2.d) - sizeof(simple2.e) - sizeof(simple2.f) - dtoe - etof);
	
	printf("---Printing: simple_struct---\n\n");
	
	printf("Address of simple_struct.ssn: 		%p\n", &(simple3.ssn));
	printf("Address of simple_struct.gender: 	%p\n", &(simple3.gender));
	printf("Address of simple_struct.in.address: 	%p\n", &(simple3.in.address));
	printf("Address of simple_struct.in.age: 	%p\n\n", &(simple3.in.age));
	
	printf("Size of simple_struct: 			%ld\n", sizeof(simple3));
	printf("Size of simple_struct.ssn: 		%ld\n", sizeof(simple3.ssn));
	printf("Size of simple_struct.gender: 		%ld\n", sizeof(simple3.gender));
	printf("Size of simple_struct.in.address: 	%ld\n", sizeof(simple3.in.address));
	printf("Size of simple_struct.in.age: 		%ld\n\n", sizeof(simple3.in.age));
	
	int * pSsn = (int *) &(simple3.ssn);
	int * pGender = (int *) &(simple3.gender);
	int * pAddress = (int *) &(simple3.in.address);
	int * pAge = (int *) &(simple3.in.age);

	unsigned long int ssnToGender = (pGender - pSsn) * sizeof(int) - sizeof(simple3.ssn);
	unsigned long int genderToAddress = (pAddress - pGender) * sizeof(int) - sizeof(simple3.gender);
	unsigned long int addressToAge = (pAge - pAddress) * sizeof(int) - sizeof(simple3.in.address);
	
	
	printf("Padding between ssn and gender: 	%ld\n", ssnToGender);
	printf("Padding between gender and address: 	%ld\n", genderToAddress);
	printf("Padding between address and age: 	%ld\n", addressToAge);
	
	printf("Padding after age: 			%ld\n", sizeof(simple3) - sizeof(simple3.ssn) - sizeof(simple3.gender)
	- sizeof(simple3.in.address) - sizeof(simple3.in.age) - ssnToGender - genderToAddress - addressToAge);
	
	
	
}

 
