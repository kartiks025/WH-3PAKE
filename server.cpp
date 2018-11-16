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
        int valread = read( new_socket , buffer, 1024); 
        printf("%s\n",buffer ); 
        send(new_socket , hello , strlen(hello) , 0 ); 
        printf("Hello message sent\n"); 
    }
    
    // valread = read( new_socket , buffer, 1024);
    // printf("%s\n",buffer );
    // std::cout << M1 << std::endl;
    return 0; 
} 