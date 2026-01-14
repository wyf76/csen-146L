#include <stdio.h>
#include <pthread.h>

int syscall_copy(char* src_filename, char* dst_filename);

void* thread_copy(void *arg) {
    char **files = (char**)arg;
    syscall_copy(files[0], files[1]);
    return NULL;
}

int main(int argc, char* argv[]) {

    if (argc < 3 || (argc % 2) == 0) {
        fprintf(stderr,
            "usage: %s <src1> <dst1> [<src2> <dst2> ...]\n",
            argv[0]);
        return 1;
    }

    int n = (argc - 1) / 2;
    pthread_t threads[n];

    // create one thread per file
    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL,
                        thread_copy,
                        &argv[1 + 2*i]);
    }

    // wait for all threads
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}