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

int main() 
{ 
    int sock_S = connectSockToServer(PORTS);
    int sock_B = connectSockToServer(PORTB);

    // char hello[1024] = {};
    // strncpy(hello, "Hello from A", sizeof(hello));
    // char buffer[1024] = {0};

    // send(sock_S , hello , strlen(hello) , 0 ); 
    // printf("Hello message sent\n"); 
    // int valread = read( sock_S , buffer, 1024); 
    // printf("%s\n",buffer ); 

    unsigned long long G = 19;
    unsigned long long g = 9; //TODO: QR(19) 4 16 7 9 17 11 6 5 1
    std::string passA = "mukesh_pareek";
    std::string str_ivA = "hostel9";
    char* keyA = hash(passA.c_str(), passA.size());
    char* ivA = hash(str_ivA.c_str(), str_ivA.size());
    unsigned long long p = 17;
    srand(time(NULL));
    unsigned long long x = rand()%p; // TODO: use a good PRNG over here
    unsigned long long g_x = ((((long long)pow(g,x))%G+G)%G);
    std::cout << g_x << std::endl;
    char* gx = to_bytes(g_x);
    char* M1 = encrypt(gx,BLOCK_SIZE,keyA,ivA);
    char* dec = decrypt(gx,BLOCK_SIZE,keyA,ivA);
    for(int i=0; i<BLOCK_SIZE ;i++)
      std::cout << (int)gx[i] << " ";
    std::cout << std::endl;
    // for(int i=0; i<BLOCK_SIZE ;i++)
    //   std::cout << (int)dec[i] << " ";
    std::cout << std::endl;
    send(sock_B, M1, BLOCK_SIZE, 0);
} 