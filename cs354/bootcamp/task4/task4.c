#include <stdio.h>
#include <stdlib.h>
#include "print_seq.h"

void print(int input) {
	if (input % 2) {
		print_odd(input);
	} else {
		print_even(input);
	}
	
}

int main(int argc, char **argv)
{
	int number;
	if (argc != 2) {
		printf("usage: enter a single number\n");
		exit(0);
	} else {
		number = atoi(argv[1]);
		//printf("number: %i\n", number);
	}
	
	void (*fp)(int) = &print;
	
	(*fp)(number);
	
}


  
