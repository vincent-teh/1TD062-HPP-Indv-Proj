# CFLAGS = -Igraphics -Wall -Wextra -O3 -march=native -ffast-math -funroll-loops -flto
CFLAGS = -Wall -Wextra -g

all: calc.o strassen.o

calc.o: calc.c calc.h
	gcc $(CFLAGS) -c calc.c -o calc.o

strassen.o: strassen.c strassen.h calc.h
	gcc $(CFLAGS) -lm -c strassen.c -o strassen.o

clean:
	rm -f calc.o
