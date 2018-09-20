#include <stdio.h>
void print_even(int limit)
{
	int num = 2;
	while (num <= limit) {
		printf("%i ", num);
		num += 2;
	}
}

void print_odd(int limit)
{
	int num = 1;
	while (num <= limit) {
		printf("%i ", num);
		num += 2;
	}
}


