#include <stdio.h>

int syscall_copy(char* src_filename, char* dst_filename);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <src> <dst>\n", argv[0]);
        return 1;
    }
    return syscall_copy(argv[1], argv[2]);
}
