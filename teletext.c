#include "teletext.h"

int main(void)
{
   Cell board[HEIGHT][WIDTH];

   read_file("lfc.m7.txt", board);
   set_board(board);
   printf("\n\n\nTwra exoume to board\n\n\n\n" );
   print_board(board);
   return 0;
}


void read_file(char *fileName, Cell board[HEIGHT][WIDTH]){

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

void set_board( Cell board[HEIGHT][WIDTH]){

   int i, j, prevFore, prevBack, prevMode;

   for (i=0; i<HEIGHT; i++){
      for (j=0; j<WIDTH; j++){
         if (j == 0){ /* set default settings in new line */
            board[i][j] = reset_settings(board[i][j]);
            prevFore = board[i][j].foreColour;
            prevBack = board[i][j].backColour;
            prevMode = board[i][j].Mode;
         }
         /* the range of alphanumeric codes */
         if (( board[i][j].character >= 0x81) && (board[i][j].character <= 0x87)){
            board[i][j].Mode = Alphanumeric;
            prevMode = Alphanumeric;
         }/* the range of graphic codes */
         if (( board[i][j].character >= 0x91) && (board[i][j].character <= 0x97)){
            board[i][j].Mode =  Graphics;
            prevMode = Graphics;
         }
         board[i][j].Mode = prevMode;
         board[i][j] = set_foreground(board[i][j], &prevFore);
         board[i][j] = set_background(board[i][j], &prevBack, prevFore);
         board[i][j] = set_character(board[i][j]);
      }
   }
}

Cell reset_settings( Cell c){

   c.foreColour = WHITE;
   c.backColour = BLACK;
   c.Height = Single;
   c.Mode = Alphanumeric;
   c.Type = Contiguous;
   return c;
}

Cell set_foreground(Cell c, int *prevFore){

   /* get new colour if the code is right*/
   if (( c.character >= RED) && ( c.character <= WHITE)){
      c.foreColour = (int) c.character;
   }
   else if ((c.character >= 0x91) && (c.character <= 0x97)){
      c.foreColour = (int) c.character - 16;
   }
   else{ /* else retain the previous one */
      c.foreColour = *prevFore;
   }
   /* set the previus foreground colour for our next cell */
   *prevFore = c.foreColour;
   return c;
}

Cell set_background(Cell c , int *prevBack, int prevFore){

   if ( c.character == BLACKBACKGROUND){
      c.backColour = BLACK;
   } /*new background will get the colour of the previous foreground colour*/
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
   if (c.Mode == Graphics){
      /* Everything, but the blast through text*/
      if ((c.character < 0xc0) || (c.character >=0xe0) ){
         c = encode_graphics(c);
         c.character = SPACE;
      }
   }
   return c;
}

Cell encode_graphics(Cell c){

   int code = c.character - BASECODE;


   c.pixel.r_bot = code / 64;/* check if the bottom right sixel is active */
   code = code % 64;/* code gets the remainder of the division */
   /* doing the same till the top left sixel */
   c.pixel.l_bot = code / 16;
   code = code % 16;
   c.pixel.r_mid = code / 8;
   code = code % 8;
   c.pixel.l_mid = code / 4;
   code = code % 4;
   c.pixel.r_top = code / 2;
   code = code % 2;
   c.pixel.l_top = code / 1;
   /* I know that these are magic numbers from the sixels
   But when I #defined them, they were uglier and less readable */
   return c;

}

void print_board(Cell board[HEIGHT][WIDTH]){

   int i, j;
   SDL_Simplewin sw;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];


   SDL_Init(SDL_INIT_EVERYTHING);
   Neill_SDL_Init(&sw);
   Neill_SDL_ReadFont(fontdata, FNTFILENAME);
   /*edw kanoume draw to background kai*/
   for (i=0; i<HEIGHT; i++){
      printf("\n");
      for (j=0; j<WIDTH; j++){
         Neill_SDL_DrawCell(&sw, fontdata, board[i][j]  , j*FNTWIDTH, FNTHEIGHT*i);
         Vlasis_draw_rect(&sw, board[i][j], i*FNTHEIGHT, j*FNTWIDTH);
         SDL_RenderPresent(sw.renderer);
         printf("%c",/*(int) board[i][j]*/board[i][j].character - 128 );
      }
   }
   SDL_UpdateWindowSurface(sw.win);
   SDL_Delay(100000);

   SDL_Quit();
}

void Vlasis_draw_rect(SDL_Simplewin *sw, Cell c, int y, int x){

   if( c.pixel.l_top == ACTIVE){
      light_pixel(sw, y, x, c.foreColour);
   }
   if( c.pixel.l_mid == ACTIVE){
      light_pixel(sw, y + PIXEL_H, x, c.foreColour);
   }
   if( c.pixel.l_bot == ACTIVE){
      light_pixel(sw, y + (2*PIXEL_H), x, c.foreColour);
   }
   if( c.pixel.r_top == ACTIVE){
      light_pixel(sw, y, x + PIXEL_W, c.foreColour);
   }
   if( c.pixel.r_mid == ACTIVE){
      light_pixel(sw, y + PIXEL_H, x + PIXEL_W, c.foreColour);
   }
   if( c.pixel.r_bot == ACTIVE){
      light_pixel(sw, y + (2*PIXEL_H), x + PIXEL_W, c.foreColour);
   }

}

void light_pixel(SDL_Simplewin *sw, int y, int x, int colour){

   SDL_Rect rect;

   rect.x = x;
   rect.y = y;
   rect.w = PIXEL_W;
   rect.h = PIXEL_H;
   select_colour(sw, colour);
   SDL_RenderFillRect( sw->renderer, &rect);
}

void Neill_SDL_Init(SDL_Simplewin *sw){


   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   sw->finished = 0;

   sw->win= SDL_CreateWindow("Vlasis Teletext",
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
      case RED:/*                    R,   G, B */
         Neill_SDL_SetDrawColour(sw, 255, 0, 0);
         break;
      case GREEN: /* edw na valw green or greengraphics */
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
