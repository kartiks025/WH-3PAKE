FILES =  server.o clientA.o clientB.o

hello: $(FILES)

.cpp.o:
        g++ -std=c++11 -lgcrypt $<

clean:
        -rm *.o