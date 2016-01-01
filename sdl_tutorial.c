#include <SDL/SDL.h>
//#include "neillsdl2.h"

#define TRUE 1
#define FALSE 0

typedef enum direction{Up, Left, Down, Right} Direction;

SDL_Rect init_rect(SDL_Rect rect);

int main(void)
{

   int running = TRUE, arrow[4]={0,0,0,0};
   int32_t start, color, color2;
   const int FPS = 30;
   SDL_Rect rect; // auto einai struct tou sdl
   rect = init_rect(rect);//give coordinates and size

   SDL_Init(SDL_INIT_EVERYTHING);
   //prepei prwta na kanw SDL_Init kai meta na dhlwsw to screen
   SDL_Surface *screen, *image;
   screen = SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
   // auto to function kanei automata to xrwma convert me to screen
   color=SDL_MapRGB(screen->format, 0x00, 0xff, 100);//U can use both hex and decimal numbers, as shown here
   color2=SDL_MapRGB(screen->format, 0xa0, 0, 0);
   // to display format einai gia na einai idio me to screen kai na trexei pio grigora
   image = SDL_DisplayFormat(SDL_LoadBMP("Red_Elf_Front.bmp"));
   if ( image == NULL){
      fprintf(stderr, "couldn't load image");
      return (1);
   }
                  //για να κανει transperant το background
   SDL_SetColorKey(image,SDL_SRCCOLORKEY, SDL_MapRGB(screen->format,0x00,0xff,0xff));
   while (running == TRUE){
      start = SDL_GetTicks();
      SDL_Event event;
      while(SDL_PollEvent(&event)){
         switch(event.type){
            case SDL_QUIT:
               running = FALSE;
               break;
            case SDL_KEYDOWN:
               switch(event.key.keysym.sym){
                  case SDLK_UP:
                     arrow[Up] = TRUE;
                     break;
                  case SDLK_LEFT:
                     arrow[Left] = TRUE;
                     break;
                  case SDLK_DOWN:
                     arrow[Down] = TRUE;
                     break;
                  case SDLK_RIGHT:
                     arrow[Right] = TRUE;
                     break;
               }
               break;
            case SDL_KEYUP:
               switch(event.key.keysym.sym){
                  case SDLK_UP:
                     arrow[Up] = FALSE;
                     break;
                  case SDLK_LEFT:
                     arrow[Left] = FALSE;
                     break;
                  case SDLK_DOWN:
                     arrow[Down] = FALSE;
                     break;
                  case SDLK_RIGHT:
                     arrow[Right] = FALSE;
                     break;
               }
               break;
         }
      }
      //logic
      if (arrow[Up]){ rect.y--; }
      if (arrow[Left]){ rect.x--; }
      if (arrow[Down]){ rect.y++; }
      if (arrow[Right]){ rect.x++; }
      // render
      // το ίδιο αλλά εδώ οι διαστάσεις είναι όλη η οθόνη δλδ. 640Χ480
      SDL_FillRect(screen, &screen->clip_rect, color);
      SDL_Rect paok;// declare rect
      paok.x=200;
      paok.y=100;

      SDL_Rect first;// declare rect
      first.x=0;
      first.y=0;
      first.w=33;
      first.h=64;
      SDL_BlitSurface(image,&first,screen,&paok);

      first.x=33;
      first.y=0;
      first.w=33;
      first.h=64;
                  // ,what,διαστασεις,where,coordinates
      SDL_BlitSurface(image,&first,screen,NULL);
      //FillRect param: where to draw, rect, color
      SDL_FillRect(screen, &rect, color2);
      SDL_Flip(screen);
      // delay FPS so the CPU den zorizetai
       if ( 1000/FPS > SDL_GetTicks() - start){
          SDL_Delay(1000/FPS-(SDL_GetTicks() - start));
       }
   }
   SDL_FreeSurface(image);
   SDL_Quit();
   return 0;
}


SDL_Rect init_rect(SDL_Rect rect){

   rect.x = 50;
   rect.y = 10;
   rect.w = 20;
   rect.h = 20;
   return rect;
}
