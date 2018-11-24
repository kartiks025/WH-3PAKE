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
    int sock_B = connectSockToServer(PORTB);

    std::string passA = "mukesh_pareek";
    std::string str_ivA = "hostel9";
    char* keyA = hash(passA.c_str(), passA.size());
    char* ivA = hash(str_ivA.c_str(), str_ivA.size());
    unsigned long long x = getValFromGroup(p); 
    unsigned long long g_x = power(g,x,p);
    char* gx = to_bytes(g_x);
    char* M1 = encrypt(gx,BLOCK_SIZE,keyA,ivA);
    std::cout << "calculated M1" << std::endl;
    std::cout << "gx: " << g_x << std::endl;

    send(sock_B, M1, BLOCK_SIZE, 0);
    std::cout << "sent M1" << std::endl;

    char M5[CAPTCHA_SIZE+6] = {0};
    int valread = recv(sock_B, M5, CAPTCHA_SIZE+6, MSG_WAITALL);

    char M6[BLOCK_SIZE] = {0};
    valread = recv(sock_B, M6, BLOCK_SIZE, MSG_WAITALL);
 
    char M7[BLOCK_SIZE+1] = {0};
    valread = recv(sock_B, M7, BLOCK_SIZE, MSG_WAITALL);
    M7[BLOCK_SIZE] = '\0';

    std::cout << "received M5, M6, M7" << std::endl;

    char* gs1 = decrypt(M6,BLOCK_SIZE,keyA,ivA);
    unsigned long long g_s1 = to_long(gs1,BLOCK_SIZE);
    std::cout << "gs1: " << g_s1 << std::endl;

    unsigned long long kas_ = power(g_s1,x,p);
    std::cout << "kas: " << kas_ << std::endl;

    char* kas = hash(to_bytes(kas_), BLOCK_SIZE);
    char* phi2 = decrypt(M5,CAPTCHA_SIZE+6,kas,ivA);

    std::cout << "decrypted captcha" << std::endl;


    int save_captcha = view_captcha(reinterpret_cast<unsigned char*>(phi2), "captcha2.gif");
    std::cout << "saved captcha, please see captcha2.gif and enter the 15 character long string:" << std::endl;

    std::string captcha_string;

    std::cin >> captcha_string ;

    std::cout << "input from user: " << captcha_string << std::endl;

    std::string to_hash1 = "1"+captcha_string+IDENTITY_B+IDENTITY_A;
    std::cout << "H(1||r||B||A): " << to_hash1 << std::endl;
    char* this_M7 = hash(to_hash1.c_str(), to_hash1.size());
    std::cout << "M7: " << prettyHash(this_M7) << std::endl;
    std::cout << "calculated M7" << std::endl;
    std::cout << "B's M7: " << prettyHash(M7) << std::endl;

    if(strcmp(this_M7, M7)!=0){
        std::cout << "verification of hash using captcha's string failed, aborting" << std::endl;
        return -1;
    }
    std::cout << "M7 matches B's M7" << std::endl;


    std::string to_hash2 = "1"+captcha_string+IDENTITY_A+IDENTITY_B;
    char* M8 = hash(to_hash2.c_str(), to_hash2.size());

    std::cout << "calculated M8" << std::endl;

    std::string to_hash3 = "2"+captcha_string+IDENTITY_A+IDENTITY_B;
    char* sk = hash(to_hash3.c_str(), to_hash3.size());
    std::cout << "calculated secret key: " << prettyHash(sk) << std::endl;

    send(sock_B, M8, BLOCK_SIZE, 0);
    std::cout << "sent M8" << std::endl;


} 