CC = gcc
CFLAGS = -Wall -Ides -Iutils
SRC = main.c ./lib/des/des.c ./lib/aes/aes.c
OUT = ./build/main

all:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

run:
	$(OUT).exe

clean:
	rm -f $(OUT) ./output/*.txt

