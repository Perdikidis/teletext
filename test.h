#include "teletext.h"

#define FAIL 1
#define PASS 0
#define CLOSED 0
#define WINDOW 0
#define BOARD_START 1
#define FINISH 2
#define WHITESTR  "\x1B[0m"
#define REDSTR  "\x1B[31m"
#define GRNSTR  "\x1B[32m"




void DrawChar(Window *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, int ox, int oy){

   unsigned x, y;

   for(y = 0; y < FNTHEIGHT; y++){
      for(x = 0; x < FNTWIDTH; x++){
         if(fontdata[chr-FNT1STCHAR][y] >> (FNTWIDTH - 1 - x) & 1){
            SDL_SetRenderDrawColor(sw->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy);
         }
      }
   }
}
void DrawString(Window *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], char *str, int ox, int oy){

   int i=0;
   unsigned char chr;
   do{
      chr = str[i++];
      DrawChar(sw, fontdata, chr, ox+i*FNTWIDTH, oy);
   }while(str[i]);
}
int popup(int test);

int test_read_file(int *tot_tests);
int test_event_handling(int *tot_tests, Window testWin);
int test_init_window( int *tot_tests,  fntrow fontdata[FNTCHARS][FNTHEIGHT]);
int test_select_colour( int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]);
int test_light_pixel(int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]);
int test_Vlasis_draw_rect(int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]);
int test_draw_cell(int *tot_tests, int *tot_errors, fntrow fontdata[FNTCHARS][FNTHEIGHT]);
int test_set_pixels_zero(int *tot_tests);
int test_new_line_settings(int *tot_tests);
int test_exam_cases(int *tot_tests);
int test_set_foreground(int *tot_tests);
int test_set_background(int *tot_tests);
int test_encode_graphics(int *tot_tests);
int test_set_character(int *tot_tests);
int test_set_current_cell(int *tot_tests);
int test_set_board(int *tot_tests);
int test_print_board(int *tot_tests);
