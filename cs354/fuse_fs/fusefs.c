/*
  FUSE: Filesystem in Userspace
  This skeleton code is made from the function prototypes found in
  /usr/include/fuse/fuse.h Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
  His code is licensed under the LGPL v2.
*/

#include "fusefs.h"


/**
  Mount the filesystem initially
*/
static void *sfs_mount(struct fuse_conn_info *conn) 
{
	
	
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("current directory is: %s\n", cwd);
	printf("length of that is %d\n", (int)strlen(cwd));
	strcat(cwd, "/simpleFS");
	printf("looking for file: %s\n", cwd);
	printf("length of that is %d\n", (int)strlen(cwd));
	
	open_filesystem(cwd, (int)strlen(cwd));
	

	
}

/**
  Close the file pointer
*/
static void sfs_unmount (void *private_data) 
{
	printf("closing file system...\n");
	fclose(fp);
	printf("file system closed\n");
}

/**
  Get the attributes of a given file
*/
static int sfs_getattr(const char *path, struct stat *stbuf)
{
	errno = 0;
	printf("getattr on path: %s\n", path);
	printf("errno is %d\n", errno);
	char * path2;
	strcpy(path2, path);
	read_stats(path2, strlen(path2), stbuf);
	printf("errno is %d\n", errno);
	if (errno != 0) {
		return -errno;
	}
}

/**
  Make a new directory
*/
static int sfs_mkdir(const char *path, mode_t mode)
{
	errno = 0;
	char * path2;
	strcpy(path2, path);
    make_directory(path2, strlen(path2));
	if (errno != 0) {
		return -errno;
	}
	return 0;
}

/**
  Read a directory into a buffer using the filler function
*/
static int sfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	errno = 0;
	printf("sfs_readdir path: %s\n", path);
	char * path2;
	strcpy(path2, path);
	read_directory_fuse(path2, strlen(path2), buf, filler);
	printf("done reading\n");
	if (errno != 0) {
		return -errno;
	}
	return 0;
}

/**
  Create a file
*/
static int sfs_create(const char *path, mode_t mode, dev_t rdev)
{
	errno = 0;
	char * path2;
	strcpy(path2, path);
	create_file(path2, strlen(path2), 0, NULL);
	if (errno != 0) {
		return -errno;
	}
	return 0;
}

/**
  Read a file into a buffer
*/
static int sfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	errno = 0;
	char * path2;
	strcpy(path2, path);
	int n = read_file(path2, strlen(path2), buf);
	if (errno != 0) {
		return -errno;
	}
	return n;
}

/**
  Write a buffer into a file
*/
static int sfs_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi)
{
	errno = 0;
	char * path2;
	strcpy(path2, path);
	char * buf2;
	strcpy(buf2, buf);
	
	int n = fill_file(path2, strlen(path2), size, buf2);
	if (errno != 0) {
		return -errno;
	}
	return n;
}

/**
  Delete a directory recursively
*/
static int sfs_remove_dir(const char *path) 
{
	errno = 0;
	char * path2;
	strcpy(path2, path);
	rm_directory(path2, strlen(path2));
	if (errno != 0) {
		return -errno;
	}
}

/**
  Delete a file
*/
static int sfs_delete(const char *path) 
{
	errno = 0;
	char * path2;
	strcpy(path2, path);
	rm_file(path2, strlen(path));
	if (errno != 0) {
		return -errno;
	}
}

/**
  Create a symlink
*/
static int sfs_symlink(const char *from, const char *to) 
{
	errno = 0;
	char * from2;
	strcpy(from2, from);
	char * to2;
	strcpy(to2, to);
	make_link(from2, strlen(from2), to2);
	if (errno != 0) {
		return -errno;
	}
}

/**
  Read a symlink
*/
static int sfs_readlink(const char *path, char *buf, size_t size)
{
}



static struct fuse_operations sfs_oper = 
{
    .init    = sfs_mount,
    .destroy = sfs_unmount,
    .getattr = sfs_getattr,
    .mkdir	 = sfs_mkdir,
    .readdir = sfs_readdir,
    .symlink = sfs_symlink,
    .readlink = sfs_readlink,
    .mknod	 = sfs_create,
    .read	 = sfs_read,
    .write	 = sfs_write,
    .unlink	 = sfs_delete,
    .rmdir   = sfs_remove_dir,
};

int main(int argc, char *argv[])
{
    umask(0); 
    return fuse_main(argc, argv, &sfs_oper, NULL);
}

