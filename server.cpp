#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <iostream>
#include "helper.hpp"
#include <vector>
#define PORTS 8080 
#define BLOCK_SIZE 16

int main() 
{ 
    int server_fd;
    struct sockaddr_in address; 
    char buffer[1024] = {0}; 
    char hello[1024] = {};
    strncpy(hello, "Hello from server", sizeof(hello));
       
    address = createServer(PORTS, server_fd);
    int num_clients = 2;
    std::vector<int> client_sockets; //TODO: assuming 0 is B, 1 is A
    while(num_clients--)
    {
        int addrlen = sizeof(address); 
        if (listen(server_fd, 3) < 0) 
        { 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        } 
        int new_socket;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                           (socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }
        client_sockets.push_back(new_socket);
        // int valread = read( new_socket , buffer, 1024); 
        // printf("%s\n",buffer ); 
        // send(new_socket , hello , strlen(hello) , 0 ); 
        // printf("Hello message sent\n"); 
    }
    
    unsigned long long G = 19;
    unsigned long long g = 9; //TODO: QR(19) 4 16 7 9 17 11 6 5 1
    unsigned long long p = 17;
    std::string passA = "mukesh_pareek";
    std::string str_ivA = "hostel9";
    char* keyA = hash(passA.c_str(), passA.size());
    char* ivA = hash(str_ivA.c_str(), str_ivA.size());
    std::string passB = "kartik_singhal";
    std::string str_ivB = "hostel5";
    char* keyB = hash(passB.c_str(), passB.size());
    char* ivB = hash(str_ivB.c_str(), str_ivB.size());

    char M1[BLOCK_SIZE] = {0};
    char M2[BLOCK_SIZE] = {0};
    int valread = recv(client_sockets[0] , M1, BLOCK_SIZE, 0);
    // for(int i=0; i<BLOCK_SIZE ;i++)
    //   std::cout << (int)M1[i] << std::endl;
    valread = recv(client_sockets[0] , M2, BLOCK_SIZE, 0);
    // for(int i=0; i<BLOCK_SIZE ;i++)
    //   std::cout << (int)M2[i] << std::endl;

    char* gx = decrypt(M1,BLOCK_SIZE,keyA,ivA);
    unsigned long long g_x = to_long(gx,BLOCK_SIZE);
    char* gy = decrypt(M2,BLOCK_SIZE,keyB,ivB);
    unsigned long long g_y = to_long(gy,BLOCK_SIZE);
    // for(int i=0; i<BLOCK_SIZE ;i++)
    //   std::cout << (int)gx[i] << " ";
    // std::cout << std::endl;
    // for(int i=0; i<BLOCK_SIZE ;i++)
    //   std::cout << (int)gy[i] << " ";
    // std::cout << std::endl;
    std::cout << g_x << std::endl;
    std::cout << g_y << std::endl;
    srand(time(NULL));
    unsigned long long s1 = rand()%p;
    unsigned long long s2 = rand()%p;
    unsigned long long kas = ((((long long)pow(g_x,s1))%G+G)%G);
    unsigned long long kbs = ((((long long)pow(g_x,s2))%G+G)%G);

    return 0; 
} 