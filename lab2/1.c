#include <stdio.h>
#include <pthread.h>

int func_copy(char* src_filename, char* dst_filename);

// data structure to hold copy
struct copy_struct {
	int thread_id; 
	char* src_filename;
	char* dst_filename; 
};

void* copy_thread(void* arg) {
    struct copy_struct *params = (struct copy_struct*)arg;

    func_copy(params->src_filename, params->dst_filename);

    pthread_exit(NULL);
}


int main(int argc, char* argv[]) {

    if (argc < 3 || (argc % 2) == 0) {
        fprintf(stderr,
            "usage: %s <src1> <dst1> [<src2> <dst2> ...]\n",
            argv[0]);
        return 1;
    }

    int num_threads = (argc - 1) / 2;

    pthread_t threads[num_threads];
    struct copy_struct thread_params[num_threads];

    // create one thread per file
    for (int i = 0; i < num_threads; i++) {
        thread_params[i].thread_id = i;
        thread_params[i].src_filename = argv[1 + 2*i];
        thread_params[i].dst_filename = argv[1 + 2*i + 1];

        pthread_create(&threads[i], NULL, copy_thread, &thread_params[i]);
    }

    // wait for all threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
