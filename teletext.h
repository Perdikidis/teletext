/*testarw to push kai add
kai ksana dokimazw
kai trith fora
kai tetarth fora*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <SDL2/SDL.h>


#define DIST_GRAPH_TO_ALPHA 16
#define HEIGHT 25
#define WIDTH 40
#define SINGLE 0x8C
#define DOUBLE 0x8D
#define RED_GRAPH 0x91
#define WHITE_GRAPH 0x97
#define SEPARATED 0x9A
#define BLACKBACKGROUND 0x9C
#define NEWBACKGROUND 0x9D
#define HOLD_GRAPHICS 0x9E
#define RELEASE_GRAPHICS 0x9F
#define SPACE 0xa0
#define BASECODE 160
#define ACTIVE 1
#define VISABLE_ASCII 128

/* Font stuff */
typedef unsigned short fntrow;

#define FNTFILENAME "m7fixed.fnt"
#define FNTWIDTH 16
#define FNTHEIGHT 18
#define FNTCHARS 96
#define FNT1STCHAR 32
#define PIXEL_H FNTHEIGHT/3
#define PIXEL_W FNTWIDTH/2
#define PIXEL_DISTANCE 2
#define WINDOW_WIDTH 40 * FNTWIDTH
#define WINDOW_HEIGHT 25 * FNTHEIGHT

enum colour {BLACK=0x80 ,RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE};
typedef enum mode {Alphanumeric, Graphics} Mode;
typedef enum type {Contiguous, Separated} Type;
typedef enum height {Single, Double} Height;
typedef enum held_graph {No, Yes} Held_graph;

typedef struct sixel_rect {
   int l_top, r_top;
   int l_mid, r_mid;
   int l_bot, r_bot;
} Sixel_rect;

typedef struct cell {
   unsigned char character;
   int foreColour;
   int backColour;
   int Mode;
   int Type;
   int Height;
   int Held_graph;
   Sixel_rect pixel;
} Cell;

typedef struct window {
   SDL_Window *win;
   SDL_Renderer *renderer;
} Window ;

int test();
void read_file (char *filename, Cell board[HEIGHT][WIDTH]);
void set_board( Cell board[HEIGHT][WIDTH]);
Cell set_pixels_zero( Cell c);/*DONE*/
Cell new_line_settings( Cell c, Cell *prev);
Cell exam_cases(Cell c, Cell *prev);
Cell set_current_cell(Cell c, Cell *prev, unsigned char *last_graph);
Cell set_foreground(Cell c, int *prevFore);
Cell set_background(Cell c , int *prevBack, int prevFore);
Cell set_character(Cell c, unsigned char *last_graph);
Cell encode_graphics(Cell old, unsigned char chr);
void print_board(Cell board[HEIGHT][WIDTH]);
void init_window(Window *sw);/* DONE */
void read_font(fntrow fontdata[FNTCHARS][FNTHEIGHT], char *fname);
void draw_cell(Window *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Cell c, int ox, int oy, int above_height);
void draw_background(Window *sw, int colour, int x, int y);
void draw_foreground(Window *sw, Cell c, int x, int y, int i);
void select_colour( Window *sw, int colour );/* DONE */
void set_colour(Window *sw, Uint8 r, Uint8 g, Uint8 b);
void Vlasis_draw_rect(Window *sw, Cell c, int y, int x);/*DONE*/
void light_pixel(Window *sw, int y, int x, int colour, int type);/*DONE*/
