CFLAGS := -O0 -g -std=c++11

CC = gcc
CXX = g++

all : app

app : 
	$(CXX) $(CFLAGS) -Wall simulate_contextswitch.cpp -pthread

run : app
	./a.out 

clean :
	rm -rf *.o a.out

