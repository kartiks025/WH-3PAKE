CC = g++
CCLIBS = -L/home/kartik/CourseWork/Sem7/WH-3PAKE/ -lgcrypt -lcaptcha
CCFLAGS = -std=c++11

src:= $(wildcard *.cpp)
exe:= $(patsubst %.cpp, %, $(src))
cap_src:= $(wildcard captcha/*.c)
cap:= $(patsubst captcha/%.c, %.so, $(cap_src))

all: captcha exec

exec : $(exe)

captcha : $(cap)

$(cap): % : captcha/libcaptcha.c
	gcc -fPIC -c $< -o captcha/captcha
	gcc -shared -o $@ captcha/captcha

$(exe): % : %.cpp helper.hpp
	$(CC) $(CCFLAGS) $(CCINCLUDES) $< $(CCLIBS) -o $@

clean:
	rm $(exe) *.gif *.so captcha/captcha