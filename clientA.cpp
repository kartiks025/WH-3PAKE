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
#define PORTS 8080 
#define PORTB 8081 

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
    int sock_S = connectSockToServer(PORTS);
    int sock_B = connectSockToServer(PORTB);

    char hello[1024] = {};
    strncpy(hello, "Hello from A", sizeof(hello));
    char buffer[1024] = {0};

    send(sock_S , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
    int valread = read( sock_S , buffer, 1024); 
    printf("%s\n",buffer ); 


    // int g = 9; //TODO: QR(19) 4 16 7 9 17 11 6 5 1
    // std::string passA = "mukesh_pareek";
    // std::string str_ivA = "hostel9";
    // char* keyA = hash(passA.c_str(), passA.size());
    // char* ivA = hash(str_ivA.c_str(), str_ivA.size());
    // int p = 17;
    // srand(time(NULL));
    // int x = rand()%p; // TODO: use a good PRNG over here
    // std::string gx = to_binary((int)pow(g,x));
    // // std::cout << gx << std::endl;
    // // std::cout << keyA << std::endl;
    // char* M1 = encrypt(gx.c_str(),gx.size(),keyA,ivA);
    // std::cout << M1 << std::endl;

    // send( sock, M1, gx.size(), 0);
} 