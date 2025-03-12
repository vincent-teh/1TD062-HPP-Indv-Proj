CFLAGS = -Igraphics -Wall -Wextra -O3 -march=native -ffast-math -funroll-loops -flto
# CFLAGS = -Wall -Wextra -g

calc.o: calc.c calc.h
	gcc $(CFLAGS) -c calc.c -o calc.o

clean:
	rm -f calc.o
