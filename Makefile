CC = g++
CCLIBS = -L -lgcrypt -lcaptcha
CCFLAGS = -std=c++11

src:= $(wildcard *.cpp)
exe:= $(patsubst %.cpp, %, $(src))

all: captcha $(exe)

captcha: 
	gcc -c captcha/captcha.c -o captcha/captcha
	gcc -shared -o libcaptcha.so captcha
	g++ -L captcha.cpp -o captcha.o -lcaptcha

$(exe): % : %.cpp
	$(CC) $(CCFLAGS) $(CCINCLUDES) $< $(CCLIBS) -o $@

clean:
	rm $(exe)