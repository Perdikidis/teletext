#include "teletext.h"

#define FAIL 1
#define PASS 0
#define CLOSED 0
#define WHITESTR  "\x1B[0m"
#define REDSTR  "\x1B[31m"
#define GRNSTR  "\x1B[32m"


int test_init_window( int *tot_tests);
int test_select_colour( int *tot_tests);
int test_light_pixel(int *tot_tests);
int test_Vlasis_draw_rect(int *tot_tests);
