#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <vsd_ioctl.h>

#include "vsd_device.h"

int vsd_descriptor = -1;

int vsd_init()
{
    vsd_descriptor = open("/dev/vsd", O_RDWR);
    return vsd_descriptor > 0 ? 0 : -1;
}

int vsd_deinit()
{
    return close(vsd_descriptor);
}

int vsd_get_size(size_t *out_size)
{
    vsd_ioctl_get_size_arg_t arg;

    if (ioctl(vsd_descriptor, VSD_IOCTL_GET_SIZE, &arg))
        return -1;

    *out_size = arg.size;
    return 0;
}

int vsd_set_size(size_t size)
{
    vsd_ioctl_set_size_arg_t arg = {
        .size = size
    };

    if (ioctl(vsd_descriptor, VSD_IOCTL_SET_SIZE, &arg))
        return -1;

    return 0;
}

ssize_t vsd_read(char* dst, off_t offset, size_t size)
{
    if (lseek(vsd_descriptor, offset, SEEK_SET) == -1)
        return -1;

    return read(vsd_descriptor, dst, size);
}

ssize_t vsd_write(const char* src, off_t offset, size_t size)
{
    if (lseek(vsd_descriptor, offset, SEEK_SET) == -1)
        return -1;

    return write(vsd_descriptor, src, size);
}

void* vsd_mmap(size_t offset)
{
    size_t len;
    int page_size = getpagesize();

    if (offset % page_size)
    {
        errno = EINVAL;
        return MAP_FAILED;
    }

    len = get_mmap_len(offset);
    if (len == -1)
        return NULL;

    return mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, vsd_descriptor, offset);
}

int vsd_munmap(void* addr, size_t offset)
{
    size_t len;
    int page_size = getpagesize();

    if (offset % page_size || (size_t)addr % page_size)
    {
        errno = EINVAL;
        return -1;
    }

    len = get_mmap_len(offset);
    if (len == -1)
        return -1;

    return munmap(addr, len);
}

int get_mmap_len(size_t offset)
{
    size_t vsd_size;

    if (vsd_get_size(&vsd_size))
        return -1;

    return vsd_size - offset;
}
