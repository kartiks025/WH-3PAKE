#include "gcrypt.h"
#include <string>
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
#define LOCAL_IP "127.0.0.1"

void xerr(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

char* encrypt(const char* in, int len, char* key, char* iv){
	gcry_cipher_hd_t hd;
	gcry_cipher_open(&hd, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC,0);
	int err = gcry_cipher_setkey(hd,key,16);
    if (err) {
        xerr("gcrypt: could not set cipher key");
    }    
	err = gcry_cipher_setiv(hd,iv,16); 
    if (err) {
        xerr("gcrypt: could not set iv");
    }
    char* out = new char[len];
    err = gcry_cipher_encrypt(hd,out,len,in,len);
    if (err) {
        xerr("gcrypt: could not encrypt with AES");
    }
    gcry_cipher_close(hd);
    return out;
}

char* decrypt(const char* in, int len, char* key, char* iv){
	gcry_cipher_hd_t hd;
	gcry_cipher_open(&hd, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC,0);
	int err = gcry_cipher_setkey(hd,key,16);
    if (err) {
        xerr("gcrypt: could not set cipher key");
    }    
	err = gcry_cipher_setiv(hd,iv,16); 
    if (err) {
        xerr("gcrypt: could not set iv");
    }
    char* out = new char[len];
    err = gcry_cipher_decrypt(hd,out,len,in,len);
    if (err) {
        xerr("gcrypt: could not encrypt with AES");
    }
    gcry_cipher_close(hd);
    return out;
}

char* hash(const char* to_hash, int len){
	// TODO: use a different algo BlakeS? GCRY_MD_BLAKE2S_128
	int size = gcry_md_get_algo_dlen(GCRY_MD_MD5);
	char* hashed = new char[size];
	gcry_md_hash_buffer(GCRY_MD_MD5, hashed, to_hash,len);
	return hashed;
}

int connectSockToServer(int port)
{
    int sock = 0; 
    struct sockaddr_in addr; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
    memset(&addr, '0', sizeof(addr)); 
   
    addr.sin_family = AF_INET; 
    addr.sin_port = htons(port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    return sock;
}

struct sockaddr_in createServer(int port, int& server_fd)
{
    int new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( port ); 

    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    return address;
}