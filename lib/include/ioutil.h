#ifndef __IOUTIL__
#define __IOUTIL__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int write_buffer(const char *path, void *buf, size_t buf_size, off_t offset);
int read_buffer(const char *path, void *buf, size_t buf_size, off_t offset);
int clear_file(const char *path);

#endif