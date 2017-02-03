INC_PATHS := -Iinterfaces -Iimpl -I.

ifndef GCOV
	OPTIMIZATION_FLAG := -O3
ifeq ($(CORE),avx2)
	CORE_FLAGS := -march=core-avx2 -mavx2
else
	CORE_FLAGS := -march=corei7 -mavx2
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
	$(CC) $(CFLAGS) $(OPTIMIZATION_FLAG) -Wall  identify.c

run : app
	./a.out 

clean :
	rm -rf *.o a.out

