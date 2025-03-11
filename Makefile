CC = gcc
# CFLAGS = -Igraphics -Wall -Wextra -O3 -ffast-math -funroll-loops -flto -ftree-vectorize
CFLAGS = -Igraphics -Wall -Wextra -O3
TARGET = app
SUB_DIR = generate_mat utils calc
SUB_TARGET = generate_mat/generate_mat.o utils/helper_func.o calc/calc.o
LDLIBS = -lm

all: $(SUB_TARGET) $(TARGET)

$(TARGET): main.o $(SUB_TARGET)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SUB_TARGET):
	@for dir in $(SUB_DIR); do $(MAKE) -C $$dir; done

clean:
	rm -f $(TARGET) main.o $(SUB_TARGET)
	@for dir in $(SUB_DIR); do $(MAKE) -C $$dir clean; done

.PHONY: all clean
