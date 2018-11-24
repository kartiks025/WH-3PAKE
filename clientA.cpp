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
#include "captcha/captcha.cpp"
#define PORTS 8080 
#define PORTB 8081 
#define BLOCK_SIZE 16
#define CAPTCHA_SIZE 210*200
#define STRING_SIZE 15
#define IDENTITY_A "CLIENTA"
#define IDENTITY_B "CLIENTB"

int main() 
{ 
    int sock_S = connectSockToServer(PORTS);
    int sock_B = connectSockToServer(PORTB);

    char hello[1024] = {};
    strncpy(hello, "Hello from A", sizeof(hello));
    // char buffer[1024] = {0};

    send(sock_S , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
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
    // for(int i=0; i<BLOCK_SIZE ;i++)
    //   std::cout << (int)gx[i] << " ";
    // std::cout << std::endl;
    // // for(int i=0; i<BLOCK_SIZE ;i++)
    // //   std::cout << (int)dec[i] << " ";
    // std::cout << std::endl;
    send(sock_B, M1, BLOCK_SIZE, 0);

    char M5[CAPTCHA_SIZE] = {0};
    int valread = recv(sock_B, M5, CAPTCHA_SIZE, MSG_WAITALL);

    char M6[BLOCK_SIZE] = {0};
    valread = recv(sock_B, M6, BLOCK_SIZE, MSG_WAITALL);
 
    char M7[BLOCK_SIZE] = {0};
    valread = recv(sock_B, M7, BLOCK_SIZE, MSG_WAITALL);

    char* gs1 = decrypt(M6,BLOCK_SIZE,keyA,ivA);
    unsigned long long g_s1 = to_long(gs1,BLOCK_SIZE);

    unsigned long long kas_ = ((((long long)pow(g_s1,x))%G+G)%G);
    char* kas = hash(to_bytes(kas_), BLOCK_SIZE);
    char* phi2 = decrypt(M5,CAPTCHA_SIZE,kas,ivA);

    int save_captcha = view_captcha(phi2, "captcha2.gif");
    std::string captcha_string;

    std::cin >> captcha_string ;

    if(!valid_string(captcha_string))
        return -1;

    std::string to_hash1 = "1"+captcha_string+IDENTITY_B+IDENTITY_A;
    char* this_M7 = hash(to_hash1.c_str(), to_hash1.size());

    if(strcmp(this_M7, M7)!=0)
        return -1;

    std::string to_hash2 = "1"+captcha_string+IDENTITY_A+IDENTITY_B;
    char* M8 = hash(to_hash2.c_str(), to_hash2.size());

    std::string to_hash3 = "2"+captcha_string+IDENTITY_A+IDENTITY_B;
    char* sk = hash(to_hash3.c_str(), to_hash3.size());

    send(sock_B, M8, BLOCK_SIZE, 0);



} 