CFLAGS := -O0 -g -std=c++11

CC = gcc
CXX = g++

all : app

app : 
	$(CXX) $(CFLAGS) $(DEFINES) -Wall build_human_tower.cpp

run : app
	./a.out 

clean :
	rm -rf *.o a.out

