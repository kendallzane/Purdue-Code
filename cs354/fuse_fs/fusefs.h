#define FUSE_USE_VERSION 26

#ifdef linux
#define _XOPEN_SOURCE 500
#endif


#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include <stdint.h>
#include <sys/statfs.h>
#include <stdbool.h>
#include <stdlib.h>

#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stddef.h>

#include "simpleFS.h"


#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif



int errno; 

extern unsigned int INO_SIZE;
extern unsigned int DIR_ENTRY_SIZE;

