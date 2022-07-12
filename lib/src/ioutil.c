#include "ioutil.h"

int write_buffer(const char *path, void *buf, size_t buf_size, off_t offset) {
    if (!path || !buf || buf_size < 0) return EINVAL;
    int fd = open(path, O_WRONLY);
    if (fd < 0) return errno;
    pwrite(fd, buf, buf_size, offset);
    close(fd);
    return 0;
}

int read_buffer(const char *path, void *buf, size_t buf_size, off_t offset) {
    if (!path || !buf || buf_size < 0) return EINVAL;
    int fd = open(path, O_RDONLY);
    if (fd < 0) return errno;
    pread(fd, buf, buf_size, offset);
    close(fd);
    return 0;
}

int clear_file(const char *path) {
    return truncate(path, 0);
}
