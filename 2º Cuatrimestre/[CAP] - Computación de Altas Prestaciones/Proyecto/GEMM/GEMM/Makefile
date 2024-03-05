
CC      := gcc
ARCH    := -march=native
FLAGS   := -O3 -fopenmp -Wall -fPIC -std=c99 ${ARCH} 
LIBS    := -lm
INCLUDE := -I./include

vpath %.c ./
vpath %.h ./

OBJDIR=build

all: gemm_driver

gemm_driver: 
	@mkdir -p ${OBJDIR}
	${CC} ${FLAGS} gemm_driver.c my_gemm.c lib/libgemm.a -o ${OBJDIR}/gemm_driver.x ${INCLUDE} ${LIBS}

clean:
	rm build/*
