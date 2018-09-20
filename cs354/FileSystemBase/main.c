#include "simpleFS.h"

int main(int argc, char **argv)
{
	
	/*
	unsigned char *block_bitmap = malloc(sizeof(unsigned char) * BLOCK_SIZE);

	print_bits(block_bitmap, 512);
	
	int i = 0;
	for (i = 0; i < 24; i++) {
		set_bit(block_bitmap, i, 1);
	}
	for (i = 0; i < 24; i+=3) {
		set_bit(block_bitmap, i, 0);
	}
	
	print_bits(block_bitmap, 512);
	
	for (i = 0; i < 24; i++) {
		check_bit(block_bitmap, i);
	}
	
	free(block_bitmap);
	struct directory_entry test;
	printf("sizeof(directory_entry) == %d\n", sizeof(test));
	*/
	/*
	init_filesystem(8, "/homes/bowlesk/cs354/FileSystemBase/block1", 42);
	open_filesystem("/homes/bowlesk/cs354/FileSystemBase/block1", 42);
	make_directory("/asuka/", 7);
	make_directory("/asuka/rei/", 11);
	make_directory("/asuka/rei/misato/", 18);
	make_directory("/shinji/", 8);
	make_directory("/shinji/gendo/", 14);
	make_directory("/shinji/eva01/", 14);
	*/
	
	
	char *storage = malloc(sizeof(unsigned char) * BLOCK_SIZE);
	memset(storage, 0, BLOCK_SIZE);
	char *storage2 = malloc(sizeof(unsigned char) * BLOCK_SIZE * 3);
	memset(storage2, 0, BLOCK_SIZE * 3);
	char *storage3 = malloc(sizeof(unsigned char) * BLOCK_SIZE * 3);
	memset(storage3, 0, BLOCK_SIZE * 3);
	
	
	char message[] = "IT ALL RETURNS TO NOTHING";
	memcpy(storage, message, (int)strlen(message));
	char message2[] = "The FitnessGram™ Pacer Test is a multistage aerobic capacity test that progressively gets more difficult as it continues. The 20 meter pacer test will begin in 30 seconds. Line up at the start. The running speed starts slowly, but gets faster each minute after you hear this signal. [beep] A single lap should be completed each time you hear this sound. [ding] Remember to run in a straight line, and run as long as possible. The second time you fail to complete a lap before the sound, your test is over. The test will begin on the word start. On your mark, get ready, start.";
	memcpy(storage2, message2, (int)strlen(message2));
	
	init_filesystem(8, "/homes/bowlesk/cs354/FileSystemBase/block1", 42);
	open_filesystem("/homes/bowlesk/cs354/FileSystemBase/block1", 42);
	make_directory("/asuka/", 7);
	make_directory("/asuka/rei", 10);
	read_directory("/asuka/rei", 11, storage3);
	//rm_directory("/asuka/rei", 10);
	//read_directory("/asuka/rei/", 11, storage3);
	create_file("/asuka/rei/gendo", 16, (BLOCK_SIZE * 2) + 100, storage2);
	make_link("/asuka/eva", 10, "/asuka/rei/gendo");
    //rm_file("/asuka/rei/gendo", 16);
	rm_file("/asuka/eva", 10);
	rm_file("/asuka/rei/gendo", 16);
	//read_file("/asuka/rei/gendo", 16, storage3);
	//print_bits(storage3, (BLOCK_SIZE * 3));
	/*
	create_file("/eva", 4, BLOCK_SIZE, storage);
	create_file("/asuka/rei/gendo", 16, (BLOCK_SIZE * 2) + 100, storage2);
	//rm_file("/asuka/rei/gendo", 16);
	read_file("/asuka/rei/gendo", 16, storage3);
	*/
	//init_filesystem(3, "/homes/bowlesk/cs354/FileSystemBase/block2", 42);
	//printf("time: %d\n", time(0));
}

