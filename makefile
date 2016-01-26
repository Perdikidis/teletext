CFLAGS = `pkg-config sdl2 --cflags` -O4 -Wall -Wextra -Wfloat-equal -pedantic -std=c89 -lm
LIBS =  `pkg-config sdl2 --libs`
CC = gcc


teletext: teletext.c teletext.h
	$(CC) teletext.c -o teletext $(CFLAGS) $(LIBS)

test:  test.c teletext_test.c teletext.h test.h
	$(CC) test.c teletext_test.c -o test $(CFLAGS) $(LIBS)


extension: teletext_extension.c teletext_extension.h
	$(CC) teletext_extension.c -o extension $(CFLAGS) $(LIBS)



