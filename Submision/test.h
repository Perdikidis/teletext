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



/* 3 new functions that help me with my tests*/
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
void DrawString(Window *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], const char *str, int ox, int oy){

   int i=0;
   unsigned char chr;
   do{
      chr = str[i++];
      DrawChar(sw, fontdata, chr, ox+i*FNTWIDTH, oy);
   }while(str[i]);
}
int popup(int test){

   Window popUp;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];
   int line=0, button=0;

   read_font(fontdata, FNTFILENAME);
   SDL_Init(SDL_INIT_EVERYTHING);
   popUp.win= SDL_CreateWindow("popup message",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          800, 300,
                          SDL_WINDOW_SHOWN);

   popUp.renderer = SDL_CreateRenderer(popUp.win, -1, 0);
   SDL_SetRenderDrawColor(popUp.renderer, 0, 100, 0, SDL_ALPHA_OPAQUE);
   SDL_RenderClear(popUp.renderer);
   SDL_RenderPresent(popUp.renderer);

   switch (test) {
      case WINDOW:
         DrawString(&popUp, fontdata,"Expected result from next test:", 0, FNTHEIGHT * ++line);
         DrawString(&popUp, fontdata,"to draw a black window named ", 0, FNTHEIGHT * ++line);
         DrawString(&popUp, fontdata,"Vlasis' Teletext with size: 640x450", 0, FNTHEIGHT * ++line);
         DrawString(&popUp, fontdata,"     if ready press space", 0, 250);
         break;
      case BOARD_START:
         DrawString(&popUp, fontdata,"Expected result from next test:", 0, FNTHEIGHT * ++line);
         DrawString(&popUp, fontdata,"draw a board with characters from ", 0, FNTHEIGHT * ++line);
         DrawString(&popUp, fontdata,"0x80 to 0xff and all over again in ", 0, FNTHEIGHT * ++line);
         DrawString(&popUp, fontdata,"in the appropriate manner ", 0, FNTHEIGHT * ++line);
         DrawString(&popUp, fontdata,"     if ready press space", 0, 250);
         break;
      case FINISH:
         DrawString(&popUp, fontdata,"was the test successful? y/n", 0, FNTHEIGHT * ++line);
      break;
   }
   SDL_RenderPresent(popUp.renderer);
   SDL_UpdateWindowSurface(popUp.win);
   do{
      event_handling(&button);
   }while((button!=SPACE) && (button!='y') && (button!='n'));
   SDL_DestroyWindow(popUp.win);
   SDL_Quit();
   return button;
}

/* Teletext's functions*/
int test_read_file(int *tot_tests);
int test_event_handling(int *tot_tests, Window testWin);
int test_init_window( int *tot_tests);
int test_select_colour( int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]);
int test_light_pixel(int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]);
int test_Vlasis_draw_rect(int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]);
int test_draw_cell(int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]);
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
