[1]

There are 40 inodes in the filesystem.

1 is the root folder, and 2 are reserved. So, the system starts with 37 free inodes.

In a scenario in which we want as many files as possible, let the root folder be the parent
of 6 additional folders.

That leaves 31 free inodes left for files, to be divided into root's subfolders.

So, we could at maximum have 31 files in the filesystem.

[2]

The largest possible file in this filesystem would take up 8 * 512 bytes, or 4096 bytes.

[3]

A directory can have 6 entries. There is space in a directory for 8, and 2 are reserved.

[4]

This simple filesystem is faulty when experiencing multiple concurrent disk accesses. There
isn't guarantee that files and directories will be accessible and modifyable throughout the
access if other users can simultaneuosly access the system.

