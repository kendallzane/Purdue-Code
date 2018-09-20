// Initialize a filesystem of the size specified in number of data blocks
#include "simpleFS.h"

/** Initialize the filesystem to the designated 
 */
void init_filesystem(unsigned int size, char *real_path, unsigned int n)
{
    /*
     * You first need to initalize a superblock. Write that to disk.
     * Create the bitmaps for block_bm and inode_bm.
     * write both to disk
     * Make a 5 block large inode table.
     * Then initialize the "size" datablocks.
     *
     * Also, when this is all done. you should have a file system ready.
     * with a parent directory of '/' at inode 2.
     */
	 
	 

	
	//check if the file in the path exists. it shouldn't, so
	//exit with error if it doesn
	FILE* fake;
	char path[n];
	strncpy(path, real_path, n);
	path[n] = '\0';
	
	//printf("looking for path  - %s\n", path);
	
	fake = fopen(path, "r");
	if (fake) {
		printf("ERROR: File in path seems to already exist. Please specify an unused file path.\n");
		exit(1);
	}
	
	//this function specifies how many blocks are wanted
	//so, we'll have to make sure the number of wanted blocks
	//will make sense
	
	if (size <= 1) {
		printf("ERROR: You must have at least two blocks for the filesystem. One is reserved for the root directory.\n");
		exit(1);
	}
	
	//each inode can point to 8 blocks of data
	//and there are 37 free inodes to start with
	//so, the maximum number of blocks is
	//8*38 = 304 blocks
	
	if (size > 304) {
		printf("ERROR: The maximum amount of blocks the file system can have for data is 304.\n");
		exit(1);
	}
	
	//now, we can actually create the file
	fake = fopen(path, "w+");

		//initializing the superblock
	struct superblock super;
	
	//inode table is 5 blocks
	//inodes are 64 bytes big
	//blocks are 512 bytes big
	//so, 8 inodes can fit in each block
	//so, our inode table is 40 inodes long.
	super.s_inodes_count = 40;

	//the user specifies the number of data blocks, from 1 to 320
	super.s_blocks_count = size;
	
	//our file system starts with 2 inodes
	//which are two NIL files
	//so, 40-2 = 38 free inodes
	
	super.s_free_inodes_count = 38;
	//all of the blocks initiated are free at the start
	//except for the first one, which is /
	super.s_free_blocks_count = size - 1;
	//first data block will be 8 to start with
	//because we skip 1 for the superblock
	//skip 1 for the block bitmap
	//skip 1 for the inode bitmap
	//and skip 5 for the inode table
	super.s_first_data_block = 9;
	//our first free inode is the one at index 3
	//because we already have 2 reserved
	//and we will have a parent directory as well
	super.s_first_ino = 3;
	//magic
	super.s_magic = 0x554e4958;
	
	//now, we write that superblock to our file
	
	//printf("size of superblock = %d\n", (int)sizeof(struct superblock));
	
	
	unsigned char blank_block[BLOCK_SIZE] = {0};
	unsigned char sb_block[BLOCK_SIZE] = {0};
	
	memcpy(sb_block, &super, sizeof(struct superblock));
	fwrite(sb_block, BLOCK_SIZE, 1, fake);
	
	//printf("finished printing superblock\n");
	
	/*
	printf("\nwriting out superblock\n");
	print_bits(&super, 512);
	
	int write;
	write = fwrite(&super, BLOCK_SIZE, 1, fake);
	
	printf("\nwriting out beginning of file\n");
	unsigned char hell[BLOCK_SIZE];
	rewind(fake);
	fread(hell, BLOCK_SIZE, 1, fake);
	print_bits(hell, BLOCK_SIZE);
	*/
	//next is to create and write the block bitmap
	//we will start with 1 block
	
	
	
	unsigned char block_bitmap[BLOCK_SIZE] = {0};
	set_bit(block_bitmap, 0, 1);
	
	fwrite(&block_bitmap, BLOCK_SIZE, 1, fake);
	
	//next is to create and write the inode bitmap
	//the first 3 inodes are reserved/used, so we need to set them to 1
	unsigned char inode_bitmap[BLOCK_SIZE] = {0};
	set_bit(inode_bitmap, 0, 1);
	set_bit(inode_bitmap, 1, 1);
	set_bit(inode_bitmap, 2, 1);
	//print_bits(inode_bitmap, BLOCK_SIZE);
	
	fwrite(&inode_bitmap, BLOCK_SIZE, 1, fake);
	
	//next is to create and write the inode table
	//each directory entry is 64 bytes
	//so, each block can hold 512/64 = 8 entries
	
	//the first two inodes are reserved and nil
	
	unsigned char inode_block[BLOCK_SIZE] = {0};
	
	struct inode placeholder;
	
	memcpy(inode_block, &placeholder, sizeof(struct inode));
	memcpy(inode_block + sizeof(struct inode), &placeholder, sizeof(struct inode));
	
	//now we have to create the inode for the parent directory
	
	struct inode top;
	
	top.i_mode = top.i_mode | S_IFREG;
	top.i_mode = top.i_mode | S_IRWXU;
	top.i_mode = top.i_mode | S_IRWXG;
	top.i_mode = top.i_mode | S_IRWXO;
	
	top.i_uid = getuid();
	top.i_gid = getgid();
	
	top.i_links_count = 1;
	
	top.i_size = 2 * sizeof(struct directory_entry);
	
	top.i_time = time(0);
	top.i_ctime = top.i_time;
	top.i_mtime = top.i_time;
	
	top.i_blocks = 1;
	
	top.i_block[0] = 8;
	
	//now we write the top directory inode
	
	memcpy(inode_block + (2 * sizeof(struct inode)), &top, sizeof(struct inode));
	memcpy(inode_block + (3 * sizeof(struct inode)), &placeholder, sizeof(struct inode));
	memcpy(inode_block + (4 * sizeof(struct inode)), &placeholder, sizeof(struct inode));
	memcpy(inode_block + (5 * sizeof(struct inode)), &placeholder, sizeof(struct inode));
	memcpy(inode_block + (6 * sizeof(struct inode)), &placeholder, sizeof(struct inode));
	memcpy(inode_block + (7 * sizeof(struct inode)), &placeholder, sizeof(struct inode));
	print_bits(inode_block, BLOCK_SIZE);
	fwrite(inode_block, BLOCK_SIZE, 1, fake);
	
	memcpy(inode_block + (2 * sizeof(struct inode)), &placeholder, sizeof(struct inode));
	
	//now we write in 37 blank inodes
	//we'll just use the original placeholder
	
	int i = 0;
	for (i = 0; i < 4; i++) {
		fwrite(inode_block, BLOCK_SIZE, 1, fake);
	}
	//now we write in our first block, which is the original directory
	
	unsigned char mem_block[BLOCK_SIZE] = {0};
	
	struct directory_entry same;
	
	same.d_inode = 2;
	same.d_file_type = 2;
	same.d_name_len = 1;
	memset(same.d_name, 0, 57);
	same.d_name[0] = '.';
	
	struct directory_entry parent;
	
	parent.d_inode = 2;
	parent.d_file_type = 2;
	parent.d_name_len = 2;
	memset(parent.d_name, 0, 57);
	parent.d_name[0] = '.';
	parent.d_name[1] = '.';
	
	//for now, just those two directory entries are in this directory
	//so we can write those in
	//and then we can write in the 6 free remaining directory entries for this directory
	
	struct directory_entry p;
	memset(p.d_name, 0, 57);
	
	memcpy(mem_block + (0 * sizeof(struct directory_entry)), &same, sizeof(struct directory_entry));
	memcpy(mem_block + (1 * sizeof(struct directory_entry)), &parent, sizeof(struct directory_entry));
	memcpy(mem_block + (2 * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
	memcpy(mem_block + (3 * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
	memcpy(mem_block + (4 * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
	memcpy(mem_block + (5 * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
	memcpy(mem_block + (6 * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
	memcpy(mem_block + (7 * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
	
	
	fwrite(mem_block, BLOCK_SIZE, 1, fake);

	
	//then we can finally write in the remaining blank data blocks
	//just using the placeholder block of 0s
	
	for (int i = 0; i < size - 1; i++) {
		fwrite(&blank_block, BLOCK_SIZE, 1, fake);
	}
	
	//print_bits(fake, 1024);
	fp = fake;
	//print_bits(fp, BLOCK_SIZE*3);
	sb = super;
	
	
	
	//print_bits(hell, BLOCK_SIZE*(8+size));
	
	//exit(1);
	sync_bitmaps();
	
}

/** Open a filesystem at the designated path.
 */
void open_filesystem(char *real_path, unsigned int n)
{

 
 
 
     /*
     * You will need to open an existing file system image from disk.
     * Read the file system information from the disk and initialize the
     * in-memory variables to match what was on disk.
     * use this file system for everything from now on.
     * Fail if the magic signature doesn't match
     */
	 
	
	FILE* fake;
	char path[n];
	strncpy(path, real_path, n);
	path[n] = '\0';
	
	//printf("looking for path  - %s\n", path);
	
	fake = fopen(path, "r+");
	
	if (!fake) {
		//printf("ERROR: Cannot find the filesytem designated by the path.\n");
		errno = EFAULT;
		return;
	}
	
	//get the superblock from the filesystem, and check signature
	
	struct superblock *super = malloc(sizeof(struct superblock));
	//struct superblock *super;
	unsigned char sb_block[BLOCK_SIZE] = {0};
	fread(sb_block, BLOCK_SIZE, 1, fake);
	
	memcpy(super, sb_block, sizeof(struct superblock));
	
	print_bits(&super, sizeof(struct superblock));
	if (super->s_magic != 0x554e4958) {
		//printf("ERROR: Magic signature does not match.\n");
		errno = EBADF;
		return;
	} else {
		//printf("Magic signature match!\n");
	}
	//free(super);
	fp = fake;
	sb = *super;
	free(super);
	sync_bitmaps();
	
}

/** Make a directory with the given path.
 */
void make_directory(char *path, unsigned int n)
{
    /*
     * For the file system image that is currently opened.
     * Make a new directory at the path provided.
     * Make sure that the path is valid.
     */
	
	if (sb.s_free_blocks_count == 0) {
		printf("ERROR: no more free blocks to allocate a directory\n");
		exit(1);
	}
	
	if (sb.s_free_inodes_count == 0) {
		printf("ERROR: no more free inodes with which to allocate a directory\n");
		exit(1);
	}
	
	unsigned char free_block[BLOCK_SIZE] = {0};
	
	char * found = (char*) malloc(n*sizeof(char));
	char * to_slice = (char*) malloc(n*sizeof(char));
	char * to_slice2 = (char*) malloc(n*sizeof(char));
	
	char new_directory[n];
	strncpy(new_directory, path, n);
	new_directory[n] = '\0';
	
	//printf("attempting to make a directory at path - %s\n", new_directory);
	
	if (n <= 1) {
		printf("ERROR: provided path is too short\n");
		exit(1);
	}
	if (new_directory[0] != '/') {
		printf("ERROR: directory path should start with /\n");
		exit(1);
	}
	/*
	if (new_directory[n-1] != '/') {
		printf("ERROR: directory path should end with /\n");
		exit(1);
	}
	*/
	//printf("checks complete\n");

	
	//printf("about to slice\n");
	strncpy(to_slice, new_directory + 1, n - 1);
	strncpy(to_slice2, new_directory + 1, n - 2);
	
	to_slice2[n-2] = '\0';
	to_slice2[n-2] = '\0';
	//printf("string is %s\n", to_slice2);
	int search_node = 2;
	
	
	
	int num_subs = 0;
	
	while((found = strsep(&to_slice2, "/")) != NULL) {
		if((int)strlen(found) > 57) {
			printf("ERROR: folder names can not be longer than 57 characters\n");
			exit(1);
		}
		num_subs++;
	}
	
	//printf("depth of new directory: %d\n", num_subs);
	
	int i = 0; int j = 0; int k = 0;
	int current_sub = 1;
	int block = 0;
	

	int depth = 1;
	
	while((found = strsep(&to_slice, "/")) != NULL) {
		

		
		//printf("looking for directory: %s\n", found);
		//printf("directory length: %d\n", (int)strlen(found));
		

		
		struct inode *searching = malloc(sizeof(struct inode));
		//fseek(fp, BLOCK_SIZE * 3 + ((search_node * sizeof(struct inode)) , SEEK_SET);
		fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
		//printf("looking for inode in block %d\n", 3 + search_node / 8);
		//fread(searching, sizeof(struct inode), 1, fp);
		fread(free_block, BLOCK_SIZE, 1, fp);
		print_bits(free_block, BLOCK_SIZE);
		
		memcpy(searching, free_block + (sizeof(struct inode) * (search_node % 8)), sizeof(struct inode));
		
		print_bits(searching, sizeof(struct inode));
		
		block = searching->i_block[0];
		//printf("looking through block %d\n", block);
		if (current_sub ==  num_subs) {
			//printf("reached the end of the path. attempting to make the directory here.\n");
			
			if (strcmp(found, "") == 0) {
				printf("ERROR: file without name found in path\n");
				exit(1);
			}
			if (found[0] == '.') {
				printf("ERROR: folder with name starting with '.' found in path\n");
				exit(1);
			}
			
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			for (i = 0; i < 8; i++) {
				//fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				//fread(possible, sizeof(struct directory_entry), 1, fp);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				if (possible->d_name_len == 0) {
					//printf("found an unused directory_entry - the %dth one\n", i);
					//printf("making a new inode for this directory.\n");
					
					searching->i_size += sizeof(struct directory_entry);
					searching->i_mtime = time(0);
					searching->i_time = searching->i_mtime;
					//fseek(fp, BLOCK_SIZE * 3 + (search_node * sizeof(struct inode)) , SEEK_SET);
					fseek(fp, BLOCK_SIZE * 3 + ((search_node / 8) * BLOCK_SIZE) , SEEK_SET);
					fread(free_block, BLOCK_SIZE, 1, fp);
					print_bits(free_block, BLOCK_SIZE);
					memcpy(free_block + (sizeof(struct inode) * (search_node % 8)), searching, sizeof(struct inode));
					print_bits(free_block, BLOCK_SIZE);
					fseek(fp, BLOCK_SIZE * 3 + ((search_node / 8) * BLOCK_SIZE) , SEEK_SET);
					fwrite(free_block, BLOCK_SIZE, 1, fp);
					
					struct inode new_inode;
					new_inode.i_mode = new_inode.i_mode | S_IFDIR;
					new_inode.i_mode = new_inode.i_mode | S_IRWXU;
					new_inode.i_mode = new_inode.i_mode | S_IRWXG;
					new_inode.i_mode = new_inode.i_mode | S_IRWXO;
	
					new_inode.i_uid = getuid();
					new_inode.i_gid = getgid();
					
					new_inode.i_links_count = 1;
	
					new_inode.i_size = 2 * sizeof(struct directory_entry);
	
					new_inode.i_time = time(0);
					new_inode.i_ctime = new_inode.i_time;
					new_inode.i_mtime = new_inode.i_time;
	
					new_inode.i_blocks = 1;
					
					sb.s_free_inodes_count -= 1;
					
					//printf("looking for free inode in bitmap\n");
					
					unsigned char *inode_bitmap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
					fseek(fp, (BLOCK_SIZE * 2), SEEK_SET);
					fread(inode_bitmap, BLOCK_SIZE, 1, fp);
					
					for (j = 0; j < sb.s_inodes_count; j++) {
						if (check_bit(inode_bitmap, j) == 0) {
							//printf("inode %d is free\n", j);
							//printf("looking for free block in bitmap\n");
							unsigned char *block_bitmap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
							fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
							fread(block_bitmap, BLOCK_SIZE, 1, fp);
							for (k = 0; k < sb.s_blocks_count; k++) {
								if (check_bit(block_bitmap, k) == 0) {
									//printf("block %d is free\n", k);
									new_inode.i_block[0] = 8 + k;
									//printf("now writing inode to disk\n");
									
									print_bits(&new_inode, sizeof(struct inode));
									
									//fseek(fp, (BLOCK_SIZE * 3) + (j * sizeof(struct inode)), SEEK_SET);
									fseek(fp, (BLOCK_SIZE * 3) + ((j / 8) * BLOCK_SIZE), SEEK_SET);
									fread(free_block, BLOCK_SIZE, 1, fp);
									print_bits(free_block, BLOCK_SIZE);
									memcpy(free_block + (sizeof(struct inode) * (j % 8)), &new_inode, sizeof(struct inode));
									print_bits(free_block, BLOCK_SIZE);
									fseek(fp, (BLOCK_SIZE * 3) + ((j / 8) * BLOCK_SIZE), SEEK_SET);
									fwrite(free_block, BLOCK_SIZE, 1, fp);
									//printf("now creating directory at free block\n");
									sb.s_free_blocks_count -= 1;
									struct directory_entry same;
	
									same.d_inode = j;
									same.d_file_type = 2;
									same.d_name_len = 1;
									memset(same.d_name, 0, 57);
									same.d_name[0] = '.';
	
									struct directory_entry parent;
	
									parent.d_inode = search_node;
									parent.d_file_type = 2;
									parent.d_name_len = 2;
									memset(parent.d_name, 0, 57);
									parent.d_name[0] = '.';
									parent.d_name[1] = '.';
									
									
									/*
									printf("same\n");
									print_bits(&same, sizeof(struct directory_entry));
									printf("parent\n");
									print_bits(&parent, sizeof(struct directory_entry));
									*/
									//fwrite(&same, sizeof(struct directory_entry), 1, fp);
									//fwrite(&parent, sizeof(struct directory_entry), 1, fp);
									
									struct directory_entry p;
									p.d_inode = 0;
									p.d_file_type = 0;
									p.d_name_len = 0;
									memset(p.d_name, 0, 57);
									memset(free_block, 0, BLOCK_SIZE);
									memcpy(free_block + (0 * sizeof(struct directory_entry)), &same, sizeof(struct directory_entry));
									memcpy(free_block + (1 * sizeof(struct directory_entry)), &parent, sizeof(struct directory_entry));
									memcpy(free_block + (2 * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
									memcpy(free_block + (3 * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
									memcpy(free_block + (4 * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
									memcpy(free_block + (5 * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
									memcpy(free_block + (6 * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
									memcpy(free_block + (7 * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
									
									fseek(fp, (BLOCK_SIZE * 8) + (BLOCK_SIZE * k), SEEK_SET);
									print_bits(free_block, BLOCK_SIZE);
									fwrite(free_block, BLOCK_SIZE, 1, fp);
									
									//printf("now creating directory entry for this directory in parent\n");
									possible->d_inode = j;
									possible->d_file_type = 2;
									possible->d_name_len = strlen(found);
									//printf("saving directory name length: %d\n", possible->d_name_len);
									strncpy(possible->d_name, found, strlen(found));
									//printf("saving directory name: %s\n", possible->d_name);
									
									//fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)), SEEK_SET);
									fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
									fread(free_block, BLOCK_SIZE, 1, fp);
									print_bits(free_block, BLOCK_SIZE);
									memcpy(free_block + (i * sizeof(struct directory_entry)), possible, sizeof(struct directory_entry));
									//fwrite(possible, sizeof(struct directory_entry), 1, fp);
									fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
									fwrite(free_block, BLOCK_SIZE, 1, fp);
									
									print_bits(possible, sizeof(struct directory_entry));
									print_bits(free_block, BLOCK_SIZE);
									
									//printf("now writing the inode bitmap\n");
									set_bit(inode_bitmap, j, 1);
									
									//print_bits(inode_bitmap, BLOCK_SIZE);
									
									fseek(fp, (BLOCK_SIZE * 2), SEEK_SET);
									fwrite(inode_bitmap, BLOCK_SIZE, 1, fp);
									
									//printf("now writing the block bitmap\n");
									set_bit(block_bitmap, k, 1);
									
									//print_bits(block_bitmap, BLOCK_SIZE);
									
									fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
									fwrite(block_bitmap, BLOCK_SIZE, 1, fp);
									
									//printf("writing superblock to memory\n");
									fseek(fp, 0, SEEK_SET);
									fwrite(&sb, BLOCK_SIZE, 1, fp);
									
									//printf("directory creating process complete\n");
									
									sync_bitmaps();
									
									return;
								}
							}
						}
					}
					
				} else {
					//printf("found a directory entry in use\n");
				}
				
			}
			
		}
		
		
		struct directory_entry *possible = malloc(sizeof(struct directory_entry));
		int found_child = 0;
		
		
		
		for (i = 0; i < 8; i++) {
			//printf("looking through potential directory location %d...\n\n", i);
			
			//fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
			fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
			//fread(possible, sizeof(struct directory_entry), 1, fp);
			fread(free_block, BLOCK_SIZE, 1, fp);
			print_bits(free_block, BLOCK_SIZE);
			memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
			//print_bits(possible, sizeof(struct directory_entry));
			//print_bits(possible, sizeof(struct directory_entry));
			/*
			if (strcmp(possible->d_name, found) == 0 && possible->d_file_type == 2) {
				printf("found a child folder to move into\n");
				search_node = possible->d_inode;
				found_child = 1;
			}
			*/
			if (possible->d_file_type == 2) {
				//printf("found a directory at entry[%d]\n", i);
				char * name = (char*) malloc(57*sizeof(char));
				strncpy(name, possible->d_name, possible->d_name_len);
				name[possible->d_name_len] = '\0';
				//printf("its name is %s\n", name);
				if (strcmp(name, found) == 0) {
					//printf("match found!\n");
					//printf("switching search_node from %d to %d\n", search_node, possible->d_inode);
					search_node = possible->d_inode;
					found_child = 1;
					i = 10;
					current_sub++;
				}
			}
		}
		if (!found_child) {
			//printf("couldn't file child path\n");
			printf("ERROR: Could not reach path\n");
			exit(1);
		}
		
		if (depth == num_subs) {
			//printf("reached max depth and found nothing\n");
			exit(1);
		}
	}
	
	
	
	
	
	
}

/** Read a directory with the given path into data.
 */
 
 
void read_stats(char *path, unsigned int n, struct stat *stbuf)
{
    /*
     * For the file system image tha t is currently opened.
     * Delete the file in the path provided.
     * Make sure that the data blocks are freed after deleting the file.
     */
	
	
	
	
	if (!strcmp(path, "/")) {
		printf("looking at root\n");
		strcat(path, ".");
		n++;
	}
	
	char * found = (char*) malloc(n*sizeof(char));
	char * to_slice = (char*) malloc(n*sizeof(char));
	char * to_slice2 = (char*) malloc(n*sizeof(char));
	
	unsigned char free_block[BLOCK_SIZE] = {0};
	
	char new_directory[n];
	strncpy(new_directory, path, n);
	
	new_directory[n] = '\0';
	
	printf("Attempting to read stats at path - %s\n", new_directory);
	if (n <= 0) {
		printf("ERROR: provided path is too short\n");
		exit(1);
	}
	if (new_directory[0] != '/') {
		printf("ERROR: file path should start with /\n");
		errno = ENOENT;
		return;
	}
	
	if (new_directory[n-1] == '/') {
		printf("input is a directory\n");
	} else {
		printf("input is a file\n");
	}
	
	//printf("checks complete\n");
	//printf("about to slice\n");
	strncpy(to_slice, new_directory + 1, n);
	strncpy(to_slice2, new_directory + 1, n);
	
	to_slice2[n-1] = '\0';
	to_slice2[n-1] = '\0';
	//printf("string is %s\n", to_slice2);
	int search_node = 2;
	
	int num_subs = -1;
	
	while((found = strsep(&to_slice2, "/")) != NULL) {
		if((int)strlen(found) > 57) {
			printf("ERROR: folder names can not be longer than 57 characters\n");
			errno = ENAMETOOLONG;
			return;
		}
		num_subs++;
	}
	
	printf("depth of file to check: %d\n", num_subs);
	

	
	
	int i = 0;
	int current_sub = 0;
	int block = 1;
	
	//int depth = 1;
	
	struct directory_entry *possible = malloc(sizeof(struct directory_entry));
	
	while((found = strsep(&to_slice, "/")) != NULL) {
		printf("looking for directory: %s\n", found);
		printf("directory length: %d\n", (int)strlen(found));
		
		struct inode *searching = malloc(sizeof(struct inode));
		fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
		fread(free_block, BLOCK_SIZE, 1, fp);
		memcpy(searching, free_block + (sizeof(struct inode) * (search_node % 8)), sizeof(struct inode));
		
		printf("printing inode %d\n", search_node);
		print_bits(searching, sizeof(struct inode));
		
		block = 8 + searching->i_block[0];
		printf("looking through block %d\n", block);
		
		if (current_sub ==  num_subs) {
			//printf("we are in the folder where we want to delete the file\n");
			//printf("looking for file\n");
			for (i = 0; i < 8; i++) {
				printf("looking through potential directory location %d...\n\n", i);
				//fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				//fread(possible, sizeof(struct directory_entry), 1, fp);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				
				print_bits(free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				if (possible->d_file_type == 1 || possible->d_file_type == 2) { //possible->d_file_type == 1
					printf("found a file/directory at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					printf("its name is %s\n", name);
					if (strcmp(name, found) == 0) {
						//printf("match found!\n");
						//printf("this file has inode %d\n", possible->d_inode);
						
						unsigned char blank_block[BLOCK_SIZE] = {0};
						
						struct inode *file_inode = malloc(sizeof(struct inode));
						//fseek(fp, BLOCK_SIZE * 3 + (possible->d_inode * sizeof(struct inode)) , SEEK_SET);
						fseek(fp, BLOCK_SIZE * 3 + ((possible->d_inode / 8) * BLOCK_SIZE), SEEK_SET);
						//fread(file_inode, sizeof(struct inode), 1, fp);
						fread(free_block, BLOCK_SIZE, 1, fp);
						memcpy(file_inode, free_block + ((possible->d_inode % 8) * sizeof(struct inode)), sizeof(struct inode));
						
						if (!(file_inode->i_mode | S_IRUSR)) {
							printf("ERROR: No permission to read file\n");
							errno = EACCES;
							return;
						}
						
						printf("filling in stbuf\n\n");
						printf("st_mode = %d\n", file_inode->i_mode);
						stbuf->st_mode = file_inode->i_mode;
						printf("st_nlink = %d\n", file_inode->i_links_count);
						stbuf->st_nlink = file_inode->i_links_count;
						printf("st_uid = %d\n", file_inode->i_uid);
						stbuf->st_uid = file_inode->i_uid;
						printf("st_gid = %d\n", file_inode->i_gid);
						stbuf->st_gid = file_inode->i_gid;
						printf("st_size = %d\n", file_inode->i_size);
						stbuf->st_size = file_inode->i_size;
						printf("st_blocks = %d\n", file_inode->i_blocks);
						stbuf->st_blocks = file_inode->i_blocks;
						printf("st_atime = %d\n", file_inode->i_time);
						stbuf->st_atime = file_inode->i_time;
						printf("st_mtime = %d\n", file_inode->i_mtime);
						stbuf->st_mtime = file_inode->i_mtime;
						printf("st_ctime = %d\n", file_inode->i_ctime);
						stbuf->st_ctime = file_inode->i_ctime;
						return;
					}
				}
			}
			return;
		} else {
			//printf("we are not in the directory where we delete the file\n");
			
			
			int found_child = 0;
			
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				fread(possible, sizeof(struct directory_entry), 1, fp);
				if (possible->d_file_type == 2) {
					//printf("found a directory at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					if (strcmp(name, found) == 0) {
						//printf("match found!\n");
						//printf("switching search_node from %d to %d\n", search_node, possible->d_inode);
						search_node = possible->d_inode;
						found_child = 1;
						i = 10;
						current_sub++;
					}
				}
			}
			if (!found_child) {
				//printf("couldn't file child path\n");
				printf("ERROR: Could delete file, could not reach path\n");
				errno = EFAULT;
				return;
			}
		
			
		}
	}
	
}


unsigned int read_directory_fuse(char *path, unsigned int n, void *buf, fuse_fill_dir_t filler)
{
	/*
     * For the file system image that is currently opened.
     * Read the contents of the directory in the path provided.
     * Make sure that the path is valid.
     * Place the directory entries in data.
     * and return the number of bytes that have been placed in data.
     */
	 
	 
	char * found = (char*) malloc(n*sizeof(char));
	char * to_slice = (char*) malloc(n*sizeof(char));
	char * to_slice2 = (char*) malloc(n*sizeof(char));
	
	char new_directory[n];
	strncpy(new_directory, path, n);
	new_directory[n] = '\0';
	
	unsigned char free_block[BLOCK_SIZE] = {0};
	
	//printf("attempting to read directory at path - %s\n", new_directory);
	
	if (n <= 0) {
		printf("ERROR: provided path is too short\n");
		errno = ENOENT;
		return -1;
	}
	if (new_directory[0] != '/') {
		printf("ERROR: file path should start with /\n");
		errno = EFAULT;
		return -1;
	}
	/*
	if (new_directory[n-1] != '/') {
		printf("ERROR: file path should end with /\n");
		exit(1);
	}
	*/
	//printf("checks complete\n");
	
	int num_subs = 0;
	
	if(strcmp("/", new_directory) == 0) {
		strcpy(to_slice, "");
		strcpy(to_slice2, "");
		//printf("looking through the root directory\n");
	} else {
		strncpy(to_slice, new_directory + 1, n - 1);
		strncpy(to_slice2, new_directory + 1, n - 1);
		
		to_slice[n - 2] = '\0';
		to_slice2[n - 2] = '\0';
		//printf("string is %s\n", to_slice2);
	}
	
	

	int search_node = 2;
	
	
	
	while((found = strsep(&to_slice2, "/")) != NULL) {
		if((int)strlen(found) > 57) {
			printf("ERROR: folder names can not be longer than 57 characters\n");
			errno = ENAMETOOLONG;
			return -1;
		}
		num_subs++;
	}
	
	if (strcmp("/", new_directory) == 0) {
		num_subs = 0;
	}
	
	//printf("depth of directory to read: %d\n", num_subs);
	
	int i = 0;
	int current_sub = 0;
	int block = 0;
	

	int depth = 0;
	
	while((found = strsep(&to_slice, "/")) != NULL || 1) {
		
		if (!found) {
			found = "";
		}
		
		//printf("looking for directory: %s\n", found);
		//printf("directory length: %d\n", (int)strlen(found));
		
		
		struct inode *searching = malloc(sizeof(struct inode));
		fseek(fp, BLOCK_SIZE * 3 + ((search_node / 8) * BLOCK_SIZE) , SEEK_SET);
		fread(free_block, BLOCK_SIZE, 1, fp);
		memcpy(searching, free_block + (sizeof(struct inode) * (search_node % 8)), sizeof(struct inode));
		
		block = 8 + searching->i_block[0];
		//printf("looking through block %d\n", block);
		
		if (current_sub ==  num_subs) {
			//printf("we are in the directory we want to read.\n");
			//int num_entries = 0;
			
			if (!(searching->i_mode | S_IRUSR)) {
				printf("ERROR: No permission to read directory\n");
				errno = EACCES;
				return -1;
			}
			
			//num_entries = searching->i_size / sizeof(struct directory_entry);
			//printf("this directory has %d entries\n", num_entries);
			
			int num_written = 0;
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			
			for (i = 2; i < 8; i++) {
				//printf("checking entry %d to see if we want to write it\n", i);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				if (possible->d_file_type != 0) {
					//printf("writing %s\n", possible->d_name);
					//print_bits(possible, sizeof(struct directory_entry));
					//memcpy(data + (num_written * sizeof(struct directory_entry)), possible, sizeof(struct directory_entry));
					//num_written++;
					filler(buf, possible->d_name, NULL, 0);
					//printf("num entries written: %d\n", num_written);
					
				}
			}
			
			//printf("total number of bytes written: %d * %d = %d bytes\n", num_written, (int)sizeof(struct directory_entry), (int)searching->i_size);
			sync_bitmaps();
			
			searching->i_time = time(0);
			fseek(fp, BLOCK_SIZE * 3 + ((search_node / 8) * BLOCK_SIZE) , SEEK_SET);
			fread(free_block, BLOCK_SIZE, 1, fp);
			//print_bits(free_block, BLOCK_SIZE);
			memcpy(free_block + (sizeof(struct inode) * (search_node % 8)), searching, sizeof(struct inode));
			//print_bits(free_block, BLOCK_SIZE);
			fseek(fp, BLOCK_SIZE * 3 + ((search_node / 8) * BLOCK_SIZE) , SEEK_SET);
			fwrite(free_block, BLOCK_SIZE, 1, fp);
			//return(searching->i_size);
			return 0;
			
		} else {
			//printf("we are not in the directory we want to read.\n");
			
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			int found_child = 0;
			
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				if (possible->d_file_type == 2) {
					//printf("found a directory at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					if (strcmp(name, found) == 0) {
						//printf("match found!\n");
						//printf("switching search_node from %d to %d\n", search_node, possible->d_inode);
						search_node = possible->d_inode;
						found_child = 1;
						i = 10;
						current_sub++;
					}
				}
			}
			
			if (!found_child) {
				//printf("couldn't file child path\n");
				printf("ERROR: Could not reach path\n");
				errno = EFAULT;
				return -1;
			}
		
			if (depth == num_subs) {
				//printf("reached max depth and found nothing\n");
				errno = EFAULT;
				return -1;
			}
			//printf("hmmm...\n");
		}
	}
	
	return 0;
	
	
}

unsigned int read_directory(char *path, unsigned int n, char *data)
{
    /*
     * For the file system image that is currently opened.
     * Read the contents of the directory in the path provided.
     * Make sure that the path is valid.
     * Place the directory entries in data.
     * and return the number of bytes that have been placed in data.
     */
	 
	 
	char * found = (char*) malloc(n*sizeof(char));
	char * to_slice = (char*) malloc(n*sizeof(char));
	char * to_slice2 = (char*) malloc(n*sizeof(char));
	
	char new_directory[n];
	strncpy(new_directory, path, n);
	new_directory[n] = '\0';
	
	unsigned char free_block[BLOCK_SIZE] = {0};
	
	//printf("attempting to read directory at path - %s\n", new_directory);
	
	if (n <= 0) {
		printf("ERROR: provided path is too short\n");
		errno = ENOENT;
		return -1;
	}
	if (new_directory[0] != '/') {
		printf("ERROR: file path should start with /\n");
		errno = EFAULT;
		return -1;
	}
	/*
	if (new_directory[n-1] != '/') {
		printf("ERROR: file path should end with /\n");
		exit(1);
	}
	*/
	//printf("checks complete\n");
	
	int num_subs = 0;
	
	if(strcmp("/", new_directory) == 0) {
		strcpy(to_slice, "");
		strcpy(to_slice2, "");
		//printf("looking through the root directory\n");
	} else {
		strncpy(to_slice, new_directory + 1, n - 1);
		strncpy(to_slice2, new_directory + 1, n - 1);
		
		to_slice[n - 2] = '\0';
		to_slice2[n - 2] = '\0';
		//printf("string is %s\n", to_slice2);
	}
	
	

	int search_node = 2;
	
	
	
	while((found = strsep(&to_slice2, "/")) != NULL) {
		if((int)strlen(found) > 57) {
			printf("ERROR: folder names can not be longer than 57 characters\n");
			errno = ENAMETOOLONG;
			return -1;
		}
		num_subs++;
	}
	
	if (strcmp("/", new_directory) == 0) {
		num_subs = 0;
	}
	
	//printf("depth of directory to read: %d\n", num_subs);
	
	int i = 0;
	int current_sub = 0;
	int block = 0;
	

	int depth = 0;
	
	while((found = strsep(&to_slice, "/")) != NULL || 1) {
		
		if (!found) {
			found = "";
		}
		
		//printf("looking for directory: %s\n", found);
		//printf("directory length: %d\n", (int)strlen(found));
		
		
		struct inode *searching = malloc(sizeof(struct inode));
		fseek(fp, BLOCK_SIZE * 3 + ((search_node / 8) * BLOCK_SIZE) , SEEK_SET);
		fread(free_block, BLOCK_SIZE, 1, fp);
		memcpy(searching, free_block + (sizeof(struct inode) * (search_node % 8)), sizeof(struct inode));
		
		block = searching->i_block[0];
		//printf("looking through block %d\n", block);
		
		if (current_sub ==  num_subs) {
			//printf("we are in the directory we want to read.\n");
			//int num_entries = 0;
			
			if (!(searching->i_mode | S_IRUSR)) {
				printf("ERROR: No permission to read directory\n");
				errno = EACCES;
				return -1;
			}
			
			//num_entries = searching->i_size / sizeof(struct directory_entry);
			//printf("this directory has %d entries\n", num_entries);
			
			int num_written = 0;
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			
			for (i = 0; i < 8; i++) {
				//printf("checking entry %d to see if we want to write it\n", i);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				if (possible->d_file_type != 0) {
					//printf("and we can!\n");
					print_bits(possible, sizeof(struct directory_entry));
					memcpy(data + (num_written * sizeof(struct directory_entry)), possible, sizeof(struct directory_entry));
					num_written++;
					//printf("num entries written: %d\n", num_written);
					
				}
			}
			
			//printf("total number of bytes written: %d * %d = %d bytes\n", num_written, (int)sizeof(struct directory_entry), (int)searching->i_size);
			sync_bitmaps();
			
			searching->i_time = time(0);
			fseek(fp, BLOCK_SIZE * 3 + ((search_node / 8) * BLOCK_SIZE) , SEEK_SET);
			fread(free_block, BLOCK_SIZE, 1, fp);
			//print_bits(free_block, BLOCK_SIZE);
			memcpy(free_block + (sizeof(struct inode) * (search_node % 8)), searching, sizeof(struct inode));
			//print_bits(free_block, BLOCK_SIZE);
			fseek(fp, BLOCK_SIZE * 3 + ((search_node / 8) * BLOCK_SIZE) , SEEK_SET);
			fwrite(free_block, BLOCK_SIZE, 1, fp);
			return(searching->i_size);
			
		} else {
			//printf("we are not in the directory we want to read.\n");
			
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			int found_child = 0;
			
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				if (possible->d_file_type == 2) {
					//printf("found a directory at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					if (strcmp(name, found) == 0) {
						//printf("match found!\n");
						//printf("switching search_node from %d to %d\n", search_node, possible->d_inode);
						search_node = possible->d_inode;
						found_child = 1;
						i = 10;
						current_sub++;
					}
				}
			}
			
			if (!found_child) {
				//printf("couldn't file child path\n");
				printf("ERROR: Could not reach path\n");
				errno = EFAULT;
				return -1;
			}
		
			if (depth == num_subs) {
				//printf("reached max depth and found nothing\n");
				errno = EFAULT;
				return -1;
			}
			//printf("hmmm...\n");
		}
	}
	
	return 0;
}

/** Remove a directory with the given path.
 */
void rm_directory(char *path, unsigned int n)
{
    /*
     * For the file system image that is currently opened.
     * Delete the directory in the path provided.
     * Make sure that the directory doesn't have any files remaining
     */
	
	
	unsigned char free_block[BLOCK_SIZE] = {0};
	
	char * found = (char*) malloc(n*sizeof(char));
	char * to_slice = (char*) malloc(n*sizeof(char));
	char * to_slice2 = (char*) malloc(n*sizeof(char));
	
	struct directory_entry *possible = malloc(sizeof(struct directory_entry));
	
	char new_directory[n];
	strncpy(new_directory, path, n);
	new_directory[n] = '\0';
	
	//printf("attempting to remove directory at path - %s\n", new_directory);
	
	if (n <= 0) {
		printf("ERROR: provided path is too short\n");
		exit(1);
	}
	if (new_directory[0] != '/') {
		printf("ERROR: directory path should start with /\n");
		exit(1);
	}
	/*
	if (new_directory[n-1] != '/') {
		printf("ERROR: directory path should end with /\n");
		exit(1);
	}
	*/
	//printf("checks complete\n");
	
	int num_subs = 0;
	
	if(strcmp("/", new_directory) == 0) {
		printf("ERROR: you aren't allowed to delete the root directory\n");
		exit(1);
	} else {
		strncpy(to_slice, new_directory + 1, n - 1);
		strncpy(to_slice2, new_directory + 1, n - 1);
	
		to_slice2[n-2] = '\0';
		to_slice2[n-2] = '\0';
		//printf("string is %s\n", to_slice2);
	}
	
	

	int search_node = 2;
	
	
	
	while((found = strsep(&to_slice2, "/")) != NULL) {
		if((int)strlen(found) > 57) {
			printf("ERROR: folder names can not be longer than 57 characters\n");
			exit(1);
		}
		num_subs++;
	}
	
	if (strcmp("/", new_directory) == 0) {
		num_subs = 0;
	}
	
	//printf("depth of directory to delete: %d\n", num_subs);
	
	int i = 0;
	int current_sub = 0;
	int block = 0;
	

	int depth = 0;
	
	while((found = strsep(&to_slice, "/")) != NULL || 1) {
		
		if (!found) {
			found = "";
		}
		//printf("looking for directory: %s\n", found);
		//printf("directory length: %d\n", (int)strlen(found));
		
		
		struct inode *searching = malloc(sizeof(struct inode));
		fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
		fread(free_block, BLOCK_SIZE, 1, fp);
		memcpy(searching, free_block + (sizeof(struct inode) * (search_node % 8)), sizeof(struct inode));
		block = searching->i_block[0];
		
		//printf("looking through block %d\n", block);
		
		if (current_sub ==  num_subs) {
			//printf("we are in the directory we want to delete\n");
			
			if (!(searching->i_mode | S_IRUSR)) {
				printf("ERROR: No permission to read directory\n");
				exit(1);
			}
			if (!(searching->i_mode | S_IWUSR)) {
				printf("ERROR: No permission to write directory\n");
				exit(1);
			}
			
			int num_entries = 0;
			num_entries = searching->i_size / sizeof(struct directory_entry);
			printf("this directory has %d entries\n", num_entries);
			
			
			for (i = 2; i < 8; i++) {
				//fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				//fread(possible, sizeof(struct directory_entry), 1, fp);
				if (possible->d_file_type == 1) {
					printf("deleting a file\n");
					char str[256];
					strncpy(str, path, n);
					strncat(str, possible->d_name, possible->d_name_len);
					rm_file(str, strlen(str));
				} else if (possible->d_file_type == 2) {
					printf("recursively deleting a directory\n");
					char str[256];
					strncpy(str, path, n);
					strncat(str, possible->d_name, possible->d_name_len);
					rm_directory(str, strlen(str));
				}
			}
			
			//printf("deleting this directory's inode\n");
			searching->i_mode = 0;
			searching->i_uid = 0;
			searching->i_gid = 0;
			searching->i_links_count = 0;
			searching->i_size = 0;
			searching->i_time = time(0);
			searching->i_mtime = searching->i_time;
			searching->i_dtime = searching->i_time;
			searching->i_blocks = 0;
			
			searching->i_block[0] = 0;
			
			fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
			fread(free_block, BLOCK_SIZE, 1, fp);
			memcpy(free_block + (sizeof(struct inode) * (search_node % 8)), searching, sizeof(struct inode));
			fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
			fwrite(free_block, BLOCK_SIZE, 1, fp);
			
			int block_to_delete = block;
			//printf("removing this directory's block from the block bitmap (%d)\n", block);
			
			unsigned char *bitmap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
			fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
			fread(bitmap, BLOCK_SIZE, 1, fp);
			
			set_bit(bitmap, block, 0);
			
			fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
			fwrite(bitmap, BLOCK_SIZE, 1, fp);
			
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			
			fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
			fread(free_block, BLOCK_SIZE, 1, fp);
			
			memcpy(possible, free_block + (0 * sizeof(struct directory_entry)), sizeof(struct directory_entry));
			
			int directory_to_delete = possible->d_inode;
			//printf("removing this directory's inode from the inode bitmap (%d)\n", possible->d_inode);
			
			fseek(fp, (BLOCK_SIZE * 2), SEEK_SET);
			fread(bitmap, BLOCK_SIZE, 1, fp);
			
			set_bit(bitmap, possible->d_inode, 0);
			
			fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
			fwrite(bitmap, BLOCK_SIZE, 1, fp);
			
			
			//fseek(fp, (block * BLOCK_SIZE) + (1 * sizeof(struct directory_entry)), SEEK_SET);
			//fread(possible, sizeof(struct directory_entry), 1, fp);
			memcpy(possible, free_block + (1 * sizeof(struct directory_entry)), sizeof(struct directory_entry));
			
			//printf("hopping to directory above this to remove this directory's entry (%d)\n", possible->d_inode);
			
			fseek(fp, (BLOCK_SIZE * 3) + ((possible->d_inode / 8) * BLOCK_SIZE), SEEK_SET);
			//fseek(fp, BLOCK_SIZE * 3 + (possible->d_inode * sizeof(struct inode)) , SEEK_SET);
			fread(free_block, BLOCK_SIZE, 1, fp);
			
			memcpy(searching, free_block + (sizeof(struct inode) * (possible->d_inode % 8)), sizeof(struct inode));
			//fread(searching, sizeof(struct inode), 1, fp);
			
			searching->i_size -= sizeof(struct directory_entry);
			searching->i_time = time(0);
			searching->i_mtime = searching->i_time;
			
			//printf("looking through the parent directory's block (%d)\n", searching->i_block[0]);
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (searching->i_block[0] * BLOCK_SIZE) , SEEK_SET);
				//fseek(fp, (searching->i_block[0] * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				//fread(possible, sizeof(struct directory_entry), 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct inode));
				
				
				if (possible->d_inode == directory_to_delete) {
					//printf("found the directory to delete at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					
					//printf("clearing this directory entry out\n");
					struct directory_entry p;
					p.d_inode = 0;
					p.d_file_type = 0;
					p.d_name_len = 0;
					memset(p.d_name, 0, 57);
					//fseek(fp, (searching->i_block[0] * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
					fseek(fp, (searching->i_block[0] * BLOCK_SIZE) , SEEK_SET);
					fread(free_block, BLOCK_SIZE, 1, fp);
					memcpy(free_block + (i * sizeof(struct directory_entry)), &p, sizeof(struct directory_entry));
					fseek(fp, (searching->i_block[0] * BLOCK_SIZE) , SEEK_SET);
					fwrite(free_block, BLOCK_SIZE, 1, fp);
					
					//printf("now clearing out the memory block of the deleted directory (%d)\n", block_to_delete);
					unsigned char blank_block[BLOCK_SIZE] = {0};
					fseek(fp, (block_to_delete * BLOCK_SIZE), SEEK_SET);
					fwrite(blank_block, BLOCK_SIZE, 1, fp);
					//printf("modifying superblock\n");
					sb.s_free_inodes_count += 1;
					sb.s_free_blocks_count += 1;
					fseek(fp, 0, SEEK_SET);
					fwrite(&sb, BLOCK_SIZE, 1, fp);
					
					//printf("directory removal process complete\n");
					
					sync_bitmaps();
					
					return;
				}
			}
			
			return;
			
		} else {
			//printf("we are not in the directory we want to delete.\n");
			
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			int found_child = 0;
			
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				if (possible->d_file_type == 2) {
					//printf("found a directory at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					if (strcmp(name, found) == 0) {
					//	printf("match found!\n");
					//	printf("switching search_node from %d to %d\n", search_node, possible->d_inode);
						search_node = possible->d_inode;
						found_child = 1;
						i = 10;
						current_sub++;
					}
				}
			}
			if (!found_child) {
				//printf("couldn't file child path\n");
				printf("ERROR: Could not make directory, could not reach path\n");
				exit(1);
			}
		
			if (depth == num_subs) {
				//printf("reached max depth and found nothing\n");
				exit(1);
			}
		}
	}
	
	return;
}

/** Create a file with the given path.
 */
void create_file(char *path, unsigned int n, unsigned int size, char *data)
{
    /*
     * For the file system image that is currently opened.
     * Create a new file at path.
     * Make sure that the path is valid.
     * Write to the contents of the file in the data provided.
     * size is the number of bytes in the data.
     */
	
	if (size < 0) {
		printf("ERROR: Please specify a size of at least one byte of memory to write\n");
		exit(1);
	}
	
	int num_blocks = 1 + (size / BLOCK_SIZE);
	if (size % BLOCK_SIZE == 0) {
		num_blocks--;
	}
	
	//printf("Attempting to write %d bytes, which will take up %d block(s)\n", size, num_blocks);
	
	if (sb.s_free_blocks_count < num_blocks) {
		printf("ERROR: Not enough blocks left in the filesystem to create the file\n");
		errno = ENOMEM;
		return;
	}
	
	if (num_blocks > 8) {
		printf("ERROR: Size of file is too big. Should be <= to 8*512 bytes\n");
		errno = ENOMEM;
		return;
	}
	/*if (sb.s_free_inodes_count == 0) {
		printf("ERROR: no more free inodes with which to allocate a file\n");
		exit(1);
	}*/
	unsigned char free_block[BLOCK_SIZE] = {0};
	
	char * found = (char*) malloc(n*sizeof(char));
	char * to_slice = (char*) malloc(n*sizeof(char));
	char * to_slice2 = (char*) malloc(n*sizeof(char));
	
	char new_directory[n];
	strncpy(new_directory, path, n);
	new_directory[n] = '\0';
	
	//printf("Attempting to make a file at path - %s\n", new_directory);
	if (n <= 1) {
		printf("ERROR: provided path is too short\n");
		errno = EFAULT;
		return;
	}
	if (new_directory[0] != '/') {
		printf("ERROR: file path should start with /\n");
		errno = EFAULT;
		return;
	}
	if (new_directory[n-1] == '/') {
		printf("ERROR: file path should end not end with /\n");
		errno = EFAULT;
		return;
	}
	
	//printf("checks complete\n");
	//printf("about to slice\n");
	strncpy(to_slice, new_directory + 1, n);
	strncpy(to_slice2, new_directory + 1, n);
	
	to_slice2[n-1] = '\0';
	to_slice2[n-1] = '\0';
	//printf("string is %s\n", to_slice2);
	int search_node = 2;
	
	int num_subs = -1;
	
	while((found = strsep(&to_slice2, "/")) != NULL) {
		if((int)strlen(found) > 57) {
			printf("ERROR: folder names can not be longer than 57 characters\n");
			errno = ENAMETOOLONG;
			return;
		}
		num_subs++;
	}
	
	//printf("depth of new file: %d\n", num_subs);
	
	int i = 0; int j = 0; int k = 0;
	int current_sub = 0;
	int block = 0;
	
	int depth = 1;
	
	while((found = strsep(&to_slice, "/")) != NULL) {
		//printf("looking for directory: %s\n", found);
		//printf("directory length: %d\n", (int)strlen(found));
		
		struct inode *searching = malloc(sizeof(struct inode));
		fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
		fread(free_block, BLOCK_SIZE, 1, fp);
		memcpy(searching, free_block + (sizeof(struct inode) * (search_node % 8)), sizeof(struct inode));
		block = searching->i_block[0];
		//printf("looking through block %d\n", block);
	
		if (current_sub ==  num_subs) {
			//printf("we are in the folder where we want to create the file\n");
			
			//printf("looking for a free directory entry\n");
			
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			int entry_to_write_to = -1;
			for (i = 0; i < 8; i++) {
				//fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				//fread(possible, sizeof(struct directory_entry), 1, fp);
				if (possible->d_file_type == 0) {
					//printf("entry %d is free\n", i);
					entry_to_write_to = i;
					i = 8;
				}
				
			}
			
			if (entry_to_write_to == -1) {
				printf("ERROR: This folder is full\n");
				errno = ENOMEM;
				return;
			}
			
			searching->i_size += sizeof(struct directory_entry);
			searching->i_mtime = time(0);
			searching->i_time = searching->i_mtime;
			//fseek(fp, BLOCK_SIZE * 3 + (search_node * sizeof(struct inode)) , SEEK_SET);
			
			fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
			fread(free_block, BLOCK_SIZE, 1, fp);
			//print_bits(free_block, BLOCK_SIZE);
			memcpy(free_block + (sizeof(struct inode) * (search_node % 8)), searching, sizeof(struct inode));
			//print_bits(free_block, BLOCK_SIZE);
			//fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
			fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
			fwrite(free_block, BLOCK_SIZE, 1, fp);
			//return;
			//printf("making a new inode for this file.\n");
			
			struct inode new_inode;
			new_inode.i_mode = new_inode.i_mode | S_IFREG;
			new_inode.i_mode = new_inode.i_mode | S_IRWXU;
			new_inode.i_mode = new_inode.i_mode | S_IRWXG;
			new_inode.i_mode = new_inode.i_mode | S_IRWXO;
			
			new_inode.i_uid = getuid();
			new_inode.i_gid = getgid();
			
			new_inode.i_links_count = 1;
			
			new_inode.i_size = size;
			
			new_inode.i_time = time(0);
			new_inode.i_ctime = new_inode.i_time;
			new_inode.i_mtime = new_inode.i_time;
			
			new_inode.i_blocks = num_blocks;
			
			sb.s_free_inodes_count -= 1;
			sb.s_free_blocks_count -= num_blocks;
			
			//printf("looking for free inode in bitmap\n");
			unsigned char *inode_bitmap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
			fseek(fp, (BLOCK_SIZE * 2), SEEK_SET);
			fread(inode_bitmap, BLOCK_SIZE, 1, fp);
			for (j = 0; j < sb.s_inodes_count; j++) {
				if (check_bit(inode_bitmap, j) == 0) {
					//printf("inode %d is free\n", j);
					//printf("looking for one free block in memory\n");
					unsigned char *block_bitmap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
					fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
					fread(block_bitmap, BLOCK_SIZE, 1, fp);
					for (k = 0; k < sb.s_blocks_count; k++) {
						if (check_bit(block_bitmap, k) == 0) {
							//printf("block %d is free\n", k);
							
							
							
							//printf("now creating directory entry for this directory in parent\n");
							
							
							possible->d_inode = j;
							possible->d_file_type = 1;
							possible->d_name_len = strlen(found);
							//printf("saving file name length: %d\n", possible->d_name_len);
							strncpy(possible->d_name, found, strlen(found));
							//printf("saving file name: %s\n", possible->d_name);
							
							
							
							//printf("saving this directory entry at position %d\n", entry_to_write_to);
							//fseek(fp, (block * BLOCK_SIZE) + (entry_to_write_to * sizeof(struct directory_entry)), SEEK_SET);
							fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
							fread(free_block, BLOCK_SIZE, 1, fp);
							memcpy(free_block + (entry_to_write_to * sizeof(struct directory_entry)), possible, sizeof(struct directory_entry));
							fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
							fwrite(free_block, BLOCK_SIZE, 1, fp);
							//fwrite(possible, sizeof(struct directory_entry), 1, fp);
							
							//printf("now writing the inode bitmap\n");
							set_bit(inode_bitmap, j, 1);
							fseek(fp, (BLOCK_SIZE * 2), SEEK_SET);
							fwrite(inode_bitmap, BLOCK_SIZE, 1, fp);
							
							int num_blocks_written = 0;
							
							unsigned char *block_bitmap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
							
							
							//printf("now beginning the file writing process\n");
							int block_to_write_to = 0;
							for (num_blocks_written = 0; num_blocks_written < num_blocks; num_blocks_written++) {
								//printf("writing block %d of %d\n", num_blocks_written + 1, num_blocks);
								if (num_blocks_written == 0) {
									block_to_write_to = k + 8;
									//printf("writing block %d of file to block in memory %d\n", num_blocks_written + 1, k);
								} else {
									for (k = 0; k < sb.s_blocks_count; k++) {
										if (check_bit(block_bitmap, k) == 0) {
											//printf("writing block %d of file to block in memory %d\n", num_blocks_written + 1, k);
											block_to_write_to = k + 8;
											k = sb.s_blocks_count;
										} else {
											//printf("block %d is in use\n", k);
										}
									}
								}
								//printf("writing to block %d\n", block_to_write_to - 8);
								
								new_inode.i_block[num_blocks_written] = block_to_write_to - 8;
								
								unsigned char block_to_write[BLOCK_SIZE] = {0};
								memcpy(block_to_write, data + (BLOCK_SIZE * num_blocks_written), BLOCK_SIZE);
								
								print_bits(block_to_write, BLOCK_SIZE);
								
								fseek(fp, (BLOCK_SIZE * block_to_write_to), SEEK_SET);
								fwrite(block_to_write, BLOCK_SIZE, 1, fp);
								
								//printf("updating the block bitmap\n");
								fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
								fread(block_bitmap, BLOCK_SIZE, 1, fp);
								set_bit(block_bitmap, block_to_write_to - 8, 1);
								fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
								fwrite(block_bitmap, BLOCK_SIZE, 1, fp);
								
								
								
								//printf("successfully wrote block %d\n\n", block_to_write_to - 8);
								
								
							}
							
							//printf("file writing process complete\n");
							
							//printf("now writing inode to disk\n");
							//fseek(fp, (BLOCK_SIZE * 3) + (j * sizeof(struct inode)), SEEK_SET);
							fseek(fp, (BLOCK_SIZE * 3) + ((j / 8) * BLOCK_SIZE), SEEK_SET);
							fread(free_block, BLOCK_SIZE, 1, fp);
							memcpy(free_block + (sizeof(struct inode) * (j % 8)), &new_inode, sizeof(struct inode));
							fseek(fp, (BLOCK_SIZE * 3) + ((j / 8) * BLOCK_SIZE), SEEK_SET);
							fwrite(free_block, BLOCK_SIZE, 1, fp);
						//	fwrite(&new_inode, sizeof(struct inode), 1, fp);
							
							//printf("writing superblock to memory\n");
							fseek(fp, 0, SEEK_SET);
							fwrite(&sb, BLOCK_SIZE, 1, fp);
							
							//printf("file creation process complete\n");
							
							sync_bitmaps();
							return;
							
						}
					}
				}
			}
			
			return;
		} else {
			//printf("we are not in the directory where we create the file\n");
			
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			int found_child = 0;
			
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				fread(possible, sizeof(struct directory_entry), 1, fp);
				if (possible->d_file_type == 2) {
					//printf("found a directory at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					if (strcmp(name, found) == 0) {
						//printf("match found!\n");
						//printf("switching search_node from %d to %d\n", search_node, possible->d_inode);
						search_node = possible->d_inode;
						found_child = 1;
						i = 10;
						current_sub++;
					}
				}
			}
			if (!found_child) {
				//printf("couldn't file child path\n");
				//printf("ERROR: Could not make directory, could not reach path\n");
				errno = EFAULT;
				return;
			}
		
			if (depth == num_subs) {
				//printf("reached max depth and found nothing\n");
				errno = EFAULT;
				return;
			}
		}
		
	}
	

	
}

unsigned int fill_file(char *path, unsigned int n, unsigned int size, char *data)
{
	    /*
     * For the file system image that is currently opened.
     * Create a new file at path.
     * Make sure that the path is valid.
     * Write to the contents of the file in the data provided.
     * size is the number of bytes in the data.
     */
	
	if (size < 0) {
		printf("ERROR: Please specify a size of at least one byte of memory to write\n");
		return -1;
	}
	
	int num_blocks = 1 + (size / BLOCK_SIZE);
	if (size % BLOCK_SIZE == 0) {
		num_blocks--;
	}
	
	//printf("Attempting to write %d bytes, which will take up %d block(s)\n", size, num_blocks);
	
	if (sb.s_free_blocks_count < num_blocks) {
		printf("ERROR: Not enough blocks left in the filesystem to create the file\n");
		errno = ENOMEM;
		return -1;
	}
	
	/*if (sb.s_free_inodes_count == 0) {
		printf("ERROR: no more free inodes with which to allocate a file\n");
		exit(1);
	}*/
	unsigned char free_block[BLOCK_SIZE] = {0};
	
	char * found = (char*) malloc(n*sizeof(char));
	char * to_slice = (char*) malloc(n*sizeof(char));
	char * to_slice2 = (char*) malloc(n*sizeof(char));
	
	char new_directory[n];
	strncpy(new_directory, path, n);
	new_directory[n] = '\0';
	
	//printf("Attempting to make a file at path - %s\n", new_directory);
	if (n <= 1) {
		printf("ERROR: provided path is too short\n");
		errno = EFAULT;
		return -1;
	}
	if (new_directory[0] != '/') {
		printf("ERROR: file path should start with /\n");
		errno = EFAULT;
		return -1;
	}
	if (new_directory[n-1] == '/') {
		printf("ERROR: file path should end not end with /\n");
		errno = EFAULT;
		return -1;
	}
	
	//printf("checks complete\n");
	//printf("about to slice\n");
	strncpy(to_slice, new_directory + 1, n);
	strncpy(to_slice2, new_directory + 1, n);
	
	to_slice2[n-1] = '\0';
	to_slice2[n-1] = '\0';
	//printf("string is %s\n", to_slice2);
	int search_node = 2;
	
	int num_subs = -1;
	
	while((found = strsep(&to_slice2, "/")) != NULL) {
		if((int)strlen(found) > 57) {
			printf("ERROR: folder names can not be longer than 57 characters\n");
			errno = ENAMETOOLONG;
			return -1;
		}
		num_subs++;
	}
	
	//printf("depth of new file: %d\n", num_subs);
	
	int i = 0; int j = 0; int k = 0;
	int current_sub = 0;
	int block = 0;
	
	int depth = 1;
	
	while((found = strsep(&to_slice, "/")) != NULL) {
		//printf("looking for directory: %s\n", found);
		//printf("directory length: %d\n", (int)strlen(found));
		
		struct inode *searching = malloc(sizeof(struct inode));
		fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
		fread(free_block, BLOCK_SIZE, 1, fp);
		memcpy(searching, free_block + (sizeof(struct inode) * (search_node % 8)), sizeof(struct inode));
		block = searching->i_block[0];
		//printf("looking through block %d\n", block);
	
		if (current_sub ==  num_subs) {
			//printf("we are in the folder where we want to create the file\n");
			
			//printf("looking for a free directory entry\n");
			
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			int entry_to_write_to = -1;
			for (i = 0; i < 8; i++) {
				//fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				//fread(possible, sizeof(struct directory_entry), 1, fp);
				if (strcmp(possible->d_name, found) == 0) {
					//printf("entry %d is free\n", i);
					entry_to_write_to = i;
					i = 8;
				}
				
			}
			
			if (entry_to_write_to == -1) {
				printf("ERROR: file to fill not found\n");
				errno = EFAULT;
				return -1;
			}
			
			//searching->i_size += sizeof(struct directory_entry);
			searching->i_mtime = time(0);
			searching->i_time = searching->i_mtime;
			//fseek(fp, BLOCK_SIZE * 3 + (search_node * sizeof(struct inode)) , SEEK_SET);
			
			fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
			fread(free_block, BLOCK_SIZE, 1, fp);
			//print_bits(free_block, BLOCK_SIZE);
			memcpy(free_block + (sizeof(struct inode) * (search_node % 8)), searching, sizeof(struct inode));
			//print_bits(free_block, BLOCK_SIZE);
			//fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
			fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
			fwrite(free_block, BLOCK_SIZE, 1, fp);
			//return;
			//printf("making a new inode for this file.\n");
			
			struct inode new_inode;
			new_inode.i_mode = new_inode.i_mode | S_IFREG;
			new_inode.i_mode = new_inode.i_mode | S_IRWXU;
			new_inode.i_mode = new_inode.i_mode | S_IRWXG;
			new_inode.i_mode = new_inode.i_mode | S_IRWXO;
			
			new_inode.i_uid = getuid();
			new_inode.i_gid = getgid();
			
			new_inode.i_links_count = 1;
			
			new_inode.i_size = size;
			
			new_inode.i_time = time(0);
			new_inode.i_ctime = new_inode.i_time;
			new_inode.i_mtime = new_inode.i_time;
			
			new_inode.i_blocks = num_blocks;
			
			//sb.s_free_inodes_count -= 1;
			sb.s_free_blocks_count -= num_blocks;
			
			//printf("looking for free inode in bitmap\n");
			unsigned char *inode_bitmap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
			fseek(fp, (BLOCK_SIZE * 2), SEEK_SET);
			fread(inode_bitmap, BLOCK_SIZE, 1, fp);
			for (j = 0; j < sb.s_inodes_count; j++) {
				if (1) {
					//printf("inode %d is free\n", j);
					//printf("looking for one free block in memory\n");
					unsigned char *block_bitmap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
					fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
					fread(block_bitmap, BLOCK_SIZE, 1, fp);
					for (k = 0; k < sb.s_blocks_count; k++) {
						if (check_bit(block_bitmap, k) == 0) {
							//printf("block %d is free\n", k);
							
							
							
							//printf("now creating directory entry for this directory in parent\n");
							
							
							possible->d_inode = j;
							possible->d_file_type = 1;
							possible->d_name_len = strlen(found);
							//printf("saving file name length: %d\n", possible->d_name_len);
							strncpy(possible->d_name, found, strlen(found));
							//printf("saving file name: %s\n", possible->d_name);
							
							
							
							//printf("saving this directory entry at position %d\n", entry_to_write_to);
							//fseek(fp, (block * BLOCK_SIZE) + (entry_to_write_to * sizeof(struct directory_entry)), SEEK_SET);
							fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
							fread(free_block, BLOCK_SIZE, 1, fp);
							memcpy(free_block + (entry_to_write_to * sizeof(struct directory_entry)), possible, sizeof(struct directory_entry));
							fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
							fwrite(free_block, BLOCK_SIZE, 1, fp);
							//fwrite(possible, sizeof(struct directory_entry), 1, fp);
							
							//printf("now writing the inode bitmap\n");
							//set_bit(inode_bitmap, j, 1);
							fseek(fp, (BLOCK_SIZE * 2), SEEK_SET);
							fwrite(inode_bitmap, BLOCK_SIZE, 1, fp);
							
							int num_blocks_written = 0;
							
							unsigned char *block_bitmap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
							
							
							//printf("now beginning the file writing process\n");
							int block_to_write_to = 0;
							for (num_blocks_written = 0; num_blocks_written < num_blocks; num_blocks_written++) {
								//printf("writing block %d of %d\n", num_blocks_written + 1, num_blocks);
								if (num_blocks_written == 0) {
									block_to_write_to = k + 8;
									//printf("writing block %d of file to block in memory %d\n", num_blocks_written + 1, k);
								} else {
									for (k = 0; k < sb.s_blocks_count; k++) {
										if (check_bit(block_bitmap, k) == 0) {
											//printf("writing block %d of file to block in memory %d\n", num_blocks_written + 1, k);
											block_to_write_to = k + 8;
											k = sb.s_blocks_count;
										} else {
											//printf("block %d is in use\n", k);
										}
									}
								}
								//printf("writing to block %d\n", block_to_write_to - 8);
								
								new_inode.i_block[num_blocks_written] = block_to_write_to - 8;
								
								unsigned char block_to_write[BLOCK_SIZE] = {0};
								memcpy(block_to_write, data + (BLOCK_SIZE * num_blocks_written), BLOCK_SIZE);
								
								print_bits(block_to_write, BLOCK_SIZE);
								
								fseek(fp, (BLOCK_SIZE * block_to_write_to), SEEK_SET);
								fwrite(block_to_write, BLOCK_SIZE, 1, fp);
								
								//printf("updating the block bitmap\n");
								fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
								fread(block_bitmap, BLOCK_SIZE, 1, fp);
								set_bit(block_bitmap, block_to_write_to - 8, 1);
								fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
								fwrite(block_bitmap, BLOCK_SIZE, 1, fp);
								
								
								
								//printf("successfully wrote block %d\n\n", block_to_write_to - 8);
								
								
							}
							
							//printf("file writing process complete\n");
							
							//printf("now writing inode to disk\n");
							//fseek(fp, (BLOCK_SIZE * 3) + (j * sizeof(struct inode)), SEEK_SET);
							fseek(fp, (BLOCK_SIZE * 3) + ((j / 8) * BLOCK_SIZE), SEEK_SET);
							fread(free_block, BLOCK_SIZE, 1, fp);
							memcpy(free_block + (sizeof(struct inode) * (j % 8)), &new_inode, sizeof(struct inode));
							fseek(fp, (BLOCK_SIZE * 3) + ((j / 8) * BLOCK_SIZE), SEEK_SET);
							fwrite(free_block, BLOCK_SIZE, 1, fp);
						//	fwrite(&new_inode, sizeof(struct inode), 1, fp);
							
							//printf("writing superblock to memory\n");
							fseek(fp, 0, SEEK_SET);
							fwrite(&sb, BLOCK_SIZE, 1, fp);
							
							//printf("file creation process complete\n");
							
							sync_bitmaps();
							return 0;
							
						}
					}
				}
			}
			
			return 0;
		} else {
			//printf("we are not in the directory where we create the file\n");
			
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			int found_child = 0;
			
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				fread(possible, sizeof(struct directory_entry), 1, fp);
				if (possible->d_file_type == 2) {
					//printf("found a directory at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					if (strcmp(name, found) == 0) {
						//printf("match found!\n");
						//printf("switching search_node from %d to %d\n", search_node, possible->d_inode);
						search_node = possible->d_inode;
						found_child = 1;
						i = 10;
						current_sub++;
					}
				}
			}
			if (!found_child) {
				//printf("couldn't file child path\n");
				printf("ERROR: Could not make directory, could not reach path\n");
				errno = EFAULT;
				return -1;
			}
		
			if (depth == num_subs) {
				//printf("reached max depth and found nothing\n");
				errno = EFAULT;
				return -1;
			}
		}
		
	}
	

}

/** Remove a file with the given path.
 */
void rm_file(char *path, unsigned int n)
{
    /*
     * For the file system image that is currently opened.
     * Delete the file in the path provided.
     * Make sure that the data blocks are freed after deleting the file.
     */
	 
	char * found = (char*) malloc(n*sizeof(char));
	char * to_slice = (char*) malloc(n*sizeof(char));
	char * to_slice2 = (char*) malloc(n*sizeof(char));
	
	unsigned char free_block[BLOCK_SIZE] = {0};
	
	char new_directory[n];
	strncpy(new_directory, path, n);
	new_directory[n] = '\0';
	
	//printf("Attempting to delete a file at path - %s\n", new_directory);
	if (n <= 1) {
		printf("ERROR: provided path is too short\n");
		errno = EFAULT;
		return;
	}
	if (new_directory[0] != '/') {
		printf("ERROR: file path should start with /\n");
		errno = EFAULT;
		return;
	}
	if (new_directory[n-1] == '/') {
		printf("ERROR: file path should end not end with /\n");
		errno = EFAULT;
		return;
	}
	
	//printf("checks complete\n");
	//printf("about to slice\n");
	strncpy(to_slice, new_directory + 1, n);
	strncpy(to_slice2, new_directory + 1, n);
	
	to_slice2[n-1] = '\0';
	to_slice2[n-1] = '\0';
	//printf("string is %s\n", to_slice2);
	int search_node = 2;
	
	int num_subs = -1;
	
	while((found = strsep(&to_slice2, "/")) != NULL) {
		if((int)strlen(found) > 57) {
			printf("ERROR: folder names can not be longer than 57 characters\n");
			errno = ENAMETOOLONG;
			return;
		}
		num_subs++;
	}
	
	//printf("depth of file to delete: %d\n", num_subs);
	
	int i = 0;
	int current_sub = 0;
	int block = 0;
	
	//int depth = 1;
	
	struct directory_entry *possible = malloc(sizeof(struct directory_entry));
	
	while((found = strsep(&to_slice, "/")) != NULL) {
		//printf("looking for directory: %s\n", found);
		//printf("directory length: %d\n", (int)strlen(found));
		
		struct inode *searching = malloc(sizeof(struct inode));
		fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
		fread(free_block, BLOCK_SIZE, 1, fp);
		memcpy(searching, free_block + (sizeof(struct inode) * (search_node % 8)), sizeof(struct inode));
		block = searching->i_block[0];
		//printf("looking through block %d\n", block);
		
		if (current_sub ==  num_subs) {
			//printf("we are in the folder where we want to delete the file\n");
			//printf("looking for file\n");
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				//fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				//fread(possible, sizeof(struct directory_entry), 1, fp);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				if (possible->d_file_type == 1) {
					//printf("found a file at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					if (strcmp(name, found) == 0) {
						//printf("match found!\n");
						//printf("this file has inode %d\n", possible->d_inode);
						
						unsigned char blank_block[BLOCK_SIZE] = {0};
						
						struct inode *file_inode = malloc(sizeof(struct inode));
						//fseek(fp, BLOCK_SIZE * 3 + (possible->d_inode * sizeof(struct inode)) , SEEK_SET);
						fseek(fp, BLOCK_SIZE * 3 + ((possible->d_inode / 8) * BLOCK_SIZE), SEEK_SET);
						//fread(file_inode, sizeof(struct inode), 1, fp);
						fread(free_block, BLOCK_SIZE, 1, fp);
						memcpy(file_inode, free_block + ((possible->d_inode % 8) * sizeof(struct inode)), sizeof(struct inode));
						
						if (!(file_inode->i_mode | S_IRUSR)) {
							printf("ERROR: No permission to read file\n");
							errno = EACCES;
							return;
						}
						if (!(file_inode->i_mode | S_IWUSR)) {
							printf("ERROR: No permission to write file\n");
							errno = EACCES;
							return;
						}
						
						//printf("clearing the file's directory entry out\n");
						struct directory_entry p;
						p.d_inode = 0;
						p.d_file_type = 0;
						p.d_name_len = 0;
						memset(p.d_name, 0, 57);
						//fseek(fp, (searching->i_block[0] * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
						fseek(fp, (searching->i_block[0] * BLOCK_SIZE), SEEK_SET);
						fread(free_block, BLOCK_SIZE, 1, fp);
						memcpy(&p, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
						fseek(fp, (searching->i_block[0] * BLOCK_SIZE), SEEK_SET);
						fwrite(free_block, BLOCK_SIZE, 1, fp);
						
						//printf("clearing out this file's data blocks (it has %d)\n", file_inode->i_blocks);
						int clearing_block = 0;
						for (clearing_block = 0; clearing_block < file_inode->i_blocks; clearing_block++) {
						//	printf("inode mem block %d is %d\n", clearing_block, file_inode->i_block[clearing_block]);
						}
						
						searching->i_size -= sizeof(struct directory_entry);
						searching->i_time = time(0);
						searching->i_mtime = searching->i_time;
						
						fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
						fread(free_block, BLOCK_SIZE, 1, fp);
						memcpy(free_block + (sizeof(struct inode) * (search_node % 8)), searching, sizeof(struct inode));
						fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
						fwrite(free_block, BLOCK_SIZE, 1, fp);
						
						if (file_inode->i_links_count > 1) {
							//printf("this file has at least one more hard link, so not deleting yet.\n");
							file_inode->i_links_count -= 1;
							file_inode->i_time = time(0);
							file_inode->i_mtime = file_inode->i_time;
							
							fseek(fp, BLOCK_SIZE * 3 + ((possible->d_inode / 8) * BLOCK_SIZE), SEEK_SET);
							fread(free_block, BLOCK_SIZE, 1, fp);
							//print_bits(free_block, BLOCK_SIZE);
							memcpy(free_block + ((possible->d_inode % 8) * sizeof(struct inode)), file_inode, sizeof(struct inode));
							//print_bits(free_block, BLOCK_SIZE);
							fseek(fp, BLOCK_SIZE * 3 + ((possible->d_inode / 8) * BLOCK_SIZE), SEEK_SET);
							fwrite(free_block, BLOCK_SIZE, 1, fp);
							return;
						}
						
						sb.s_free_blocks_count += file_inode->i_blocks;
						
						
						
						unsigned char *block_bitmap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
						
						for (clearing_block = 0; clearing_block < file_inode->i_blocks; clearing_block++) {
							//printf("clearing out this file's %dth block\n", clearing_block + 1);
							//printf("clearing block in memory %d\n", file_inode->i_block[clearing_block]);
							fseek(fp, (BLOCK_SIZE * 8) + (BLOCK_SIZE * file_inode->i_block[clearing_block]), SEEK_SET);
							fwrite(blank_block, BLOCK_SIZE, 1, fp);
							
							//printf("updating the block bitmap\n");
							fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
							fread(block_bitmap, BLOCK_SIZE, 1, fp);
							set_bit(block_bitmap, file_inode->i_block[clearing_block], 0);
							fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
							fwrite(block_bitmap, BLOCK_SIZE, 1, fp);
							
						}
						
						
						
						//printf("data deletion complete\n");
						

						
						//printf("deleting this file's inode\n");
						
						file_inode->i_mode = 0;
						file_inode->i_uid = 0;
						file_inode->i_gid = 0;
						file_inode->i_links_count = 0;
						file_inode->i_size = 0;
						file_inode->i_time = time(0);
						file_inode->i_mtime = file_inode->i_time;
						file_inode->i_dtime = file_inode->i_time;
						file_inode->i_blocks = 0;
						
						fseek(fp, BLOCK_SIZE * 3 + ((possible->d_inode / 8) * BLOCK_SIZE), SEEK_SET);
						fread(free_block, BLOCK_SIZE, 1, fp);
						//print_bits(free_block, BLOCK_SIZE);
						memcpy(free_block + ((possible->d_inode % 8) * sizeof(struct inode)), file_inode, sizeof(struct inode));
						//print_bits(free_block, BLOCK_SIZE);
						fseek(fp, BLOCK_SIZE * 3 + ((possible->d_inode / 8) * BLOCK_SIZE), SEEK_SET);
						fwrite(free_block, BLOCK_SIZE, 1, fp);
						

						
						for (i = 0; i < 8; i++) {
							file_inode->i_block[i] = 0;
						}
					/*
						fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
						fread(free_block, BLOCK_SIZE, 1, fp);
						memcpy(free_block + (sizeof(struct inode) * (search_node % 8)), searching, sizeof(struct inode));
						fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
						fwrite(free_block, BLOCK_SIZE, 1, fp);
					*/
						//printf("updating superblock\n");
						sb.s_free_inodes_count += 1;
						
						fseek(fp, 0, SEEK_SET);
						fwrite(&sb, BLOCK_SIZE, 1, fp);
						
						//printf("file removal process complete\n");
						
						sync_bitmaps();
						
						return;
					}
				}
			}
			return;
		} else {
			//printf("we are not in the directory where we delete the file\n");
			
			
			int found_child = 0;
			
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				fread(possible, sizeof(struct directory_entry), 1, fp);
				if (possible->d_file_type == 2) {
					//printf("found a directory at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					if (strcmp(name, found) == 0) {
						//printf("match found!\n");
						//printf("switching search_node from %d to %d\n", search_node, possible->d_inode);
						search_node = possible->d_inode;
						found_child = 1;
						i = 10;
						current_sub++;
					}
				}
			}
			if (!found_child) {
				//printf("couldn't file child path\n");
				printf("ERROR: Could delete file, could not reach path\n");
				errno = EFAULT;
				return;
			}
		
			
		}
	}
	
}

/** Read a file with the given path into data.
 */
unsigned int read_file(char *path, unsigned int n, char *data)
{
    /*
     * For the file system image that is currently opened.
     * Read the contents of the file in the path provided.
     * Make sure that the path is valid.
     * Place the file contents in data
     * and return the number of bytes in the file.
     */
	
	char * found = (char*) malloc(n*sizeof(char));
	char * to_slice = (char*) malloc(n*sizeof(char));
	char * to_slice2 = (char*) malloc(n*sizeof(char));
	
	unsigned char free_block[BLOCK_SIZE] = {0};
	
	char new_directory[n];
	strncpy(new_directory, path, n);
	new_directory[n] = '\0';
	
	//printf("Attempting to read a file at path - %s\n", new_directory);
	if (n <= 1) {
		printf("ERROR: provided path is too short\n");
		errno = EFAULT;
		return -1;
	}
	if (new_directory[0] != '/') {
		printf("ERROR: file path should start with /\n");
		errno = EFAULT;
		return -1;
	}
	if (new_directory[n-1] == '/') {
		printf("ERROR: file path should end not end with /\n");
		errno = EFAULT;
		return -1;
	}
	
	//printf("checks complete\n");
	//printf("about to slice\n");
	strncpy(to_slice, new_directory + 1, n);
	strncpy(to_slice2, new_directory + 1, n);
	
	to_slice2[n-1] = '\0';
	to_slice2[n-1] = '\0';
	//printf("string is %s\n", to_slice2);
	int search_node = 2;
	
	int num_subs = -1;
	
	while((found = strsep(&to_slice2, "/")) != NULL) {
		if((int)strlen(found) > 57) {
			printf("ERROR: folder/file names can not be longer than 57 characters\n");
			errno = ENAMETOOLONG;
			return -1;
		}
		num_subs++;
	}
	
	//printf("depth of file to read: %d\n", num_subs);
	
	int i = 0;
	int current_sub = 0;
	int block = 0;
	
	int depth = 1;
	
	struct directory_entry *possible = malloc(sizeof(struct directory_entry));
	
	while((found = strsep(&to_slice, "/")) != NULL) {
		//printf("looking for directory: %s\n", found);
		//printf("directory length: %d\n", (int)strlen(found));
		
		struct inode *searching = malloc(sizeof(struct inode));
		fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
		fread(free_block, BLOCK_SIZE, 1, fp);
		memcpy(searching, free_block + (sizeof(struct inode) * (search_node % 8)), sizeof(struct inode));
		block = searching->i_block[0];
		//printf("looking through block %d\n", block);
		
		if (current_sub ==  num_subs) {
			//printf("we are in the folder where we want to read the file\n");
			//printf("looking for file\n");
			for (i = 0; i < 8; i++) {
			//	printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				if (possible->d_file_type == 1) {
					//printf("found a file at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					
					if (strcmp(name, found) == 0) {
						//printf("match found!\n");
						//printf("this file has inode %d\n", possible->d_inode);
						
						struct inode *file_inode = malloc(sizeof(struct inode));
						fseek(fp, BLOCK_SIZE * 3 + ((possible->d_inode / 8) * BLOCK_SIZE), SEEK_SET);
						fread(free_block, BLOCK_SIZE, 1, fp);
						memcpy(file_inode, free_block + ((possible->d_inode % 8) * sizeof(struct inode)), sizeof(struct inode));
						
						if (!(file_inode->i_mode | S_IRUSR)) {
							printf("ERROR: No permission to read file\n");
							errno = EACCES;
							return -1;
						}
						//printf("this file is %d bytes large\n", file_inode->i_size);
						//printf("writing out this file's data blocks (it has %d)\n", file_inode->i_blocks);
						
						unsigned char block_to_write[BLOCK_SIZE] = {0};
						
						
						
						int reading_block = 0;
						for (reading_block = 0; reading_block < file_inode->i_blocks; reading_block++) {
							//printf("inode mem block %d is %d\n", reading_block, file_inode->i_block[reading_block]);
						}
						
						for (reading_block = 0; reading_block < file_inode->i_blocks; reading_block++) {
							//printf("writing out this file's %dth block\n", reading_block + 1);
							//printf("reading block in memory %d\n", file_inode->i_block[reading_block]);
							fseek(fp, (BLOCK_SIZE * 8) + (BLOCK_SIZE * file_inode->i_block[reading_block]), SEEK_SET);
							fread(block_to_write, BLOCK_SIZE, 1, fp);
							if (reading_block + 1 < file_inode->i_blocks) {
								//printf("writing 512 bytes\n");
								memcpy(data + (BLOCK_SIZE * reading_block), block_to_write, BLOCK_SIZE);
							} else {
								//printf("writing %d bytes\n", file_inode->i_size - ((file_inode->i_blocks - 1) * BLOCK_SIZE));
								memcpy(data + (BLOCK_SIZE * reading_block), block_to_write, file_inode->i_size - ((file_inode->i_blocks - 1) * BLOCK_SIZE));
							}
							
						}
						//printf("finished file reading process\n");
						sync_bitmaps();
						searching->i_time = time(0);
						fseek(fp, BLOCK_SIZE * 3 + ((search_node / 8) * BLOCK_SIZE) , SEEK_SET);
						fread(free_block, BLOCK_SIZE, 1, fp);
						memcpy(free_block + (sizeof(struct inode) * (search_node % 8)), searching, sizeof(struct inode));
						fseek(fp, BLOCK_SIZE * 3 + ((search_node / 8) * BLOCK_SIZE) , SEEK_SET);
						fwrite(free_block, BLOCK_SIZE, 1, fp);
						
						file_inode->i_time = time(0);
						fseek(fp, BLOCK_SIZE * 3 + ((possible->d_inode / 8) * BLOCK_SIZE), SEEK_SET);
						fread(free_block, BLOCK_SIZE, 1, fp);
						memcpy(free_block + (sizeof(struct inode) * (possible->d_inode % 8)), file_inode, sizeof(struct inode));
						fseek(fp, BLOCK_SIZE * 3 + ((possible->d_inode / 8) * BLOCK_SIZE) , SEEK_SET);
						fwrite(free_block, BLOCK_SIZE, 1, fp);
						
						return(searching->i_size);
					}
					
				}
			}
			return 0;
		} else {
			//printf("we are not in the directory where we read the file\n");
			
			
			
			int found_child = 0;
			
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				fread(possible, sizeof(struct directory_entry), 1, fp);
				if (possible->d_file_type == 2) {
					//printf("found a directory at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					if (strcmp(name, found) == 0) {
						//printf("match found!\n");
						//printf("switching search_node from %d to %d\n", search_node, possible->d_inode);
						search_node = possible->d_inode;
						found_child = 1;
						i = 10;
						current_sub++;
					}
				}
			}
			if (!found_child) {
				//printf("couldn't file child path\n");
				printf("ERROR: Could delete file, could not reach path\n");
				errno = EFAULT;
				return -1;
			}
		
			if (depth == num_subs) {
				//printf("reached max depth and found nothing\n");
				errno = EFAULT;
				return -1;
			}			
			
			
		}
		
	}
	return 0;
}

/** Create a hard link to the given target at the given path.
 */
void make_link(char *path, unsigned int n, char *target)
{
    /*
     * make a new hard link in the path to target
     * make sure that the path and target are both valid.
     */
	 
	unsigned char free_block[BLOCK_SIZE] = {0};
	
	char * found = (char*) malloc(n*sizeof(char));
	char * to_slice = (char*) malloc(n*sizeof(char));
	char * to_slice2 = (char*) malloc(n*sizeof(char));
	
	char * t_found = (char*) malloc(n*sizeof(char));
	char * t_to_slice = (char*) malloc(n*sizeof(char));
	char * t_to_slice2 = (char*) malloc(n*sizeof(char));
	
	int m = strlen(target);
	
	char new_directory[n];
	char target_directory[m + 1];
	
	//printf("path is %s\n", path);
	
	strncpy(new_directory, path, n);
	//printf("path is %s\n", new_directory);
	strncpy(target_directory, target, m);
	new_directory[n] = '\0';
	target_directory[m] = '\0';
	//printf("path is %s\n", new_directory);
	//target_directory[m] = '\0';
	//printf("path is %s\n", new_directory);
	
	//printf("Attempting to make a hard link at path - %s\n", new_directory);
	//printf("Attempting to make a hard link from path - %s\n", target_directory);
	
	if (n <= 1) {
		printf("ERROR: provided path is too short\n");
		exit(1);
	}
	if (m <= 1) {
		printf("ERROR: provided path is too short\n");
		exit(1);
	}
	if (new_directory[0] != '/') {
		printf("ERROR: file path should start with /\n");
		exit(1);
	}
	if (target_directory[0] != '/') {
		printf("ERROR: file path should start with /\n");
		exit(1);
	}
	if (new_directory[n-1] == '/') {
		printf("ERROR: file path should not end with /\n");
		exit(1);
	}
	if (new_directory[n-1] == '/') {
		printf("ERROR: target path should not end with /\n");
		exit(1);
	}
	
	//printf("checks complete\n");
	//printf("about to slice\n");
	strncpy(to_slice, new_directory + 1, n);
	strncpy(to_slice2, new_directory + 1, n);
	strncpy(t_to_slice, target_directory + 1, m);
	strncpy(t_to_slice2, target_directory + 1, m);
	
	to_slice[n-1] = '\0';
	to_slice2[n-1] = '\0';
	t_to_slice[m-1] = '\0';
	t_to_slice2[m-1] = '\0';
	//printf("path string is %s\n", to_slice);
	//printf("target string is %s\n", t_to_slice);
	int search_node = 2;
	
	int num_subs = -1;
	int t_num_subs = -1;
	
	while((found = strsep(&to_slice2, "/")) != NULL) {
		if((int)strlen(found) > 57) {
			printf("ERROR: folder names can not be longer than 57 characters\n");
			exit(1);
		}
		num_subs++;
	}
	
	while((t_found = strsep(&t_to_slice2, "/")) != NULL) {
		if((int)strlen(t_found) > 57) {
			printf("ERROR: folder names can not be longer than 57 characters\n");
			exit(1);
		}
		t_num_subs++;
	}
	
	//printf("depth of path: %d\n", num_subs);
	//printf("depth of target: %d\n", t_num_subs);
	
	int i = 0;
	int current_sub = 0;
	int block = 0;
	
	//int depth = 1;
	
	int target_inode = -1;
	//int target_name_len = 0;
	char target_name[57];
	
	struct directory_entry *possible = malloc(sizeof(struct directory_entry));
	
	while((t_found = strsep(&t_to_slice, "/")) != NULL && target_inode == -1) {
		//printf("looking for target directory: %s\n", t_found);
		//printf("directory length: %d\n", (int)strlen(t_found));
		
		struct inode *searching = malloc(sizeof(struct inode));
		fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
		fread(free_block, BLOCK_SIZE, 1, fp);
		memcpy(searching, free_block + (sizeof(struct inode) * (search_node % 8)), sizeof(struct inode));
		block = searching->i_block[0];
		//printf("looking through block %d\n", block);
		if (current_sub ==  t_num_subs) {
			//printf("we are in the folder where we want to read the file\n");
			//printf("looking for file\n");
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				if (possible->d_file_type == 1) {
				//	printf("found a file at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					
					if (strcmp(name, t_found) == 0) {
						//printf("match found!\n");
						//printf("this file has inode %d\n", possible->d_inode);
						struct inode *file_inode = malloc(sizeof(struct inode));
						fseek(fp, BLOCK_SIZE * 3 + ((possible->d_inode / 8) * BLOCK_SIZE), SEEK_SET);
						fread(free_block, BLOCK_SIZE, 1, fp);
						memcpy(file_inode, free_block + ((possible->d_inode % 8) * sizeof(struct inode)), sizeof(struct inode));
						
						if (!(file_inode->i_mode | S_IRUSR)) {
							printf("ERROR: No permission to read file\n");
							exit(1);
						}
						
						target_inode = possible->d_inode;
						//target_name_len = possible->d_name_len;
						memcpy(target_name, possible->d_name, 57);
						
						
					}
				}
			}
		} else {
			//printf("we are not in the directory where we read the file\n");
			
			
			
			int found_child = 0;
			
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				fread(possible, sizeof(struct directory_entry), 1, fp);
				if (possible->d_file_type == 2) {
					//printf("found a directory at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					//printf("f\n");
					if (strcmp(name, t_found) == 0) {
						//printf("match found!\n");
						//printf("switching search_node from %d to %d\n", search_node, possible->d_inode);
						search_node = possible->d_inode;
						found_child = 1;
						i = 10;
						current_sub++;
					}
				}
			}
			if (!found_child) {
				//printf("couldn't file child path\n");
				printf("ERROR: Could create hard link, could not reach path\n");
				exit(1);
			}
		
						
			
			
		}
		
	}
	
	search_node = 2;
	i = 0;
	current_sub = 0;
	block = 0;
	
	//depth = 1;
	
	while((found = strsep(&to_slice, "/")) != NULL) {
		//printf("looking for directory: %s\n", found);
	//	printf("directory length: %d\n", (int)strlen(found));
		
		struct inode *searching = malloc(sizeof(struct inode));
		fseek(fp, (BLOCK_SIZE * 3) + ((search_node / 8) * BLOCK_SIZE), SEEK_SET);
		fread(free_block, BLOCK_SIZE, 1, fp);
		memcpy(searching, free_block + (sizeof(struct inode) * (search_node % 8)), sizeof(struct inode));
		block = searching->i_block[0];
		//printf("looking through block %d\n", block);
		
		if (current_sub ==  num_subs) {
			//printf("we are in the folder where we want to create the hard link\n");
			
			//printf("looking for a free directory entry\n");
			
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			int entry_to_write_to = -1;
			for (i = 0; i < 8; i++) {
				//fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
				fread(free_block, BLOCK_SIZE, 1, fp);
				memcpy(possible, free_block + (i * sizeof(struct directory_entry)), sizeof(struct directory_entry));
				//fread(possible, sizeof(struct directory_entry), 1, fp);
				if (possible->d_file_type == 0) {
					//printf("entry %d is free\n", i);
					entry_to_write_to = i;
					i = 8;
				}
				
			}
			
			if (entry_to_write_to == -1) {
				//printf("ERROR: This folder is full\n");
				exit(1);
			}
			
			searching->i_size += sizeof(struct directory_entry);
			searching->i_mtime = time(0);
			searching->i_time = searching->i_mtime;
			
			//printf("grabbing the inode for the file to hard link\n");
			struct inode *file_inode = malloc(sizeof(struct inode));
			fseek(fp, BLOCK_SIZE * 3 + ((target_inode / 8) * BLOCK_SIZE), SEEK_SET);
			fread(free_block, BLOCK_SIZE, 1, fp);
			memcpy(file_inode, free_block + ((target_inode % 8) * sizeof(struct inode)), sizeof(struct inode));
			
			possible->d_inode = target_inode;
			possible->d_file_type = 1;
			possible->d_name_len = strlen(found);
			memset(possible->d_name, 0, 57);
			memcpy(possible->d_name, found, strlen(found));
			
			file_inode->i_mtime = time(0);
			file_inode->i_time = file_inode->i_mtime;
			file_inode->i_links_count += 1;
			
			file_inode->i_time = time(0);
			fseek(fp, BLOCK_SIZE * 3 + ((target_inode / 8) * BLOCK_SIZE), SEEK_SET);
			fread(free_block, BLOCK_SIZE, 1, fp);
			memcpy(free_block + (sizeof(struct inode) * (target_inode % 8)), file_inode, sizeof(struct inode));
			fseek(fp, BLOCK_SIZE * 3 + ((target_inode / 8) * BLOCK_SIZE) , SEEK_SET);
			fwrite(free_block, BLOCK_SIZE, 1, fp);
			
			fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
			fread(free_block, BLOCK_SIZE, 1, fp);
			print_bits(free_block, BLOCK_SIZE);
			print_bits(possible, sizeof(struct directory_entry));
			//printf("i is %d\n", entry_to_write_to);
			memcpy(free_block + (entry_to_write_to * sizeof(struct directory_entry)), possible, sizeof(struct directory_entry));
			print_bits(free_block, BLOCK_SIZE);
			fseek(fp, (block * BLOCK_SIZE), SEEK_SET);
			fwrite(free_block, BLOCK_SIZE, 1, fp);
			
			return;
			
			
		} else {
		//	printf("we are not in the directory where we create the hard link\n");
			
			struct directory_entry *possible = malloc(sizeof(struct directory_entry));
			int found_child = 0;
			
			for (i = 0; i < 8; i++) {
				//printf("looking through potential directory location %d...\n\n", i);
				fseek(fp, (block * BLOCK_SIZE) + (i * sizeof(struct directory_entry)) , SEEK_SET);
				fread(possible, sizeof(struct directory_entry), 1, fp);
				if (possible->d_file_type == 2) {
					//printf("found a directory at entry[%d]\n", i);
					char * name = (char*) malloc(57*sizeof(char));
					strncpy(name, possible->d_name, possible->d_name_len);
					name[possible->d_name_len] = '\0';
					//printf("its name is %s\n", name);
					if (strcmp(name, found) == 0) {
						//printf("match found!\n");
						//printf("switching search_node from %d to %d\n", search_node, possible->d_inode);
						search_node = possible->d_inode;
						found_child = 1;
						i = 10;
						current_sub++;
					}
				}
			}
			if (!found_child) {
				//printf("couldn't file child path\n");
				printf("ERROR: Could not make hard link, could not reach path\n");
				exit(1);
			}
		
			
		}
		
	}
}

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define BBLACK     	   "\x1b[40m"

/** Dump memory, including in binary format
 */
void print_bits(void *ptr, int size) 
{
	
	return;
	
	
	printf("\nPrinting %d bytes (%d bits) starting at %p\n\n", size, size * 8, ptr);
	//printf("x");
    unsigned char *p = ptr;
	//printf("x");
	unsigned char byte;
    int i = 0; int j = 0; int k = 0; int l = 0; int m = 0; int n = 0;
	
	if (size > 512) {
		printf("[BLOCK 0]\n\n");
	}
	
    for (i = 0; i < size; i++) {
				
		if (l == BLOCK_SIZE && i < size) {
			m++;
			printf("\n[BLOCK %d]\n\n", m);
			l = 0;
		}
		if (j == 0) {
			
			printf(ANSI_COLOR_RED BBLACK "%p		" ANSI_COLOR_RESET, &p[i]);
		}
		printf("%02hhX [", p[i]);
		
        for (k = 7; k >= 0; k--) {
            byte = (p[i] >> k) & 1;
			if (byte) {
				printf(ANSI_COLOR_CYAN BBLACK);
			} else {
				printf(ANSI_COLOR_MAGENTA BBLACK);
			}
            printf("%u", byte);
        }
		printf(ANSI_COLOR_RESET "] ");
		j++;
		if (j == 8) {
			printf("\n");
			j = 0;
		}
		l++;
		
		n++;
		if (n == 64) {
			printf("\n");
			n = 0;
		}

    }
    printf("\n");
}

/** Set a specific bit in given memory. Starts at LSB.
 */
void set_bit(void *ptr, int pos, int value)
{
	int byte_offset = pos / 8;
	int bit_offset = pos % 8;
	unsigned char *p = ptr;
	//unsigned char byte;
	
	//printf("Setting the %dth bit in byte %d <%p> to %d", bit_offset, byte_offset, &p[byte_offset], value);
	//printf(" --- %02hhX [", p[byte_offset]);
	/*
	int k = 0;
	for (k = 7; k >= 0; k--)
    {
		byte = (p[byte_offset] >> k) & 1;
		printf("%u", byte);
    }
	*/
	//printf("] to ");
	
	//setting bit now
	
	//int real_bit_offset = 7-bit_offset;
	int real_bit_offset = bit_offset;
	
	p[byte_offset] ^= (-value ^ p[byte_offset]) & (1UL << real_bit_offset);
	//printf("%02hhX [", p[byte_offset]);
	/*
	for (k = 7; k >= 0; k--)
    {
		byte = (p[byte_offset] >> k) & 1;
		//printf("%u", byte);
    }
	*/
	//printf("]\n");
	
}

/** Return the value of a bit in given memory. Starts at LSB.
 */
int check_bit(void *ptr, int pos)
{
	int byte_offset = pos / 8;
	int bit_offset = pos % 8;
	unsigned char *p = ptr;
	unsigned char byte;
	
	//printf("Checking the %dth bit in byte %d <%p>", bit_offset, byte_offset, &p[byte_offset]);
	
	//int real_bit_offset = 7-bit_offset;
	int real_bit_offset = bit_offset;
	
	byte = (p[byte_offset] >> real_bit_offset) & 1;
	
	if (byte) {
		//printf(" --- it is 1\n");
		return 1;
	} else {
		//printf(" --- it is 0\n");
		return 0;
	}
	
}

/** Write the local copies of the inode and block bitmaps into the filesystem.
 */
void sync_bitmaps()
{
	//printf("\nSYNCING BITMAPS\n\n");
	
	unsigned char *bitmap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
	
	fseek(fp, (BLOCK_SIZE * 1), SEEK_SET);
	fread(bitmap, BLOCK_SIZE, 1, fp);
	memcpy(block_bm, bitmap, BLOCK_SIZE);
	
	fseek(fp, (BLOCK_SIZE * 2), SEEK_SET);
	fread(bitmap, BLOCK_SIZE, 1, fp);
	memcpy(inode_bm, bitmap, BLOCK_SIZE);
	/*
	printf("\nblock_bm in memory:\n");
	print_bits(block_bm, BLOCK_SIZE / 4);
	
	printf("\ninode_bm in memory:\n");
	print_bits(inode_bm, BLOCK_SIZE / 4);
	*/
	/*
	int i = 0;
	for (i = 0; i < sb.s_inodes_count; i++) {
		if (check_bit(inode_bm, i) == 0) {
			//printf("first free inode is %d\n", i);
			sb.s_first_ino = i;
			i = sb.s_inodes_count;
		}
	}
	for (i = 0; i < sb.s_blocks_count; i++) {
		if (check_bit(block_bm, i) == 0) {
			//printf("first free block is %d\n", i);
			sb.s_first_data_block = i;
			i = sb.s_blocks_count;
		}
	}
	*/
	
	sb.s_first_ino = 2; sb.s_first_data_block = 0;
	
	fseek(fp, 0, SEEK_SET);
	fwrite(&sb, BLOCK_SIZE, 1, fp);
	print_superblock();
}

/** Print out the state of the filesystem's superblock.
 */
void print_superblock()
{
	return;
	printf("\nSUPERBLOCK:\n\n");
	
	printf("Total number of inodes: %d\n", sb.s_inodes_count);
	printf("Total number of blocks: %d\n", sb.s_blocks_count);
	printf("Total number of free inodes: %d\n", sb.s_free_inodes_count);
	printf("Total number of free blocks: %d\n", sb.s_free_blocks_count);
	printf("First data block: %d\n", sb.s_first_data_block);
	printf("First inode: %d\n\n", sb.s_first_ino);
	
	
	
}