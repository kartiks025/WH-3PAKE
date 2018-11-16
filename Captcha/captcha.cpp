#include<fstream>
#include<iostream>
// #include<conio.h>
extern "C"
{
	void captcha(unsigned char im[70*200], unsigned char l[6]);
	void makegif(unsigned char im[70*200], unsigned char gif[]);
}

int main() {
	unsigned char l[6] = "abcde";
	unsigned char im[70*200];
	unsigned char gif[17646];

	captcha(im,l);
	makegif(im,gif);

	//std::cout << gif <<std::endl;

	std::cout.write(reinterpret_cast<const char*>(gif),17646);
	std::cerr.write(reinterpret_cast<const char*>(l),5);

	return 0;
}