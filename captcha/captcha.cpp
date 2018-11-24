#include<fstream>
#include<iostream>
// #include<conio.h>
extern "C"
{
	void captcha(unsigned char im[210*200], unsigned char l[16]);
	void makegif(unsigned char im[210*200], unsigned char gif[]);
}

int create_captcha(unsigned char l[16]) {
	unsigned char im[210*200];
	unsigned char gif[17646*3];

	captcha(im,l);
	makegif(im,gif);

	// std::cout << gif <<std::endl;

	std::cout.write(reinterpret_cast<const char*>(gif),17646*3);
	std::cerr.write(reinterpret_cast<const char*>(l),15);

	return 0;
}