INC_PATHS := -Iinterfaces -Iimpl -I.

ifndef GCOV
	OPTIMIZATION_FLAG := -O3
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

CFLAGS := $(OPTIMIZATION_FLAG) $(INC_PATHS) $(CORE_FLAGS) -std=c++11
PFLAGS = -g

CC = gcc
CXX = g++
ASM = yasm

all : app

app : 
	$(CXX) $(CFLAGS) $(OPTIMIZATION_FLAG) -Wall  async_futures.cpp

run : app
	./a.out 

clean :
	rm -rf *.o a.out

