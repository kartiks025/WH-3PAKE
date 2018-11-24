#include <unistd.h> 
#include <arpa/inet.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string> 
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <ctime> 
#include "helper.hpp"
#include <cmath>
#define PORTS 8080 
#define PORTB 8081 
#define BLOCK_SIZE 16
#define CAPTCHA_SIZE 70*200

int main() 
{ 
    int sock_S = connectSockToServer(PORTS);

    // char hello[1024] = {};
    // strncpy(hello, "Hello from B", sizeof(hello));

    // send(sock_S , hello , strlen(hello) , 0 ); 
    // printf("Hello message sent\n"); 
    // int valread = read( sock_S , buffer, 1024); 
    // printf("%s\n",buffer ); 

    int server_fd;
    struct sockaddr_in address = createServer(PORTB, server_fd);
    int addrlen = sizeof(address); 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    int sock_A;
    if ((sock_A = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }

    char M1[BLOCK_SIZE] = {0};

    int valread = recv(sock_A , M1, BLOCK_SIZE, 0);
    // for(int i=0; i<BLOCK_SIZE ;i++)
    //   std::cout << (int)M1[i] << std::endl;


    unsigned long long G = 19;
    unsigned long long g = 9; //TODO: QR(19) 4 16 7 9 17 11 6 5 1
    std::string passB = "kartik_singhal";
    std::string str_ivB = "hostel5";
    char* keyB = hash(passB.c_str(), passB.size());
    char* ivB = hash(str_ivB.c_str(), str_ivB.size());
    unsigned long long p = 17;
    srand(time(NULL));
    unsigned long long y = rand()%p; // TODO: use a good PRNG over here
    unsigned long long g_y = ((((long long)pow(g,y))%G+G)%G);
    std::cout << g_y << std::endl;
    char* gy = to_bytes(g_y);
    char* M2 = encrypt(gy,BLOCK_SIZE,keyB,ivB);
    for(int i=0; i<BLOCK_SIZE ;i++)
      std::cout << (int)gy[i] << " ";
    std::cout << std::endl;

    send(sock_S, M1, BLOCK_SIZE, 0);
    send(sock_S, M2, BLOCK_SIZE, 0);

    unsigned char M3[CAPTCHA_SIZE] = {0};
    valread = recv(sock_S, M3, CAPTCHA_SIZE, 0);

    unsigned char M4[BLOCK_SIZE] = {0};
    valread = recv(sock_S, M4, BLOCK_SIZE, 0);

    unsigned char M5[CAPTCHA_SIZE] = {0};
    valread = recv(sock_S, M5, CAPTCHA_SIZE, 0);

    unsigned char M6[BLOCK_SIZE] = {0};
    valread = recv(sock_S, M6, BLOCK_SIZE, 0);


} 