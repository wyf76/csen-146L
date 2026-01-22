//COEN 146L : Lab3, step 1: TCP client requesting a file transfer from the server.
/*
Name: Yufan Weng
Date: Jan 21, 2026
Title: Lab 3 client - TCP file transfer client
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    //Get from the command line, server IP, src and dst files.
    if (argc != 5){
		printf ("Usage: %s <ip of server> <port #> <src_file> <dest_file>\n",argv[0]);
		exit(0);
    } 
    //Declare socket file descriptor and buffer
    int sockfd;
    char buf[10];

    //Declare server address to accept
    struct sockaddr_in servAddr;

   //Declare host
    struct hostent *host;

    //get hostname
    host = gethostbyname(argv[1]);

    //Open a socket, if successful, returns
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //Set the server address to send using socket addressing structure
    memset(&servAddr, 0, sizeof(servAddr)); //zero the struct
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[2]));
    memcpy(&servAddr.sin_addr, host->h_addr_list[0], host->h_length);

    //Connect to the server
    connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr));

    //Client sends the name of the file to retrieve from the server
    write(sockfd, argv[3], strlen(argv[3]));

    //Client begins to write and read from the server
    FILE *fp = fopen(argv[4], "wb");

    int n;
    while ((n = read(sockfd, buf, sizeof(buf))) > 0){
        fwrite(buf, 1, n, fp);
    }
    fclose(fp);

    //Close socket descriptor
    close(sockfd);

    return 0;
}

