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


int main() 
{ 
    mtInit();
    int sock_S = connectSockToServer(PORTS);

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
    std::cout << "received M1" << std::endl;

    std::string passB = "kartik_singhal";
    std::string str_ivB = "hostel5";
    char* keyB = hash(passB.c_str(), passB.size());
    char* ivB = hash(str_ivB.c_str(), str_ivB.size());
    unsigned long long y = getValFromGroup(p);
    unsigned long long g_y =  power(g,y,p);
    char* gy = to_bytes(g_y);
    // std::cout << "gy bytes: " << gy << std::endl;
    // for(int i=0;i<BLOCK_SIZE;i++)
    //     std::cout << (int)gy[i] << " " << std::endl;
    // std::cout << std::endl;
    std::cout << "gy: " << g_y << std::endl;
    char* M2 = encrypt(gy,BLOCK_SIZE,keyB,ivB);

    std::cout << "calculated M2" << std::endl;

    send(sock_S, M1, BLOCK_SIZE, 0);
    send(sock_S, M2, BLOCK_SIZE, 0);

    std::cout << "sent M2" << std::endl;

    char M3[CAPTCHA_SIZE+6] = {0};
    valread = recv(sock_S, M3, CAPTCHA_SIZE+6, MSG_WAITALL);

    char M4[BLOCK_SIZE] = {0};
    valread = recv(sock_S, M4, BLOCK_SIZE, MSG_WAITALL);

    char M5[CAPTCHA_SIZE+6] = {0};
    valread = recv(sock_S, M5, CAPTCHA_SIZE+6, MSG_WAITALL);

    char M6[BLOCK_SIZE] = {0};
    valread = recv(sock_S, M6, BLOCK_SIZE, MSG_WAITALL);

    std::cout << "received M3, M4, M5, M6" << std::endl;

    char* gs2 = decrypt(M4,BLOCK_SIZE,keyB,ivB);
    unsigned long long g_s2 = to_long(gs2,BLOCK_SIZE);

    std::cout << "gs2: " << g_s2 << std::endl;

    unsigned long long kbs_ = power(g_s2,y,p);
    std::cout << "kbs: " << kbs_ << std::endl;

    char* kbs = hash(to_bytes(kbs_), BLOCK_SIZE);
    char* phi1 = decrypt(M3,CAPTCHA_SIZE+6,kbs,ivB);

    std::cout << "decrypted captcha" << std::endl;

    int save_captcha = view_captcha(reinterpret_cast<unsigned char*>(phi1), "captcha1.gif");
    std::cout << "saved captcha, please see captcha1.gif and enter the 15 character long string:" << std::endl;
    std::string captcha_string;

    std::cin >> captcha_string ;

    std::cout << "input from user: " << captcha_string << std::endl;

    std::string to_hash1 = "1"+captcha_string+IDENTITY_B+IDENTITY_A;
    std::cout << "H(1||r||B||A): " << to_hash1 << std::endl;
    char* M7 = hash(to_hash1.c_str(), to_hash1.size());
    std::cout << "M7: " << prettyHash(M7) << std::endl;
    std::cout << "calculated M7" << std::endl;

    send(sock_A, M5, CAPTCHA_SIZE+6, 0);
    send(sock_A, M6, BLOCK_SIZE, 0);
    send(sock_A, M7, BLOCK_SIZE, 0);

    std::cout << "sent M5, M6, M7" << std::endl;


    char M8[BLOCK_SIZE+1] = {0};
    valread = recv(sock_A, M8, BLOCK_SIZE, MSG_WAITALL);
    if(valread == 0)
    {
        std::cout << "client A exit the protcol before completion" << std::endl;
        return -1;
    }
    M8[BLOCK_SIZE] = '\0';

    std::cout << "received M8" << std::endl;

    std::string to_hash2 = "1"+captcha_string+IDENTITY_A+IDENTITY_B;
    char* this_M8 = hash(to_hash2.c_str(), to_hash2.size());

    if(strcmp(this_M8, M8)!=0)
    {
        std::cout << "verification of hash using captcha's string failed, aborting" << std::endl;
        return -1;
    }

    std::string to_hash3 = "2"+captcha_string+IDENTITY_A+IDENTITY_B;
    char* sk = hash(to_hash3.c_str(), to_hash3.size());

    std::cout << "calculated secret key: " << prettyHash(sk) << std::endl;


} 