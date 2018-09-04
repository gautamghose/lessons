CFLAGS := -O0 -g -std=c++11

CC = gcc
CXX = g++

all : app

app : 
	$(CXX) $(CFLAGS) $(DEFINES) -Wall check_palindrome.cpp

run : app
	./a.out 

clean :
	rm -rf *.o a.out

