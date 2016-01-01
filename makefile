CFLAGS = `sdl-config --cflags` -O4 -Wall -Wextra -Wfloat-equal -pedantic -std=c99 -lm
TARGET = sdl_tutorial
SOURCES =   $(TARGET).c
LIBS =  `sdl-config --libs`
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET) 
