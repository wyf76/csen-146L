//COEN 146L : Lab3, step 1: TCP server that accepts a client connection for file transfer.
/*
Name: Yufan Weng
Date: Jan 21, 2026
Title: Lab 3 - TCP server that accepts a client connection for file transfer
Description: This program implements a TCP server that accepts a client connection and handles file transfer requests.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

//Declare socket file descriptor.
int sockfd, connfd;

//Declare receiving and sending buffers of size 10 bytes
char recvBuf[10], sendBuf[10];

//Declare server address to which to bind for receiving messages and client address to fill in sending address
struct sockaddr_in servAddr, clienAddr;
socklen_t clienAddrLen = sizeof(clienAddr);

//Connection handler for servicing client request for file transfer
void* connectionHandler(void* sock){
   //declate buffer holding the name of the file from client
   char fileName[256];
   //get the connection descriptor
   int connfd = *(int*)sock;
   //Connection established, server begins to read and write to the connecting client
   printf("Connection Established with client IP: %s and Port: %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));
   
	//receive name of the file from the client
   int m = read(connfd, fileName, sizeof(fileName)-1);
   if (m <= 0) { close(connfd); return 0; }
   fileName[m] = '\0';
   fileName[strcspn(fileName, "\r\n")] = 0;

printf("Requested file: '%s'\n", fileName);
   //open file and send to client
   FILE *fp = fopen(fileName, "rb");
   if (fp == NULL){
       printf("File not found\n");
       close(connfd);
       return 0;
   }

   //read file and send to connection descriptor
   int n;
   while ((n = fread(sendBuf, 1, sizeof(sendBuf), fp)) > 0){
       write(connfd, sendBuf, n);
   }
   printf("File transfer complete\n");
   
   //close file
   fclose(fp);
   //Close connection descriptor
   close(connfd);
   return 0;
}


int main(int argc, char *argv[]){
   //Get from the command line, server IP, src and dst files.
   if (argc != 2){
      printf ("Usage: %s <port #> \n",argv[0]);
      exit(0);
   } 
   //Open a TCP socket, if successful, returns a descriptor
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   //Setup the server address to bind using socket addressing structure
   memset(&servAddr, 0, sizeof(servAddr));
   servAddr.sin_family = AF_INET;
   servAddr.sin_addr.s_addr = INADDR_ANY;
   servAddr.sin_port = htons(atoi(argv[1]));

   //bind IP address and port for server endpoint socket 
   bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr));

   // Server listening to the socket endpoint, and can queue 5 client requests
   listen(sockfd, 5);
   printf("Server listening/waiting for client at port %s\n", argv[1]);

   //Server accepts the connection and call the connection handler
   connfd = accept(sockfd, (struct sockaddr *)&clienAddr, &clienAddrLen);
   connectionHandler((void*)&connfd);
   
   //close socket descriptor
   close(sockfd);
   return 0;
}