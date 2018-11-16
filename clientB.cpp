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

int main() 
{ 
    int sock_S = connectSockToServer(PORTS);

    char hello[1024] = {};
    strncpy(hello, "Hello from B", sizeof(hello));
    char buffer[1024] = {0};

    send(sock_S , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
    int valread = read( sock_S , buffer, 1024); 
    printf("%s\n",buffer ); 
} 