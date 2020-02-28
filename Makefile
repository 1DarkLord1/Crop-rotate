all: crop-rotate

CFLAGS=-I include -Wall -Wextra -Werror -g


hw_01: obj obj/bmp.o obj/main.o
	gcc obj/bmp.o obj/main.o -o crop-rotate

obj/bmp.o: src/bmp.c include/bmp.h
	gcc $(CFLAGS) -c src/bmp.c -o obj/bmp.o

obj/main.o: src/main.c
	gcc $(CFLAGS) -c src/main.c -o obj/main.o

obj: 
	mkdir -p obj

clean:
	rm -rf obj crop-rotate
.PHONY: clean

















