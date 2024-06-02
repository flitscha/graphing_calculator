CC = gcc
CFLAGS = -Wall -Wextra -D_Default_SOURCE

.PHONY: all clean

all: main

main: main.c
#	gcc -o main main.c glad/glad.c -lglfw -ldl -lX11 -lpthread -lm -lXrandr -lXi
	gcc -o main main.c glad/glad.c -lglfw

clean:
	rm main