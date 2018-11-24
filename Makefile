CC = g++
CCLIBS = -L -lgcrypt -lcaptcha
CCFLAGS = -std=c++11

src:= $(wildcard *.cpp)
exe:= $(patsubst %.cpp, %, $(src))

all: captcha $(exe)

captcha: captcha/captcha.c captcha/captcha.hpp
	gcc -c -fPIC captcha/captcha.c -o captcha/captcha
	gcc -shared -o libcaptcha.so captcha/captcha

$(exe): % : %.cpp
	$(CC) $(CCFLAGS) $(CCINCLUDES) $< $(CCLIBS) -o $@

clean:
	rm $(exe)