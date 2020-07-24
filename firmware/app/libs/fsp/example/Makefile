TARGET=fsp-example
LIBS=../
SOURCE=main.c $(LIBS)/fsp/fsp.c $(LIBS)/fsp/crc16.c

CC=gcc
FLAGS=-Wall -I$(LIBS) -o

all:
	$(CC) $(FLAGS) $(TARGET).x $(SOURCE)

clean:
	rm *.x
