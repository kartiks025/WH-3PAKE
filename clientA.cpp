
// Client side C/C++ program to demonstrate Socket programming 
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
#include "helper.cpp"
#include <cmath>
#define PORT 8080 
   
std::string to_binary(int val)
{
  std::size_t sz = sizeof(val)*8;
  std::string ret(sz, ' ');
  while( sz-- )
  {
    ret[sz] = '0'+(val&1);
    val >>= 1;
  }
  return ret;
}

int main() 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char hello[1024] = {};
    strncpy(hello, "Hello from client", sizeof(hello));
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    send(sock , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 


    int g = 9; //TODO: QR(19) 4 16 7 9 17 11 6 5 1
    std::string passA = "mukesh_pareek";
    std::string str_ivA = "hostel9";
    char* keyA = hash(passA.c_str(), passA.size());
    char* ivA = hash(str_ivA.c_str(), str_ivA.size());
    int p = 17;
    srand(time(NULL));
    int x = rand()%p; // TODO: use a good PRNG over here
    std::string gx = to_binary((int)pow(g,x));
    // std::cout << gx << std::endl;
    // std::cout << keyA << std::endl;
    char* M1 = encrypt(gx.c_str(),gx.size(),keyA,ivA);
    std::cout << M1 << std::endl;

    send( sock, M1, gx.size(), 0);
    return 0; 
} 