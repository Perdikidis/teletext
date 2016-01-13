CFLAGS = `sdl2-config --cflags` -O4 -Wall -Wextra -Wfloat-equal -pedantic -std=c89 -lm -lSDL2_image -lSDL2_mixer -lSDL2_ttf
INCS = teletext.h test.h
TARGET = teletext
TEST = test
SOURCES = $(TARGET).c
LIBS =  `sdl2-config --libs`
CC = gcc


all: $(TARGET)

$(TARGET): test.c $(SOURCES) $(INCS)
	$(CC) test.c $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET) test.m7.txt

test: teletext.c test.c $(INCS)
	gcc test.c -o test `sdl2-config --cflags` -O4 -Wall -Wextra -Wfloat-equal -pedantic -std=c89 -lm -lSDL2_image -lSDL2_mixer -lSDL2_ttf `sdl2-config --libs`
	./$(TEST) test.m7.txt

push:
	git commit -a -m $$(date +%d-%m-%Y/%H:%M)
	git push
pull:
	git pull

