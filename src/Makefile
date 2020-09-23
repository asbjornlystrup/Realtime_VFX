CFLAGS=-std=c++11 -O2 -lm -lGL -lGLU -lglfw -lGLEW -lfreeimage
CC=g++

OBJ=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

all : clean main

main : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o : %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY : clean

clean :
	rm -f *.o main
