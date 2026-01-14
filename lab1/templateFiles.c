// COEN 146L : Lab 1 - template to use for file transfer (steps 1, 2, and 3)
// You need to adapt this template according to the requirements of each of the steps
/*
Name: Yufan Weng
Date: Jan 7, 2026
Title: Lab 1 - File Copying using Functions and Syscalls
Description: This program copies a file from a source path to a destination path using two methods—(1) C standard I/O functions 
(fopen, fread, fwrite) and (2) UNIX system calls (open, read, write)—and measures the CPU time of each method using clock().
*/

#include <stdio.h>   // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h>  // malloc(), free()
#include <pthread.h> // pthread_create()
#include <unistd.h>  // read(), write()
#include <fcntl.h>   // open(), close()
#include <errno.h>   // errno
#include <time.h>    // clock()
#include <sys/mman.h> // mmap()

#define BUF_SIZE 2048 //buffer size

// copies a files from src_filename to dst_filename using functions fopen(), fread(), fwrite(), fclose()
int func_copy(char* src_filename, char* dst_filename) {
	FILE *src, *dst;
	src = fopen(src_filename, "r");	// opens a file for reading
	if (src == NULL) { // fopen() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
		exit(0);
	}
	dst = fopen(dst_filename, "w");	// opens a file for writing; erases old file/creates a new file
	if (dst == NULL) { // fopen() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
		exit(0);
	}

	unsigned char *buf = (unsigned char*)malloc((size_t)BUF_SIZE);  // allocate a buffer to store read data
	// reads content of file is loop iterations until end of file
	while(1){
		size_t bytes_read = fread(buf, 1, BUF_SIZE, src); // reads up to BUF_SIZE bytes from src_filename
		if (bytes_read == 0) break; // check for end of file
    		fwrite(buf, 1, bytes_read, dst);
	}
	free(buf);
	fclose(src);
	fclose(dst);
	return 0;	
	// writes bytes_read to dst_filename 
	
	// closes src file pointer
	// closes dst file pointer
	// frees memory used for buf
	
}

// copies a files form src_filename to dst_filename using syscalls open(), read(), write(), close()
int syscall_copy(char* src_filename, char* dst_filename)  {
	
	int src_fd = open(src_filename, O_RDONLY);	// opens a file for reading
	if (src_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
		close(src_fd);
		exit(0);
	}
	// O_WRONLY - write only
	// O_CREAT - create file if it doesn't exist
	// O_TRUNC - truncates file to length 0 if it already exists (deletes existing file)
	// opens a file for writing; erases old file/creates a new file
	int dst_fd = open(dst_filename, O_WRONLY | O_CREAT | O_TRUNC);	
	if (dst_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
		close(dst_fd);
		exit(0);
	}


	// read/ write loop
	unsigned char *buf = (unsigned char*)malloc((size_t)BUF_SIZE);   // allocate a buffer to store read data
	// reads up to BUF_SIZE bytes from src_filename
	// writes bytes_read to dst_filename
	
	// closes src_fd file descriptor
	// closes dst_fd file descriptor
	// frees memory used for buf
	while(1){
		size_t bytes_read = read(src_fd, buf, BUF_SIZE); // reads up to BUF_SIZE bytes from src_filename
		if (bytes_read == 0) break;// check for end of file
    		write(dst_fd, buf, bytes_read);
	}
  	free(buf);
    close(src_fd);
    close(dst_fd);
	return 0;
}

// checks the runtime to copy a file using functions vs syscalls
void check_copy_times(char* src_filename, char* dst_filename) {
	clock_t func_start, func_end, syscall_start, syscall_end;
	
	// capture runtime of func_copy() using start clock, call the copy, end the clock
	func_start = clock();
	func_copy(src_filename, dst_filename);
	func_end = clock();
	double func_time = ((double)(func_end - func_start)) / CLOCKS_PER_SEC;
	// capture runtime of syscall_copy() using start clock, call the copy, end the clock
	syscall_start = clock();
	syscall_copy(src_filename, dst_filename);
	syscall_end = clock();
	double syscall_time = ((double)(syscall_end - syscall_start)) / CLOCKS_PER_SEC;

	printf("time to copy using functions: %.7f\n", func_time);
	printf("time to copy using syscalls: %.7f\n", syscall_time);
}
