#include "teletext.h"

int main(void)
{

   Cell board[HEIGHT][WIDTH];
   int a=20, test=0;

   read_file("lfc.m7.txt", board);
   set(board);
   printf("\n\n\nTwra exoume to board\n\n\n\n" );
   print_board(board);
   return 0;
}


void read_file(char *fileName, Cell board[HEIGHT][WIDTH]){

   char c;
   int i, j;
   size_t items;
   FILE *fp;

   fp = fopen(fileName, "rb");

   if (fp == NULL){
      fprintf(stderr, "Can't open Font file %s\n", fileName);
      exit(1);
   }
   for (i=0; i<HEIGHT; i++){
      printf("\n");
      for (j=0; j<WIDTH; j++){
         items = fread(&board[i][j].character, sizeof(unsigned char), 1, fp);
         if ( items != 1){
            fprintf(stderr, "couldn't read this item");
            exit(1);
         }
         /* we want all our characters to be above 0x80 */
         if( board[i][j].character < 128 ){
            board[i][j].character += 128;
         }
         printf("%d,", board[i][j].character /*, board[i][j]-128)*/);
      }
   }
   fclose(fp);
}

void set( Cell board[HEIGHT][WIDTH]){

   int i, j, prevFore, prevBack;

   for (i=0; i<HEIGHT; i++){
      for (j=0; j<WIDTH; j++){
         if (j == 0){
            board[i][j] = reset_settings(board[i][j]);
            prevFore = board[i][j].foreColour;
            prevBack = board[i][j].backColour;
         }
         board[i][j] = set_foreground(board[i][j], &prevFore);
         board[i][j] = set_background(board[i][j], &prevBack, prevFore);
         board[i][j] = set_character(board[i][j]);
      }
   }
}

 /* isws xreiastei na kanw to c pointer 'h na to valw na epistrefei cell */
Cell reset_settings( Cell c){

   c.foreColour = WHITE;
   c.backColour = BLACK;
   c.Height = Single;
   return c;
}

Cell set_foreground(Cell c, int *prevFore){

   if (( c.character >= 0x81) && ( c.character <= 0x87)){
      c.foreColour = (int) c.character;
   }
   else{
      c.foreColour = *prevFore;
   }
   /* set the previus foreground colour for our next cell */
   *prevFore = c.foreColour;
   return c;
}

Cell set_background(Cell c , int *prevBack, int prevFore){

   if ( c.character == BLACKBACKGROUND){
      c.backColour = BLACK;
   }
   else if ( c.character == NEWBACKGROUND){
      c.backColour = prevFore;
   }
   else{
      c.backColour = *prevBack;
   }
   /* set the previus background colour for our next cell */
   *prevBack = c.backColour;
   return c;
}

Cell set_character(Cell c){

   /* so we don't print the control codes */
   if ( c.character < SPACE){
      c.character = SPACE;
   }
   return c;
}

void print_board(Cell board[HEIGHT][WIDTH]){

   int i, j;
   SDL_Simplewin sw;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];

   SDL_Init(SDL_INIT_EVERYTHING);
   Neill_SDL_Init(&sw);
   Neill_SDL_ReadFont(fontdata, FNTFILENAME);
   for (i=0; i<HEIGHT; i++){
      printf("\n");
      for (j=0; j<WIDTH; j++){
         Neill_SDL_DrawCell(&sw, fontdata, board[i][j]  , j*FNTWIDTH, 20*i);
         SDL_RenderPresent(sw.renderer);
         printf("%c",/*(int) board[i][j]*/board[i][j].character - 128 );
      }
   }
   SDL_UpdateWindowSurface(sw.win);
   SDL_Delay(10000);

   SDL_Quit();
}

void Neill_SDL_Init(SDL_Simplewin *sw){


   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   sw->finished = 0;

   sw->win= SDL_CreateWindow("SDL Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WWIDTH, WHEIGHT,
                          SDL_WINDOW_SHOWN);
   if(sw->win == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   sw->renderer = SDL_CreateRenderer(sw->win, -1, 0);
   if(sw->renderer == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   /* Set screen to black */
   Neill_SDL_SetDrawColour(sw, 0, 100, 0);
   SDL_RenderClear(sw->renderer);
   SDL_RenderPresent(sw->renderer);

}

void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b){

   SDL_SetRenderDrawColor(sw->renderer, r, g, b, SDL_ALPHA_OPAQUE);

}

void Neill_SDL_DrawCell(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Cell c, int ox, int oy){

   unsigned x, y;
   unsigned char chr = c.character -128 ;

   for(y = 0; y < FNTHEIGHT; y++){
      for(x = 0; x < FNTWIDTH; x++){
         if(fontdata[chr-FNT1STCHAR][y] >> (FNTWIDTH - 1 - x) & 1){
            /* foreground */
            select_colour(sw, c.foreColour);
            SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy);
         }
         else{/*background*/
            select_colour(sw, c.backColour);
            SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy);
         }
      }
   }
}

void select_colour( SDL_Simplewin *sw, int colour ){

   switch (colour) {
      case RED:/*               R,   G, B */
         Neill_SDL_SetDrawColour(sw, 255, 0, 0);
         break;
      case GREEN:
         Neill_SDL_SetDrawColour(sw, 0, 255, 0);
         break;
      case YELLOW:
         Neill_SDL_SetDrawColour(sw, 255, 255, 0);
         break;
      case BLUE:
         Neill_SDL_SetDrawColour(sw, 0, 0, 255);
         break;
      case MAGENTA:
         Neill_SDL_SetDrawColour(sw, 255, 0, 127);
         break;
      case CYAN:
         Neill_SDL_SetDrawColour(sw, 0, 255, 255);
         break;
      case WHITE:
         Neill_SDL_SetDrawColour(sw, 255, 255, 255);
         break;
      case BLACK:
         Neill_SDL_SetDrawColour(sw, 0, 0, 0);
         break;
   }

}

void Neill_SDL_ReadFont(fntrow fontdata[FNTCHARS][FNTHEIGHT], char *fname){

    FILE *fp = fopen(fname, "rb");
    size_t itms;
    if(!fp){
       fprintf(stderr, "Can't open Font file %s\n", fname);
       exit(1);
   }
   itms = fread(fontdata, sizeof(fntrow), FNTCHARS*FNTHEIGHT, fp);
   if(itms != FNTCHARS*FNTHEIGHT){
       fprintf(stderr, "Can't read all Font file %s (%d) \n", fname, (int)itms);
       exit(1);
   }
   fclose(fp);

}
