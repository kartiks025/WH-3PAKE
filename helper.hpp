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
#include <random>
#include <ctime> 
#include <cmath>
#include <sstream>
#include <iomanip>
#define LOCAL_IP "127.0.0.1"
#define BLOCK_SIZE 16
#define RANGE_SIZE 25
#define PORTS 8080 
#define PORTB 8081 
#define BLOCK_SIZE 16
#define CAPTCHA_SIZE 17646*3
#define STRING_SIZE 15
#define IDENTITY_A "CLIENTA"
#define IDENTITY_B "CLIENTB"

unsigned long long G = 1451;
unsigned long long g = 3; 
unsigned long long p = 2903;

std::mt19937 my_gen;
std::uniform_int_distribution<> my_dist = std::uniform_int_distribution<> (0,1);

void xerr(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void mtInit()
{
    std::random_device dev;
    std::string seed = std::to_string(dev()) + std::to_string(dev()) + std::to_string(dev()) + std::to_string(dev());
    std::seed_seq new_seed(seed.begin(), seed.end());
    my_gen = std::mt19937(new_seed);
    my_dist = std::uniform_int_distribution<> (0,1);
}

unsigned long long getValFromGroup(unsigned long long mod)
{
    unsigned long long val = mod;
    while(val >= mod){
        unsigned int num_bits = ceil(log2(mod));
        val = 0;
        for(int i=0; i<num_bits; i++)
            val = val*2+my_dist(my_gen);
    }
    return val;
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
    if(inet_pton(AF_INET, LOCAL_IP, &addr.sin_addr)<=0)  
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

char* to_bytes(unsigned long long val)
{
  std::size_t sz = BLOCK_SIZE;
  // std::cout << val << std::endl << std::endl;
  unsigned char* ret = new unsigned char[sz];
  while( sz-- )
  {
    if(val>0)
        ret[sz] = (val&255);
    else
        ret[sz] = 0;
    val >>= 8;
  }
  // for(int i=0; i<sz ;i++)
  //   std::cout << (int)ret[i] << std::endl;
  // std::cout << ret << std::endl;
  return reinterpret_cast<char*>(ret);
}

unsigned long long to_long(char* s_bytes, int len)
{
  unsigned char* bytes = reinterpret_cast<unsigned char*>(s_bytes);
  unsigned long long val = 0;
  for(int i=0; i<len ; i++){
    val = val*256 + bytes[i];
  }
  return val;
}

unsigned long long power(unsigned long long x, unsigned long long y, unsigned long long p)
{
    int res = 1;
    x %= p;
    while (y > 0) 
    { 
        if (y & 1) 
            res = (res*x) % p; 
        y = y>>1;
        x = (x*x) % p;   
    } 
    return res;
} 

std::string get_random_string(int len)
{
    static const char *letters="abcdafahijklmnopqrstuvwxy";
    std::string l = "";
    for (int i = 0; i < len; ++i)
    {
        unsigned long long r = getValFromGroup(RANGE_SIZE);
        l += letters[r];
    }
    return l;
}

int valid_string(std::string l)
{
    std::string letters="abcdafahijklmnopqrstuvwxy";
    for (int i = 0; i < l.size(); ++i)
    {
        if(letters.find(l[i])==std::string::npos)
            return 0;
    }
    return 1;
}

std::string prettyHash(char* s_hash) // bytes to hex
{
    unsigned char* hash = reinterpret_cast<unsigned char*>(s_hash);
    std::stringstream ss;
    ss<<std::hex;
    for (int i=0;i<BLOCK_SIZE;i++)
    {
        ss<<(unsigned int)hash[i]<< " ";
    }
    return ss.str();
}