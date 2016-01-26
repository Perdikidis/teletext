#include "teletext.h"

int main(int argc, char *argv[])
{
   Cell board[HEIGHT][WIDTH];

   if (argc != 2){
      fprintf(stderr,"you need to give 2 arguments\n");
      exit(1);
   }
   if (test() != 0 ){
      printf("Testing failed\n\n\n");
      exit (1);
   }
   read_file(argv[1], board);
   set_board(board);
   print_board(board);
   return 0;
}

void read_file(char *fileName, Cell board[HEIGHT][WIDTH]){

   int i, j;
   size_t items;
   FILE *fp;

   fp = fopen(fileName, "rb");

   if (fp == NULL){
      fprintf(stderr, "Can't open this file %s\n", fileName);
      exit(1);
   }
   for (i=0; i<HEIGHT; i++){
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
      }
   }
   fclose(fp);
}

void set_board( Cell board[HEIGHT][WIDTH]){

   unsigned char last_graph = SPACE;
   int i, j;
   Cell prev;

   for (i=0; i<HEIGHT; i++){
      for (j=0; j<WIDTH; j++){
         board[i][j] = set_pixels_zero(board[i][j]);
         if (j == 0){
            /* set default settings for new line */
            board[i][j] = new_line_settings(board[i][j], &prev);
         }
         board[i][j] = exam_cases(board[i][j], &prev);
         board[i][j] = set_current_cell(board[i][j], &prev, &last_graph);
      }
   }
}

Cell set_pixels_zero( Cell c){

   c.pixel.l_top = 0;
   c.pixel.l_mid = 0;
   c.pixel.l_bot = 0;
   c.pixel.r_top = 0;
   c.pixel.r_mid = 0;
   c.pixel.r_bot = 0;
   return c;
}

Cell new_line_settings( Cell c, Cell *prev){

   prev->foreColour = c.foreColour = WHITE;
   prev->backColour = c.backColour = BLACK;
   prev->Height = c.Height = Single;
   prev->Mode = c.Mode = Alphanumeric;
   prev->Type = c.Type = Contiguous;
   prev->Held_graph = c.Held_graph = No;
   return c;
}

Cell exam_cases(Cell c, Cell *prev){

   if (c.character == SINGLE){
      prev->Height = c.Height = Single;
      prev->Held_graph = c.Held_graph = No;
   }
   if (c.character == DOUBLE){
      prev->Height = c.Height = Double;
      prev->Held_graph = c.Held_graph = No;
   }
   /* the range of alphanumeric colour code */
   if (( c.character >= RED) && (c.character <= WHITE)){
      prev->Mode = c.Mode = Alphanumeric;
      prev->Held_graph = c.Held_graph = No;
   }/* the range of graphic colour codes */
   if (( c.character >= RED_GRAPH) && (c.character <= WHITE_GRAPH)){
      prev->Mode = c.Mode =  Graphics;
      prev->Type = c.Type = Contiguous;
   }
   if( c.character == SEPARATED){
      prev->Mode = c.Mode =  Graphics;
      prev->Type = c.Type = Separated;
   }
   if( c.character == HOLD_GRAPHICS){
      prev->Held_graph = c.Held_graph = Yes;
   }
   if( c.character == RELEASE_GRAPHICS){
      prev->Held_graph = c.Held_graph = No;
   }
   return c;
}

Cell set_current_cell(Cell c, Cell *prev, unsigned char *last_graph){

   c.Held_graph =  prev->Held_graph;
   c.Type = prev->Type;
   c.Height = prev->Height;
   c.Mode = prev->Mode;
   c = set_foreground(c, &prev->foreColour);
   c = set_background(c, &prev->backColour, prev->foreColour);
   c = set_character(c, last_graph);
   return c;
}

Cell set_foreground(Cell c, int *prevFore){

   /* new text colour*/
   if (( c.character >= RED) && ( c.character <= WHITE)){
      c.foreColour =  c.character;
   }
   /* new graphics colour*/
   else if ((c.character >= RED_GRAPH) && (c.character <= WHITE_GRAPH)){
      c.foreColour =  c.character - DIST_GRAPH_TO_ALPHA;
   }
   else{ /* else retain previous colour*/
      c.foreColour = *prevFore;
   }
   /*set prevFore colour to use on next cell */
   *prevFore = c.foreColour;
   return c;
}

Cell set_background(Cell c , int *prevBack, int prevFore){

   if ( c.character == BLACKBACKGROUND){
      c.backColour = BLACK;
   }
    /*set background to previous foreground colour*/
   else if ( c.character == NEWBACKGROUND){
      c.backColour = prevFore;
   }
   else{/*else retain previous colour*/
      c.backColour = *prevBack;
   }
   /*set prevBack colour to use on next cell */
   *prevBack = c.backColour;
   return c;
}

Cell set_character(Cell c, unsigned char *last_graph){

   /*so we don't print the control codes*/
   if ( c.character < SPACE){
      c.character = SPACE;
   }
   if (c.Mode == Graphics){
      /* Everything, but the blast through text*/
      if ((c.character < 0xc0) || (c.character >=0xe0) ){
         if( c.Held_graph == Yes ){
            c = encode_graphics(c, *last_graph);
            c.character = *last_graph;
         }
         else{
            c = encode_graphics(c, c.character);
         }
         /*I will use last_graph for the hold graphics mode*/
         *last_graph = c.character;
         /*don't want to print anything else apart from the graphics*/
         c.character = SPACE;
      }
   }
   return c;
}

Cell encode_graphics(Cell old, unsigned char chr){

   Cell c = old;
   int code = chr - BASECODE;

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

   int i, y, j, x, height_of_above, button;
   Window sw;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];

   if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
     fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
     SDL_Quit();
     exit(1);
   }
   init_window(&sw);
   read_font(fontdata, FNTFILENAME);
   for (i=0; i<HEIGHT; i++){
      for (j=0; j<WIDTH; j++){
         height_of_above = board[i-1][j].Height;
         y = FNTHEIGHT * i;     x = j * FNTWIDTH;
         draw_cell(&sw, fontdata, board[i][j], x, y, height_of_above);
         if (board[i][j].Mode == Graphics){
            Vlasis_draw_rect(&sw, board[i][j], y, x);
         }
         SDL_RenderPresent(sw.renderer);
         SDL_Delay(5);
      }
   }
   while ((button != ESCAPE) && (button != SPACE)){
      event_handling(&button);
   }
   SDL_DestroyWindow(sw.win);
   SDL_Quit();
}

void init_window(Window *sw){

   sw->win= SDL_CreateWindow("Vlasis' Teletext",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WINDOW_WIDTH, WINDOW_HEIGHT,
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
   /*set screen black*/
   if(SDL_SetRenderDrawColor(sw->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0){
      fprintf(stderr, "\nUnable to draw colour:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }
   if(SDL_RenderClear(sw->renderer) !=0 ){
      fprintf(stderr, "\nUnable to clear render:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }
   SDL_RenderPresent(sw->renderer);
}

void read_font(fntrow fontdata[FNTCHARS][FNTHEIGHT], char *fname){

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

void draw_cell(Window *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], Cell c, int x, int y, int height_of_above){

   unsigned char chr = c.character - VISABLE_ASCII;
   int i=0, j, height_to_draw = FNTHEIGHT, width_to_draw = FNTWIDTH;

   draw_background(sw, c.backColour, x, y);
   /* and now the foreground */
   if (c.Height == Double){
      height_to_draw /= 2;
      if ( height_of_above == Double){
         i = height_to_draw;/*set starting point in the mid of letter*/
         height_to_draw += i; /* set ending point*/
         y -= FNTHEIGHT; /*shift the half upwards*/
      }
   }
   for( ; i < height_to_draw; i++){
      for(j=0 ; j < width_to_draw; j++){
         if(fontdata[chr-FNT1STCHAR][i] >> (FNTWIDTH - 1 - j) & 1){
            draw_foreground(sw, c, x, j, y, i);
         }
      }
   }
}

void draw_background(Window *sw, int colour, int x, int y){

   int i, j;
   for(i = 0; i < FNTHEIGHT; i++){
      for(j = 0; j < FNTWIDTH; j++){
         select_colour(sw, colour);
         if(SDL_RenderDrawPoint(sw->renderer, x + j, y + i) != 0){
            fprintf(stderr, "\nUnable to draw point:  %s\n", SDL_GetError());
            SDL_Quit();
            exit(1);
         }
      }
   }
}

void draw_foreground(Window *sw, Cell c, int x, int j, int y, int i){

   int error=0;

   select_colour(sw, c.foreColour);
   if (c.Height == Single){
      error += SDL_RenderDrawPoint(sw->renderer, x + j , y + i );
   }
   else{
      error += SDL_RenderDrawPoint(sw->renderer, x + j, y + i*2);
      error += SDL_RenderDrawPoint(sw->renderer, x + j, y +  i*2+1);
   }
   if(error != 0){
      fprintf(stderr,"unable to draw point");
   }
}

void select_colour( Window *sw, int colour ){

   int error=0;

   switch (colour) {
      case RED:                             /* R   G  B */
         error += SDL_SetRenderDrawColor(sw->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
         break;
      case GREEN:
         error += SDL_SetRenderDrawColor(sw->renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
         break;
      case YELLOW:
         error += SDL_SetRenderDrawColor(sw->renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
         break;
      case BLUE:
         error += SDL_SetRenderDrawColor(sw->renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
         break;
      case MAGENTA:
         error += SDL_SetRenderDrawColor(sw->renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
         break;
      case CYAN:
         error += SDL_SetRenderDrawColor(sw->renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
         break;
      case WHITE:
         error += SDL_SetRenderDrawColor(sw->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
         break;
      case BLACK:
         error += SDL_SetRenderDrawColor(sw->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
         break;
   }
   if(error !=0){
      fprintf(stderr, "unable to render draw colour");
   }
}

void Vlasis_draw_rect(Window *sw, Cell c, int y, int x){

   if( c.pixel.l_top == ACTIVE){
      light_pixel(sw, y, x, c.foreColour, c.Type);
   }
   if( c.pixel.l_mid == ACTIVE){
      light_pixel(sw, y + PIXEL_H, x, c.foreColour, c.Type);
   }
   if( c.pixel.l_bot == ACTIVE){
      light_pixel(sw, y + (2*PIXEL_H), x, c.foreColour, c.Type);
   }
   if( c.pixel.r_top == ACTIVE){
      light_pixel(sw, y, x + PIXEL_W, c.foreColour, c.Type);
   }
   if( c.pixel.r_mid == ACTIVE){
      light_pixel(sw, y + PIXEL_H, x + PIXEL_W, c.foreColour, c.Type);
   }
   if( c.pixel.r_bot == ACTIVE){
      light_pixel(sw, y + (2*PIXEL_H), x + PIXEL_W, c.foreColour, c.Type);
   }

}

void light_pixel(Window *sw, int y, int x, int colour, int type){

   SDL_Rect rect;
   int pixel_w = PIXEL_W , pixel_h = PIXEL_H;

   if (type == Separated){
      pixel_h -=  PIXEL_DISTANCE;
      pixel_w -=  PIXEL_DISTANCE;
   }
   rect.x = x;
   rect.y = y;
   rect.w = pixel_w;
   rect.h = pixel_h;
   select_colour(sw, colour);
   if(SDL_RenderFillRect( sw->renderer, &rect) != 0){
      fprintf(stderr, "\nUnable to fill rect: %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }
}

int event_handling(int *button){

   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
      switch (event.type){
        case SDL_KEYDOWN:
          switch( event.key.keysym.sym ){
            case SDLK_y:
              *button='y';
              break;
            case SDLK_n:
              *button='n';
              break;
            case SDLK_ESCAPE:
               *button = ESCAPE;
               break;
            case SDLK_SPACE:
               *button = SPACE;
               break;
          }
        break;
      }
      return(0);
   }
   return(0);
}
