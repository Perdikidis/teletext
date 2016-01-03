#include "teletext.h"

int main(void)
{

   unsigned char board[HEIGHT][WIDTH];
   int a=20, test=0;

   read_file("lfc.m7.txt", board);
   printf("\n\n\nTwra exoume to board\n\n\n\n" );
   print_board(board);
   printf("\n\n\n\n\n" );
   printf("a=%d\n", a);
   a = a << 2;
   printf("a=%d\n", a);
   test = 0x81;
   printf("test is:%d\n\n",test );
   return 0;
}


void read_file(char *fileName, unsigned char board[HEIGHT][WIDTH]){

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
      for (j=0; j<WIDTH; j++){
         items = fread(&board[i][j], sizeof(unsigned char), 1, fp);
         if ( items != 1){
            fprintf(stderr, "couldn't read this item");
            exit(1);
         }
         if( board[i][j] < 128 ){
            board[i][j] += 128;
         }
         printf("%d:%c ", board[i][j], board[i][j]-128);
      }
   }
   fclose(fp);
}

void print_board(unsigned char board[HEIGHT][WIDTH]){

   int i, j;
   /*SDL_Color TextColor = {255,00,00,255};*/
   int32_t color;
   SDL_Simplewin d;
   SDL_Rect rect;
   SDL_Surface *screen;
   SDL_Renderer *rend;

   SDL_Init(SDL_INIT_EVERYTHING);

   SDL_Simplewin sw;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];
   Neill_SDL_Init(&sw);
   Neill_SDL_ReadFont(fontdata, FNTFILENAME);
   /*screen = SDL_SetVideoMode(WWIDTH,WHEIGHT,32,SDL_SWSURFACE);*/
   /*screen = SDL_GetWindowSurface(sw.win);

   printf("mpikame edw\n" );
   color = SDL_MapRGB( screen->format, 255, 0, 0);




   /*SDL_SetRenderDrawColor(d.renderer, 255, 0, 0, 255);*/

   for (i=0; i<HEIGHT; i++){
      printf("\n");
      for (j=0; j<WIDTH; j++){
         if ((int) board[i][j] == 0x81){
            Neill_SDL_DrawChar(&sw, fontdata, 'R', j*FNTWIDTH, 20*i);
         }
         else if((int) board[i][j] == 0x91){
            rect.x = i;
            rect.y = j;
            rect.w = FNTWIDTH;
            rect.h = 20;
            /*SDL_FillRect(screen, &rect, color);*/
            Neill_SDL_RenderDrawCircle(sw.renderer, i, j, 10 );
            Neill_SDL_RenderFillCircle(sw.renderer, i+50, j+50, 10 );
         }
         else{
            Neill_SDL_DrawChar(&sw, fontdata, board[i][j] -128  , j*FNTWIDTH, 20*i);
         }
         SDL_RenderPresent(sw.renderer);
         printf("%c",/*(int) board[i][j]*/board[i][j] - 128 );
      }
   }
   SDL_UpdateWindowSurface(sw.win);
   SDL_Delay(10000);

   SDL_Quit();
}
