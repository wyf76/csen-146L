#include <stdio.h>

void check_copy_times(char* src_filename, char* dst_filename);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <src> <dst>\n", argv[0]);
        return 1;
    }
    check_copy_times(argv[1], argv[2]);
    return 0;
}
