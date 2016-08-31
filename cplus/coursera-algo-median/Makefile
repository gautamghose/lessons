INC_PATHS := -Iinterfaces -Iimpl -I.

ifndef GCOV
	OPTIMIZATION_FLAG := -O0 -g
ifeq ($(CORE),avx2)
	CORE_FLAGS := -march=core-avx2
else
	CORE_FLAGS := -march=corei7
endif # AVX2
else # ifndef GCOV
	OPTIMIZATION_FLAG := -O0
	CORE_FLAGS := -fno-strict-aliasing -march=corei7
	OTHER_LIBS :=-lgcov

endif # ifndef GCOV

CFLAGS := $(OPTIMIZATION_FLAG) $(INC_PATHS) $(CORE_FLAGS)
PFLAGS = -g

CC = gcc
CXX = g++
ASM = yasm

all : app

app : 
	$(CXX) -std=c++11 $(CFLAGS) $(OPTIMIZATION_FLAG) -Wall  maintain_median.cpp

run : app
	./a.out 

clean :
	rm -rf *.o a.out

