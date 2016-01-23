#include "test.h"



int test(void)
{

   int tot_errors=0, tot_tests=0;
   Window testWin;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];

   read_font(fontdata, FNTFILENAME);
   printf("\n\nTESTING!!!!\n\n\n");
   /*
   tot_errors += test_event_handling( &tot_tests, testWin);
   tot_errors += test_init_window( &tot_tests, fontdata);
   tot_errors += test_select_colour( &tot_tests);
   tot_errors += test_light_pixel(&tot_tests);
   tot_errors += test_Vlasis_draw_rect(&tot_tests);*/
   tot_errors += test_draw_background( &tot_tests, fontdata);

   printf("\n\n%d of %d tests passed\n\n", tot_tests - tot_errors, tot_tests);

   return tot_errors;
}

int test_event_handling(int *tot_tests, Window testWin){
   int result=0;
   /*Window testWin;*/
   fntrow fontdata[FNTCHARS][FNTHEIGHT];
   int line=0, button=0;

   read_font(fontdata, FNTFILENAME);
   testWin.win= SDL_CreateWindow("testing event handling",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          600, 400,
                          SDL_WINDOW_SHOWN);

   testWin.renderer = SDL_CreateRenderer(testWin.win, -1, 0);
   SDL_SetRenderDrawColor(testWin.renderer, 10, 10, 10, SDL_ALPHA_OPAQUE);
   SDL_RenderClear(testWin.renderer);
   SDL_RenderPresent(testWin.renderer);

   DrawString(&testWin, fontdata,"press 'y'", 0, FNTHEIGHT * ++line);
   SDL_RenderPresent(testWin.renderer);
   do{
      event_handling(&button);
   }while(button != 'y');
   DrawString(&testWin, fontdata,"passed", 300, FNTHEIGHT * line);

   DrawString(&testWin, fontdata,"press 'n' ", 0, FNTHEIGHT * ++line);
   SDL_RenderPresent(testWin.renderer);
   do{
      event_handling(&button);
   }while(button != 'n');
   DrawString(&testWin, fontdata,"passed", 300, FNTHEIGHT * line);

   DrawString(&testWin, fontdata,"press space bar", 0, FNTHEIGHT * ++line);
   SDL_RenderPresent(testWin.renderer);
   do{
      event_handling(&button);
   }while(button != SPACE);
   DrawString(&testWin, fontdata,"passed", 300, FNTHEIGHT * line);

   DrawString(&testWin, fontdata,"press escape", 0, FNTHEIGHT * ++line);
   SDL_RenderPresent(testWin.renderer);
   do{
      event_handling(&button);
   }while(button != ESCAPE);
   DrawString(&testWin, fontdata,"passed", 300, FNTHEIGHT * line);
   SDL_RenderPresent(testWin.renderer);

   SDL_Delay(500);
   SDL_DestroyWindow(testWin.win);
   SDL_Quit();
   printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "handle_event", GRNSTR, WHITESTR );
   return PASS;
}

void popup(){

   Window popUp;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];
   int line=0, button=0;

   read_font(fontdata, FNTFILENAME);
   popUp.win= SDL_CreateWindow("popup message",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          600, 400,
                          SDL_WINDOW_SHOWN);

   popUp.renderer = SDL_CreateRenderer(popUp.win, -1, 0);
   SDL_SetRenderDrawColor(popUp.renderer, 0, 100, 0, SDL_ALPHA_OPAQUE);
   SDL_RenderClear(popUp.renderer);
   SDL_RenderPresent(popUp.renderer);

   DrawString(&popUp, fontdata,"Expected result from next test:", 0, FNTHEIGHT * ++line);
   DrawString(&popUp, fontdata,"to draw a black window named ", 0, FNTHEIGHT * ++line);
   DrawString(&popUp, fontdata,"Vlasis' Teletext with size: 640x450", 0, FNTHEIGHT * ++line);
   DrawString(&popUp, fontdata,"     if ready press space", 0, 350);
   SDL_RenderPresent(popUp.renderer);
   SDL_UpdateWindowSurface(popUp.win);
   do{
      event_handling(&button);
   }while(button != SPACE);
   SDL_DestroyWindow(popUp.win);
   SDL_Quit();
}

int did_test_passed(){

   Window pass_popUp;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];
   int line=0, button=0;

   read_font(fontdata, FNTFILENAME);
   pass_popUp.win= SDL_CreateWindow("did it pass?",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          600, 200,
                          SDL_WINDOW_SHOWN);

   pass_popUp.renderer = SDL_CreateRenderer(pass_popUp.win, -1, 0);
   SDL_SetRenderDrawColor(pass_popUp.renderer, 200, 0, 0, SDL_ALPHA_OPAQUE);
   SDL_RenderClear(pass_popUp.renderer);
   SDL_RenderPresent(pass_popUp.renderer);

   DrawString(&pass_popUp, fontdata, "Was the test successful?", 0, FNTHEIGHT * ++line);
   DrawString(&pass_popUp, fontdata, "if yes press 'y', else 'n'", 0, FNTHEIGHT * ++line);
   SDL_RenderPresent(pass_popUp.renderer);
   SDL_UpdateWindowSurface(pass_popUp.win);
   do{
      event_handling(&button);
   }while(button != ESCAPE);
   SDL_DestroyWindow(pass_popUp.win);
   SDL_Quit();
   return button;
}

int test_init_window( int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]){

   Window testWin;
   int button=0, result=0;

   testWin.win = NULL;
   popup();
   init_window(&testWin);
   SDL_Delay(1000);
   /*
   do{
   event_handling(&button);
   }while(button != SPACE);
   */
   /*result = did_test_passed();*/
   if (result == 'n'){
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "init_window", REDSTR, WHITESTR );
      return FAIL;
   }
   else if(result == 'y'){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "init_window", GRNSTR, WHITESTR );
      return PASS;
   }
   SDL_DestroyWindow(testWin.win);
   SDL_Quit();
}

int test_select_colour( int *tot_tests){

   Window testWin;
   SDL_Rect testRect;

   testRect.x = 50; testRect.y = 50;
   testRect.w = 10; testRect.h = 10;

   testWin.win= SDL_CreateWindow("select colour",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         300, 300,
                         SDL_WINDOW_SHOWN);
  testWin.renderer = SDL_CreateRenderer(testWin.win, -1, 0);
  /*set a grey window of size 300x300 */
  SDL_SetRenderDrawColor(testWin.renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(testWin.renderer);
  SDL_RenderPresent(testWin.renderer);
  /* expect to draw a rect in the middle with the eight colours that
     I have set: black, red, green, blue, yellow, magenta, cyan, white.*/
   select_colour(&testWin, BLACK );
   SDL_RenderFillRect( testWin.renderer, &testRect);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   select_colour(&testWin, WHITE );
   SDL_RenderFillRect( testWin.renderer, &testRect);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   select_colour(&testWin, RED );
   SDL_RenderFillRect( testWin.renderer, &testRect);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   select_colour(&testWin, GREEN );
   SDL_RenderFillRect( testWin.renderer, &testRect);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   select_colour(&testWin, YELLOW );
   SDL_RenderFillRect( testWin.renderer, &testRect);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   select_colour(&testWin, BLUE );
   SDL_RenderFillRect( testWin.renderer, &testRect);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   select_colour(&testWin, MAGENTA );
   SDL_RenderFillRect( testWin.renderer, &testRect);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   select_colour(&testWin, CYAN );
   SDL_RenderFillRect( testWin.renderer, &testRect);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   /* Here I give some edgy prices that the function should ignore.
      I expect to draw the rectangle with the last set colour
      and see a small delay in the sdl window */
   select_colour(&testWin,-500);
   SDL_RenderFillRect( testWin.renderer, &testRect);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   select_colour(&testWin,0);
   SDL_RenderFillRect( testWin.renderer, &testRect);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   select_colour(&testWin, 255);
   SDL_RenderFillRect( testWin.renderer, &testRect);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);

   SDL_Quit();
   printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "select_colour", GRNSTR, WHITESTR );
   return PASS;
}

int test_light_pixel( int *tot_tests){

   Window testWin;

   testWin.win= SDL_CreateWindow("light pixel",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          300, 300,
                          SDL_WINDOW_SHOWN);
  testWin.renderer = SDL_CreateRenderer(testWin.win, -1, 0);
  /*set a grey window of size 300x300 */
  SDL_SetRenderDrawColor(testWin.renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(testWin.renderer);
  SDL_RenderPresent(testWin.renderer);
  /* expect to draw two green rect on the top left corner with
     with size (6x8) each */
  light_pixel(&testWin, 0, 0, GREEN, Contiguous);
  light_pixel(&testWin, 0, PIXEL_W, GREEN, Contiguous);
  SDL_RenderPresent(testWin.renderer);
  SDL_Delay(1500);
  /*expect to draw two red rect in the middle, but
    Separated with size (4x6) each */
  light_pixel(&testWin, 0, 50, RED, Separated);
  light_pixel(&testWin, 0, 50 + PIXEL_W, RED, Separated);
  SDL_RenderPresent(testWin.renderer);
  SDL_Delay(1500);
  SDL_Quit();

  printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "light_pixel", GRNSTR, WHITESTR );

  return PASS;
  /*if(){
    printf("%23s %sFAIL%s\n",__func__, REDSTR, WHITESTR );
    return FAIL;
  }
  else{
   }*/
}

/*in this function I also test the set_pixels_zero function*/
int test_Vlasis_draw_rect(int *tot_tests){

   Window testWin;
   Cell testCell;

   testCell.foreColour = BLUE;
   testWin.win= SDL_CreateWindow("Vlasis draw rect",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          300, 300,
                          SDL_WINDOW_SHOWN);
   testWin.renderer = SDL_CreateRenderer(testWin.win, -1, 0);
   /*set a grey window of size 300x300 */
   SDL_SetRenderDrawColor(testWin.renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
   SDL_RenderClear(testWin.renderer);
   SDL_RenderPresent(testWin.renderer);

   /*expect to draw a blue rect in the top left corner with size 18x16
     pixel by pixel*/
   testCell.pixel.l_top = ACTIVE;
   Vlasis_draw_rect(&testWin, testCell, 0, 0);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   testCell.pixel.r_top = ACTIVE;
   Vlasis_draw_rect(&testWin, testCell, 0, 0);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   testCell.pixel.l_mid = ACTIVE;
   Vlasis_draw_rect(&testWin, testCell, 0, 0);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);

   /* expect to draw nothing if set_pixels_zero works properly*/
   set_pixels_zero( testCell);
   printf("%d. %23s %sPASS%s\n", ++*tot_tests, "set_pixels_zero", GRNSTR, WHITESTR );
   Vlasis_draw_rect(&testWin, testCell, 0, 0);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(1000);

   testCell.pixel.r_mid = ACTIVE;
   Vlasis_draw_rect(&testWin, testCell, 0, 0);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   testCell.pixel.l_bot = ACTIVE;
   Vlasis_draw_rect(&testWin, testCell, 0, 0);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   testCell.pixel.r_bot = ACTIVE;
   Vlasis_draw_rect(&testWin, testCell, 0, 0);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   /* I know that this function is too long and I should probably
      break it into smaller, but then the whole testing process is
      useless cause I should also need to test the new functions */
   SDL_Quit();
   /* edw prepei na valw if statments */
   printf("%d. %23s %sPASS%s\n", ++*tot_tests, "Vlasis_draw_rect", GRNSTR, WHITESTR );
   return PASS;
}

int test_draw_background(int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]){

   int x=0, y=0, button=0;
   Window testWin;

   testWin.win= SDL_CreateWindow("draw_background",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          800, 300,
                          SDL_WINDOW_SHOWN);
   testWin.renderer = SDL_CreateRenderer(testWin.win, -1, 0);
   /*set a grey window of size 300x300 */
   SDL_SetRenderDrawColor(testWin.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
   SDL_RenderClear(testWin.renderer);
   SDL_RenderPresent(testWin.renderer);

   DrawString(&testWin, fontdata, "expect to see a small green rect ", 0, FNTHEIGHT * ++y);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(2000);

   draw_background(&testWin, GREEN, 400,  FNTHEIGHT * ++y);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(2000);


   DrawString(&testWin, fontdata, "Was the test successful?", 0, FNTHEIGHT * ++y);
   DrawString(&testWin, fontdata, "if yes press 'y', else 'n'", 0, FNTHEIGHT * ++y);
   SDL_RenderPresent(testWin.renderer);

   do{
      event_handling(&button);
   }while((button != 'y') && (button != 'n'));
   SDL_DestroyWindow(testWin.win);
   SDL_Quit();
   if (button == 'n'){
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "draw_background", REDSTR, WHITESTR );
      return FAIL;
   }
   else if(button == 'y'){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "draw_background", GRNSTR, WHITESTR );
      return PASS;
   }
}
