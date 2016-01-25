CFLAGS = `pkg-config sdl2 --cflags` -O4 -Wall -Wextra -Wfloat-equal -pedantic -std=c89 -lm
INCS = teletext.h test.h
TARGET = teletext
SOURCES = $(TARGET).c
LIBS =  `pkg-config sdl2 --libs`
CC = gcc


all: $(TARGET)

$(TARGET): test.c $(SOURCES) $(INCS)
	$(CC) test.c $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET) lfc.m7.txt




push:
	git commit -a -m $$(date +%d-%m-%Y/%H:%M)
	git push
pull:
	git pull
