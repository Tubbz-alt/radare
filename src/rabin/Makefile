include ../../config.mk

CFLAGS+=-DRADARE_CORE -g
OBJ=dietelf.o dietelf64.o rabin.o dietpe.o dietpe64.o dietmach0.o dietmach0_utils.o aux.o

all: rabin${BINSFX}

rabin${BINSFX}: ${OBJ}
	${CC} ${OBJ} ${DL_LIBS} -o rabin${BINSFX}

install:
	cd .. && ${MAKE} install

clean:
	-rm -f dietelf.o dietelf64.o rabin.o dietpe.o dietpe64.o dietmach0.o dietmach0_utils.o rabin${BINSFX} aux.o
