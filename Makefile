CC = g++
CCLIBS = -lgcrypt
CCFLAGS = -std=c++11

src:= $(wildcard *.cpp)
exe:= $(patsubst %.cpp, %, $(src))

all: $(exe)

$(exe): % : %.cpp
	$(CC) $(CCFLAGS) $(CCINCLUDES) $< $(CCLIBS) -o $@

clean:
	rm $(exe)