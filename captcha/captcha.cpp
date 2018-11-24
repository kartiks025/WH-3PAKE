#include<fstream>
#include<iostream>
// #include<conio.h>
extern "C"
{
	void captcha(unsigned char im[210*200], unsigned char l[16]);
	void makegif(unsigned char im[210*200], unsigned char gif[]);
}

unsigned char *create_captcha(unsigned char l[15]) {
	unsigned char im[210*200];
	unsigned char gif[17646*3];

	captcha(im,l);
	makegif(im,gif);

	return gif;
}

int view_captcha(char gif[17646*3], std::string file_name) {
	std::ofstream gif_file;
	gif_file.open(file_name);
	gif_file.write(reinterpret_cast<const char*>(gif),17646*3);
	return 0;
}