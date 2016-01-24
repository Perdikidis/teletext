#include "test.h"



int test(void)
{

   int tot_errors=0, tot_tests=0;
   Window testWin;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];

   read_font(fontdata, FNTFILENAME);
   printf("\n\nTESTING!!!!\n\n\n");
   tot_errors += test_new_line_settings(&tot_tests);

   /*
   tot_errors += test_exam_cases(&tot_tests);
   tot_errors += test_set_current_cell(&tot_tests);
   tot_errors += test_set_foreground(&tot_tests);
   tot_errors += test_set_background(&tot_tests);
   tot_errors += test_set_character(&tot_tests);
   tot_errors += test_encode_graphics(&tot_tests);
   tot_errors += test_set_pixels_zero(&tot_tests);
   tot_errors += test_event_handling( &tot_tests, testWin);
   tot_errors += test_init_window( &tot_tests, fontdata);
   tot_errors += test_select_colour( &tot_tests, fontdata);
   tot_errors += test_light_pixel(&tot_tests, fontdata);
   tot_errors += test_Vlasis_draw_rect(&tot_tests, fontdata);
   tot_errors += test_draw_cell( &tot_tests, fontdata);
   */

   printf("\n\n%d of %d tests passed\n\n", tot_tests - tot_errors, tot_tests);

   return tot_errors;
}

int test_new_line_settings(int *tot_tests){

   Cell c, prev;
   int fail=0;

   c = new_line_settings(c, &prev);
   if((c.Height != prev.Height) || (c.Height != Single)){
      fail++;
   }
   if((c.foreColour != prev.foreColour) || (c.foreColour != WHITE)){
      fail++;
   }
   if((c.backColour != prev.backColour) || (c.backColour != BLACK)){
      fail++;
   }
   if((c.Mode != prev.Mode) || (c.Mode != Alphanumeric)){
      fail++;
   }
   if((c.Type != prev.Type) || (c.Type != Contiguous)){
      fail++;
   }
   if((c.Held_graph != prev.Held_graph) || (c.Held_graph != No)){
      fail++;
   }

   if (fail == 0){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "new_line_settings", GRNSTR, WHITESTR );
      return PASS;
   }
   else{
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "new_line_settings", REDSTR, WHITESTR );
      return FAIL;
   }
}

int test_exam_cases(int *tot_tests){

   Cell c, prev;
   int fail=0;

   /*will test all 7 cases*/
   c.character = SINGLE;
   c = exam_cases(c, &prev);
   if((c.Height != prev.Height) || (c.Height != Single) ||
   (c.Held_graph != prev.Held_graph) || (c.Held_graph != No )){
      fail++;
   }
   c.character = DOUBLE;
   c = exam_cases(c, &prev);
   if((c.Height != prev.Height) || (c.Height != Double) ||
   (c.Held_graph != prev.Held_graph) || (c.Held_graph != No )){
      fail++;
   }
   c.character = CYAN;
   c = exam_cases(c, &prev);
   if((c.Mode != prev.Mode) || (c.Mode != Alphanumeric) ||
   (c.Held_graph != prev.Held_graph) || (c.Held_graph != No )){
      fail++;
   }
   c.character = WHITE_GRAPH;
   c = exam_cases(c, &prev);
   if((c.Mode != prev.Mode) || (c.Mode != Graphics) ||
   (c.Type != prev.Type) || (c.Type != Contiguous )){
      fail++;
   }
   c.character = SEPARATED;
   c = exam_cases(c, &prev);
   if((c.Mode != prev.Mode) || (c.Mode != Graphics) ||
   (c.Type != prev.Type) || (c.Type != Separated )){
      fail++;
   }
   c.character = HOLD_GRAPHICS;
   c = exam_cases(c, &prev);
   if((c.Held_graph != prev.Held_graph) || (c.Held_graph != Yes)){
      fail++;
   }
   c.character = RELEASE_GRAPHICS;
   c = exam_cases(c, &prev);
   if((c.Held_graph != prev.Held_graph) || (c.Held_graph != No)){
      fail++;
   }

   if (fail == 0){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "exam_cases", GRNSTR, WHITESTR );
      return PASS;
   }
   else{
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "exam_cases", REDSTR, WHITESTR );
      return FAIL;
   }
}

int test_set_current_cell(int *tot_tests){

   Cell c, prev;
   unsigned char last_graph;

   prev.foreColour = WHITE;
   prev.backColour = BLACK;
   prev.Height = Single;
   prev.Mode = Alphanumeric;
   prev.Type = Contiguous;
   prev.Held_graph = No;
   c = set_current_cell(c, &prev, &last_graph);

   if ((prev.Mode != c.Mode)  || (prev.Held_graph != c.Held_graph)
   || (prev.Type != c.Type) || (prev.Height != c.Height) ||
   (prev.foreColour != c.foreColour) || (prev.backColour != c.backColour)){
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "set_current_cell", REDSTR, WHITESTR );
      return FAIL;
   }
   else{
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "set_current_cell", GRNSTR, WHITESTR );
      return PASS;
   }

}

int test_set_foreground(int *tot_tests){

   Cell c;
   int prevFore=0, fail=0;

   c.character = GREEN;
   c = set_foreground(c, &prevFore);
   if(c.foreColour != GREEN){
      fail++;
   }
   c.character = RED_GRAPH;
   c = set_foreground(c, &prevFore);
   if(c.foreColour != RED){
      fail++;
   }
   c.character = SPACE;
   c = set_foreground(c, &prevFore);
   if(c.foreColour != RED){/*should retain prev colour*/
      fail++;
   }

   if (fail == 0){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "set_foreground", GRNSTR, WHITESTR );
      return PASS;
   }
   else{
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "set_foreground", REDSTR, WHITESTR );
      return FAIL;
   }
}

int test_set_background(int *tot_tests){

   Cell c;
   int prevBack=0, prevFore=0, fail=0;

   c.character = BLACKBACKGROUND;
   c = set_background(c, &prevBack, prevFore);
   if(c.backColour != BLACK){
      fail++;
   }
   c.character = NEWBACKGROUND;
   prevFore = GREEN;
   c = set_background(c, &prevBack, prevFore);
   if(c.backColour != GREEN){
      fail++;
   }
   c.character = SPACE;
   c = set_background(c, &prevBack, prevFore);
   if(c.backColour != GREEN){/*should retain prev colour*/
      fail++;
   }

   if (fail == 0){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "set_background", GRNSTR, WHITESTR );
      return PASS;
   }
   else{
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "set_background", REDSTR, WHITESTR );
      return FAIL;
   }
}

int test_set_character(int *tot_tests){

   Cell c;
   int fail=0;
   unsigned char last_graph=0;

   /*testing all cases*/
   c.character = 0x83;/*control code*/
   last_graph = c.character;
   c.Mode = Alphanumeric;
   c = set_character(c, &last_graph);
   if ((c.character != SPACE) || (last_graph != 0x83)){
      fail++;
   }
   c.character = 'a' + VISABLE_ASCII;/*letter*/
   last_graph = c.character;
   c = set_character(c, &last_graph);
   if ((c.character != 'a' + VISABLE_ASCII) || (last_graph != c.character)){
      fail++;
   }
   c.Mode = Graphics;
   c.Held_graph = No;
   c = set_character(c, &last_graph);
   if ((c.character != SPACE) || (last_graph != 'a' + VISABLE_ASCII)){
      fail++;
   }
   c.Held_graph = Yes;
   c = set_character(c, &last_graph);
   if ((c.character != SPACE) || (last_graph != 'a' + VISABLE_ASCII)){
      fail++;
   }

   if (fail == 0){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "set_character", GRNSTR, WHITESTR );
      return PASS;
   }
   else{
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "set_character", REDSTR, WHITESTR );
      return FAIL;
   }
}

int test_encode_graphics(int *tot_tests){

   Cell c;

   c.pixel.l_top = 0;   c.pixel.r_top = 0;
   c.pixel.l_mid = 0;   c.pixel.r_mid = 0;
   c.pixel.l_bot = 0;   c.pixel.r_bot = 0;
   /*expect the r_bot and l_mid sixels to be active*/
   c = encode_graphics(c, 160+64+4);
   if ((c.pixel.l_mid == 0) || (c.pixel.r_bot == 0)){
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "encode_graphics", REDSTR, WHITESTR );
      return FAIL;
   }
   else{
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "encode_graphics", GRNSTR, WHITESTR );
      return PASS;
   }

}

int test_set_pixels_zero(int *tot_tests){
   Cell c;

   c.pixel.l_top = 1;
   c.pixel.l_mid = 2;
   c.pixel.l_bot = 3;
   c.pixel.r_top = 4;
   c.pixel.r_mid = 5;
   c.pixel.r_bot = 6;
   c = set_pixels_zero(c);
   if ((c.pixel.l_top!=0) || (c.pixel.l_mid!=0) || (c.pixel.l_bot!=0)
   || (c.pixel.r_top!=0) || (c.pixel.r_mid!=0) || (c.pixel.r_bot!=0)){
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "set_pixels_zero", REDSTR, WHITESTR );
      return FAIL;
   }
   else{
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "set_pixels_zero", GRNSTR, WHITESTR );
      return PASS;
   }

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
                          600, 300,
                          SDL_WINDOW_SHOWN);

   popUp.renderer = SDL_CreateRenderer(popUp.win, -1, 0);
   SDL_SetRenderDrawColor(popUp.renderer, 0, 100, 0, SDL_ALPHA_OPAQUE);
   SDL_RenderClear(popUp.renderer);
   SDL_RenderPresent(popUp.renderer);

   DrawString(&popUp, fontdata,"Expected result from next test:", 0, FNTHEIGHT * ++line);
   DrawString(&popUp, fontdata,"to draw a black window named ", 0, FNTHEIGHT * ++line);
   DrawString(&popUp, fontdata,"Vlasis' Teletext with size: 640x450", 0, FNTHEIGHT * ++line);
   DrawString(&popUp, fontdata,"     if ready press space", 0, 250);
   SDL_RenderPresent(popUp.renderer);
   SDL_UpdateWindowSurface(popUp.win);
   do{
      event_handling(&button);
   }while(button != SPACE);
   SDL_DestroyWindow(popUp.win);
   SDL_Quit();
}

int test_init_window( int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]){

   Window testWin;
   int button=0, result=0;

   popup();
   init_window(&testWin);
   SDL_Delay(3000);
   DrawString(&testWin, fontdata, "did the window appear? y/n", 0, 400);
   SDL_RenderPresent(testWin.renderer);
   do{
      event_handling(&button);
   }while((button != 'y') && (button != 'n'));
   SDL_DestroyWindow(testWin.win);
   SDL_Quit();
   if (button == 'n'){
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "init_window", REDSTR, WHITESTR );
      return FAIL;
   }
   else if(button == 'y'){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "init_window", GRNSTR, WHITESTR );
      return PASS;
   }
}

int test_select_colour( int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]){

   Window testWin;
   SDL_Rect testRect;
   int button=0, colour= BLACK, i, line=0;

   testRect.x = 400; testRect.y = 100;
   testRect.w = 80; testRect.h = 80;

   testWin.win= SDL_CreateWindow("select colour",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         800, 300,
                         SDL_WINDOW_SHOWN);
  testWin.renderer = SDL_CreateRenderer(testWin.win, -1, 0);

  SDL_SetRenderDrawColor(testWin.renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(testWin.renderer);
  DrawString(&testWin, fontdata, "in this test a rect should appear in all 8 colours", 0, ++line * FNTHEIGHT );
  DrawString(&testWin, fontdata, "press space bar to begin", 0, ++ line * FNTHEIGHT );
  SDL_RenderPresent(testWin.renderer);
  do{
     event_handling(&button);
  }while(button != SPACE);
  for (i=0; i<8; i++){
     select_colour(&testWin, colour++ );
     SDL_RenderFillRect( testWin.renderer, &testRect);
     SDL_RenderPresent(testWin.renderer);
     SDL_Delay(1000);
  }
   DrawString(&testWin, fontdata, "did all 8 colours appear? y/n", 0, 250 );
   SDL_RenderPresent(testWin.renderer);
   do{
      event_handling(&button);
   }while((button != 'y') && (button != 'n'));
   SDL_DestroyWindow(testWin.win);
   SDL_Quit();
   if (button == 'n'){
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "select_colour", REDSTR, WHITESTR );
      return FAIL;
   }
   else if(button == 'y'){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "select_colour", GRNSTR, WHITESTR );
      return PASS;
   }
}

int test_light_pixel( int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]){

   Window testWin;
   int line=0, button=0;

   testWin.win= SDL_CreateWindow("light pixel",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          800, 300,
                          SDL_WINDOW_SHOWN);
  testWin.renderer = SDL_CreateRenderer(testWin.win, -1, 0);
  /*set a grey window of size 300x300 */
  SDL_SetRenderDrawColor(testWin.renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(testWin.renderer);
  SDL_RenderPresent(testWin.renderer);

  DrawString(&testWin, fontdata, "expect to draw two rect next to each other ", 0, ++line * FNTHEIGHT );
  DrawString(&testWin, fontdata, "first Contiguous and then Separated", 0, ++line * FNTHEIGHT );
  DrawString(&testWin, fontdata, "press space bar to begin", 0, ++line * FNTHEIGHT );
  SDL_RenderPresent(testWin.renderer);
  do{
     event_handling(&button);
  }while(button != SPACE);
  light_pixel(&testWin, ++line * FNTHEIGHT, 300, GREEN, Contiguous);
  light_pixel(&testWin, line * FNTHEIGHT, 300 + PIXEL_W, GREEN, Contiguous);
  SDL_RenderPresent(testWin.renderer);
  SDL_Delay(1500);

  light_pixel(&testWin, line * FNTHEIGHT, 350, RED, Separated);
  light_pixel(&testWin, line * FNTHEIGHT, 350 + PIXEL_W, RED, Separated);
  SDL_RenderPresent(testWin.renderer);
  SDL_Delay(1500);
  DrawString(&testWin, fontdata, "was the test successful? y/n", 0, ++line * FNTHEIGHT );
  SDL_RenderPresent(testWin.renderer);
  do{
     event_handling(&button);
  }while((button != 'y') && (button != 'n'));
  SDL_DestroyWindow(testWin.win);
  SDL_Quit();
  if (button == 'n'){
     printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "light_pixel", REDSTR, WHITESTR );
     return FAIL;
  }
  else if(button == 'y'){
     printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "light_pixel", GRNSTR, WHITESTR );
     return PASS;
  }
}

int test_Vlasis_draw_rect(int *tot_tests,fntrow fontdata[FNTCHARS][FNTHEIGHT]){

   Window testWin;
   Cell testCell;
   int button=0, line=0;

   testCell.foreColour = BLUE;
   testWin.win= SDL_CreateWindow("Vlasis draw rect",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          800, 300,
                          SDL_WINDOW_SHOWN);
   testWin.renderer = SDL_CreateRenderer(testWin.win, -1, 0);
   SDL_SetRenderDrawColor(testWin.renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
   SDL_RenderClear(testWin.renderer);
   DrawString(&testWin, fontdata, "expect to draw all sixels one by one", 0, ++line * FNTHEIGHT );
   DrawString(&testWin, fontdata, "press space bar to begin", 0, ++line * FNTHEIGHT );
   SDL_RenderPresent(testWin.renderer);
   do{
      event_handling(&button);
   }while(button != SPACE);

   testCell.pixel.l_top = ACTIVE;
   Vlasis_draw_rect(&testWin, testCell, ++line * FNTHEIGHT, 400);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   testCell.pixel.r_top = ACTIVE;
   Vlasis_draw_rect(&testWin, testCell, line * FNTHEIGHT, 400);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   testCell.pixel.l_mid = ACTIVE;
   Vlasis_draw_rect(&testWin, testCell, line * FNTHEIGHT, 400);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   testCell.pixel.r_mid = ACTIVE;
   Vlasis_draw_rect(&testWin, testCell, line * FNTHEIGHT, 400);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   testCell.pixel.l_bot = ACTIVE;
   Vlasis_draw_rect(&testWin, testCell, line * FNTHEIGHT, 400);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   testCell.pixel.r_bot = ACTIVE;
   Vlasis_draw_rect(&testWin, testCell, line * FNTHEIGHT, 400);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(500);
   DrawString(&testWin, fontdata, "was the test successful? y/n", 0, ++line * FNTHEIGHT );
   SDL_RenderPresent(testWin.renderer);
   do{
      event_handling(&button);
   }while((button != 'y') && (button != 'n'));
   SDL_DestroyWindow(testWin.win);
   SDL_Quit();
   if (button == 'n'){
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "Vlasis_draw_rect", REDSTR, WHITESTR );
      return FAIL;
   }
   else if(button == 'y'){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "Vlasis_draw_rect", GRNSTR, WHITESTR );
      return PASS;
   }
}

/*in this function I also test draw_background and draw_foreground functions*/
int test_draw_cell(int *tot_tests, fntrow fontdata[FNTCHARS][FNTHEIGHT]){

   int x=0, y=0, button=0;
   Window testWin;
   Cell testCell;

   testCell.backColour = GREEN;
   testCell.foreColour = BLUE;
   testCell.character = 'A' + VISABLE_ASCII;

   testWin.win= SDL_CreateWindow("draw_cell",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          800, 600,
                          SDL_WINDOW_SHOWN);
   testWin.renderer = SDL_CreateRenderer(testWin.win, -1, 0);
   SDL_SetRenderDrawColor(testWin.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
   SDL_RenderClear(testWin.renderer);
   SDL_RenderPresent(testWin.renderer);

   DrawString(&testWin, fontdata, "attempt to draw a Cell", 0, FNTHEIGHT * ++y);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(2000);

   draw_cell(&testWin, fontdata, testCell, 400, FNTHEIGHT * ++y , Single);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(2000);

   DrawString(&testWin, fontdata, "is the background green? y/n", 0, FNTHEIGHT * ++y);
   SDL_RenderPresent(testWin.renderer);
   do{
      event_handling(&button);
   }while((button != 'y') && (button != 'n'));
   if (button == 'n'){
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "draw_background", REDSTR, WHITESTR );
   }
   else if(button == 'y'){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "draw_background", GRNSTR, WHITESTR );
   }
   button = 0;

   DrawString(&testWin, fontdata, "Is the foreground blue? y/n", 0, FNTHEIGHT * ++y);
   SDL_RenderPresent(testWin.renderer);
   do{
      event_handling(&button);
   }while((button != 'y') && (button != 'n'));
   if (button == 'n'){
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "draw_foreground", REDSTR, WHITESTR );
   }
   else if(button == 'y'){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "draw_foreground", GRNSTR, WHITESTR );
   }
   button = 0;
   testCell.Height = Double;
   draw_cell(&testWin, fontdata, testCell, 400 + FNTWIDTH, FNTHEIGHT * ++y , Single);
   draw_cell(&testWin, fontdata, testCell, 400+2*FNTWIDTH, FNTHEIGHT * y , Double);
   DrawString(&testWin, fontdata, "are both halfs of double height displayed? y/n", 0, FNTHEIGHT * ++y);
   SDL_RenderPresent(testWin.renderer);
   SDL_Delay(2000);
   do{
      event_handling(&button);
   }while((button != 'y') && (button != 'n'));

   SDL_DestroyWindow(testWin.win);
   SDL_Quit();
   if (button == 'n'){
      printf("%d. %23s %sFAIL%s\n", ++*tot_tests, "draw_Cell", REDSTR, WHITESTR );
      return FAIL;
   }
   else if(button == 'y'){
      printf("%d. %23s %sPASS%s\n",  ++*tot_tests, "draw_Cell", GRNSTR, WHITESTR );
      return PASS;
   }
}
