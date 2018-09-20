#include <stdio.h>

void print_binary(int number)
{
    if (number) {
        print_binary(number >> 1);
        putc((number & 1) ? '1' : '0', stdout);
    }
	
}

int main()
{
	int c1[9] = {0x76, 0xe, 0x11, 0x34, 0x40, 0x13, 0x51, 0x17, 0x6};
	int c2[9] = {0x73, 0xe, 0x58, 0x8, 0x50, 0x13, 0x15, 0x37, 0x13};
	int c3[9] = {0x79, 0x0, 0x41, 0x14, 0x5c, 0xf, 0x50, 0x11, 0x10};
	
	int boiler1[9] = {0x42, 0x4f, 0x49, 0x4c, 0x45, 0x52, 0, 0, 0};
	int boiler2[9] = {0, 0x42, 0x4f, 0x49, 0x4c, 0x45, 0x52, 0, 0};
	int boiler3[9] = {0, 0, 0x42, 0x4f, 0x49, 0x4c, 0x45, 0x52, 0};
	int boiler4[9] = {0, 0, 0, 0x42, 0x4f, 0x49, 0x4c, 0x45, 0x52};
	
	int crib[9];
	
	for (int i = 0; i < 9; i++) {
		crib[i] = (c1[i] ^ c2[i]);
	}
	
	printf("pos 0: ");
	for (int i = 0; i < 9; i++) {
		printf("%c | ", (boiler1[i] ^ crib[i]));
	}
	printf("\n");
	
	printf("pos 1: ");
	for (int i = 0; i < 9; i++) {
		printf("%c | ", (boiler2[i] ^ crib[i]));
	}
	printf("\n");
	
	printf("pos 2: ");
	for (int i = 0; i < 9; i++) {
		printf("%c | ", (boiler3[i] ^ crib[i]));
	}
	printf("\n");
	
	printf("pos 3: ");
	for (int i = 0; i < 9; i++) {
		printf("%c | ", (boiler4[i] ^ crib[i]));
	}
	printf("\n");
	
	//first line is:
	//pos 0: G | O |  | p | U | R | D |   |  |
	
	int boilerup[9] = {0x42, 0x4f, 0x49, 0x4c, 0x45, 0x52, 0, 0x55, 0x50};
	int gopurdue[9];
	
	printf("guess: ");
	for (int i = 0; i < 9; i++) {
		printf("%c | ", (boilerup[i] ^ crib[i]));
		gopurdue[i] = (boilerup[i] ^ crib[i]);
		
	}
	printf("\n");
	
	
	int key1[9];
	int key2[9];
	
	printf("key1 = ");
	for (int i = 0; i < 9; i++) {
		printf("%c | ", (boilerup[i] ^ c1[i]));
		key1[i] = (boilerup[i] ^ c2[i]);
	}
	printf("\n");
	
	printf("key2 = ");
	for (int i = 0; i < 9; i++) {
		printf("%c | ", (gopurdue[i] ^ c2[i]));
		key2[i] = (boilerup[i] ^ c1[i]);
	}
	printf("\n");
	
	
	printf("m1 = ");
	for (int i = 0; i < 9; i++) {
		printf("%c | ", (c1[i] ^ key1[i]));
	}
	printf("\n");
	
	printf("m2 = ");
	for (int i = 0; i < 9; i++) {
		printf("%c | ", (c2[i] ^ key1[i]));
	}
	printf("\n");
	
	printf("m3 = ");
	for (int i = 0; i < 9; i++) {
		printf("%c | ", (c3[i] ^ key1[i]));
	}
	printf("\n");
	
}