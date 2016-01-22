CFLAGS = `sdl2-config --cflags` -O4 -Wall -Wextra -Wfloat-equal -pedantic -std=c89 -lm -lSDL2_image -lSDL2_mixer -lSDL2_ttf
INCS = teletext.h test.h
TARGET = teletext
SOURCES = $(TARGET).c
LIBS =  `sdl2-config --libs`
CC = gcc


all: $(TARGET)

$(TARGET):  $(SOURCES) $(INCS)
	$(CC)  $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)


run: all
	./$(TARGET) test.m7.txt



push:
	git commit -a -m $$(date +%d-%m-%Y/%H:%M)
	git push https://github.com/Perdikidis/teletext
pull:
	git pull https://github.com/Perdikidis/teletext

