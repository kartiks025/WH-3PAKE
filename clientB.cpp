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

    int valread = recv(sock_A , M1, BLOCK_SIZE, MSG_WAITALL);
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
    valread = recv(sock_S, M3, CAPTCHA_SIZE, MSG_WAITALL);

    unsigned char M4[BLOCK_SIZE] = {0};
    valread = recv(sock_S, M4, BLOCK_SIZE, MSG_WAITALL);

    unsigned char M5[CAPTCHA_SIZE] = {0};
    valread = recv(sock_S, M5, CAPTCHA_SIZE, MSG_WAITALL);

    unsigned char M6[BLOCK_SIZE] = {0};
    valread = recv(sock_S, M6, BLOCK_SIZE, MSG_WAITALL);

    char* gs2 = decrypt(M4,BLOCK_SIZE,keyB,ivB);
    unsigned long long g_s2 = to_long(gs2,BLOCK_SIZE);
    
    unsigned long long kbs_ = ((((long long)pow(g_s2,y))%G+G)%G);
    char* kbs = hash(to_bytes(kbs_), BLOCK_SIZE);
    char* phi1 = decrypt(M3,CAPTCHA_SIZE,kbs,ivB);

    int save_captcha = view_captcha(phi1, "captcha1.gif");
    std::string captcha_string;

    std::cin >> captcha_string ;

    if(!valid_string(captcha_string))
        return -1;

    std::string to_hash1 = "1"+captcha_string+IDENTITY_B+IDENTITY_A;
    char* M7 = hash(to_hash1.c_str(), to_hash1.size());

    send(sock_A, M5, CAPTCHA_SIZE, 0);
    send(sock_A, M6, BLOCK_SIZE, 0);
    send(sock_A, M7, BLOCK_SIZE, 0);

    unsigned char M8[BLOCK_SIZE] = {0};
    valread = recv(sock_A, M8, BLOCK_SIZE, MSG_WAITALL);

    std::string to_hash2 = "1"+captcha_string+IDENTITY_A+IDENTITY_B;
    char* this_M8 = hash(to_hash2.c_str(), to_hash2.size());

    if(strcmp(this_M8, M8)!=0)
        return -1;

    std::string to_hash3 = "2"+captcha_string+IDENTITY_A+IDENTITY_B;
    char* sk = hash(to_hash3.c_str(), to_hash3.size());

} 