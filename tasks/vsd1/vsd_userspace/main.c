/*
 * TODO parse command line arguments and call proper
 * VSD_IOCTL_* using C function ioctl (see man ioctl).
 */
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

 #include "vsd_ioctl.h"

void print_usage() {
    printf("Usage: ./vsd_userspace size_get\n");
    printf("       ./vsd_userspace size_set SIZE_IN_BYTES\n");
}

int get_size(int vsd_descriptor) {
    int ret = EXIT_FAILURE;
    vsd_ioctl_get_size_arg_t arg;
    if (ioctl(vsd_descriptor, VSD_IOCTL_GET_SIZE, &arg) != -1) {
        printf("virtual storage device size: %zu\n", arg.size);
        ret = EXIT_SUCCESS;
    } else {
        perror("ioctl");
    }
    return ret;
}

int set_size(int vsd_descriptor, int size) {
    int ret = EXIT_FAILURE;
    vsd_ioctl_set_size_arg_t arg = {
        .size = size
    };

    if (ioctl(vsd_descriptor, VSD_IOCTL_SET_SIZE, &arg) != -1) {
        ret = EXIT_SUCCESS;
    } else {
        perror("ioctl");
    }
    return ret;
}


int main(int argc, char **argv) {
    int ret = EXIT_FAILURE;
    int vsd_descriptor;

    if (argc != 2 && argc != 3) {
        print_usage();
        goto exit;
    }

    vsd_descriptor = open("/dev/vsd", O_RDWR);
    if (vsd_descriptor == - 1) {
        perror("open");
        goto exit;
    }

    if (!strcmp("size_get", argv[1])) {
        ret = get_size(vsd_descriptor);
    } else if (!strcmp("size_set", argv[1])) {
        int size = atoi(argv[2]);
        if (size <= 0) {
            print_usage();
            goto err;
        }
        ret = set_size(vsd_descriptor, size);
    } else {
        print_usage();
        goto err;
    }

err:
    close(vsd_descriptor);
exit:
    return ret;
}
