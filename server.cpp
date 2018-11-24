#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <iostream>
#include "helper.hpp"
#include <vector>
#include <cmath>
#include "captcha/captcha.cpp"

int main() 
{ 
    mtInit();
    int server_fd;
    struct sockaddr_in address; 
       
    address = createServer(PORTS, server_fd);
    int num_clients = 2;
    std::vector<int> client_sockets;
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
    }
    std::cout << "Both clients connected" << std::endl;
    
    std::string passA = "mukesh_pareek";
    std::string str_ivA = "hostel9";
    char* keyA = hash(passA.c_str(), passA.size());
    char* ivA = hash(str_ivA.c_str(), str_ivA.size());
    std::string passB = "kartik_singhal";
    std::string str_ivB = "hostel5";
    char* keyB = hash(passB.c_str(), passB.size());
    char* ivB = hash(str_ivB.c_str(), str_ivB.size());

    std::cout << "key construction done" << std::endl;


    char M1[BLOCK_SIZE] = {0};
    char M2[BLOCK_SIZE] = {0};
    int valread = recv(client_sockets[0] , M1, BLOCK_SIZE, MSG_WAITALL);


    valread = recv(client_sockets[0] , M2, BLOCK_SIZE, MSG_WAITALL);
    std::cout << "Received M1 and M2" << std::endl;

    char* gx = decrypt(M1,BLOCK_SIZE,keyA,ivA);
    unsigned long long g_x = to_long(gx,BLOCK_SIZE);
    char* gy = decrypt(M2,BLOCK_SIZE,keyB,ivB);
    // std::cout << "gy bytes: " << gy << std::endl;
    // for(int i=0;i<BLOCK_SIZE;i++)
    //     std::cout << (int)gy[i] << " " << std::endl;
    // std::cout << std::endl;
    unsigned long long g_y = to_long(gy,BLOCK_SIZE);
    std::cout << "gx: " << g_x << std::endl;
    std::cout << "gy: " << g_y << std::endl;
    unsigned long long s1 = getValFromGroup(p);
    unsigned long long s2 = getValFromGroup(p);

    unsigned long long kas_ = power(g_x,s1,p);
    unsigned long long kbs_ = power(g_y,s2,p);

    std::cout << "kas: " << kas_ << std::endl;
    std::cout << "kbs: " << kbs_ << std::endl;

    char* kas = hash(to_bytes(kas_), BLOCK_SIZE);
    char* kbs = hash(to_bytes(kbs_), BLOCK_SIZE);

    std::cout << "calculated kas, kbs" << std::endl;


    std::string l = get_random_string(STRING_SIZE);
    std::cout << "constructing captcha for " << l << std::endl;
    unsigned char* phi1 = create_captcha(reinterpret_cast<unsigned char*>(const_cast<char*>(l.c_str())));
    view_captcha(phi1, "abc.gif");
    char* M3 = encrypt(reinterpret_cast<char*>(phi1),CAPTCHA_SIZE+6,kbs,ivB);
    char* phi1_ = decrypt(M3,CAPTCHA_SIZE+6,kbs,ivB);
    view_captcha(reinterpret_cast<unsigned char*>(phi1_), "abcd.gif");

    std::cout << "constructed M3" << std::endl;

    unsigned long long g_s2 = power(g,s2,p);
    std::cout << "gs2: " << g_s2 << std::endl;
    char* gs2 = to_bytes(g_s2);
    char* M4 = encrypt(gs2,BLOCK_SIZE,keyB,ivB);

    std::cout << "constructed M4" << std::endl;

    unsigned char* phi2 = create_captcha(reinterpret_cast<unsigned char*>(const_cast<char*>(l.c_str())));
    char* M5 = encrypt(reinterpret_cast<char*>(phi2),CAPTCHA_SIZE+6,kas,ivA);

    unsigned long long g_s1 = power(g,s1,p);
    std::cout << "gs1: " << g_s1 << std::endl;
    char* gs1 = to_bytes(g_s1);
    char* M6 = encrypt(gs1,BLOCK_SIZE,keyA,ivA);

    std::cout << "constructed M3,M4" << std::endl;

    send(client_sockets[0], M3, CAPTCHA_SIZE+6, 0);
    send(client_sockets[0], M4, BLOCK_SIZE, 0);
    send(client_sockets[0], M5, CAPTCHA_SIZE+6, 0);
    send(client_sockets[0], M6, BLOCK_SIZE, 0);

    std::cout << "sent M3,M4,M5,M6" << std::endl;

    delete phi1,phi2;

    return 0; 
} 