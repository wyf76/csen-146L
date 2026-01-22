//COEN 146L : Lab3, step 3: concurrent TCP server that accepts and responds to multiple client connection requests, each requesting a file transfer
/*
Name: Yufan Weng
Date: Jan 21, 2026
Title: Lab 3 - Concurrent TCP server for file transfer
Description: This program implements a concurrent TCP server that can handle multiple client connection requests for file transfer
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

//Define the number of clients/threads that can be served
#define N 100
int threadCount = 0;
pthread_t clients[N]; //declaring N threads

 //Declare socket and connection file descriptors.
 int sockfd, connfd;

 //Declare receiving and sending buffers of size 10 bytes
 char recvBuf[10], sendBuf[10];
 //Declare server address to which to bind for receiving messages and client address to fill in sending address
 struct sockaddr_in servAddr, clienAddr;
 socklen_t clienAddrLen = sizeof(clienAddr);

//Connection handler (thread function) for servicing client requests for file transfer
void* connectionHandler(void* sock){
   //declare buffer holding the name of the file from client
   char fileName[256];

   //Connection established, server begins to read and write to the connecting client
   printf("Connection Established with client IP: %s and Port: %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));
   
	//receive name of the file from the client
	memset(fileName, 0, sizeof(fileName));
   read(connfd, fileName, sizeof(fileName));
   //open file and send to client
   FILE *fp = fopen(fileName, "rb");
   if (fp == NULL){
       printf("File not found\n");
       close(connfd);
       return 0;
   }
   //read file and send to connection descriptor
   int n;
   while ((n = fread(sendBuf, 1, sizeof(sendBuf), fp)) > 0)
       write(connfd, sendBuf, n);
   printf("File transfer complete\n");
   
   //close file
   fclose (fp);

   //Close connection descriptor
   close(connfd);
   
   pthread_exit(0);
}


int main(int argc, char *argv[]){
   //Get from the command line, server IP, src and dst files.
   if (argc != 2){
      printf ("Usage: %s <port #> \n",argv[0]);
      exit(0);
   } 
   //Open a TCP socket, if successful, returns a descriptor
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0){
      perror("socket");
      exit(0);
   }
   //Setup the server address to bind using socket addressing structure
   memset(&servAddr, 0, sizeof(servAddr));
   servAddr.sin_family = AF_INET;
   servAddr.sin_addr.s_addr = INADDR_ANY;
   servAddr.sin_port = htons(atoi(argv[1]));

   //bind IP address and port for server endpoint socket
   if (bind(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0){
      perror("bind");
      exit(0);
   }

   // Server listening to the socket endpoint, and can queue 5 client requests
   printf("Server listening/waiting for client at port %s\n", argv);
   listen(sockfd, 5);
   while (1){
      //Server accepts the connection and call the connection handler
      connfd = accept(sockfd, (struct sockaddr *)&clienAddr, &clienAddrLen);

      if(pthread_create(&clients[threadCount], NULL, connectionHandler, (void*) &connfd) < 0){
         perror("Unable to create a thread");
         exit(0);
      }
      else 
         printf("Thread %d has been created to service client request\n",++threadCount);
   }
   for(int i = 0; i < threadCount; i++){
         pthread_join(clients[i], NULL);
   }
   return 0;
}