#include "teletext.h"

#define FAIL 1
#define PASS 0
#define CLOSED 0
/*#define ESCAPE 2
#define SPACE 3*/
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
void popup();
int did_test_passed();

int test_event_handling(int *tot_tests, Window testWin);
int test_init_window( int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]);
int test_select_colour( int *tot_tests);
int test_light_pixel(int *tot_tests);
int test_Vlasis_draw_rect(int *tot_tests);
int test_draw_background(int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]);
