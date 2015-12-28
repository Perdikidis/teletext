
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WIDTH 25
#define HEIGHT 40


void read_file (char *filename, char board[WIDTH][HEIGHT]);
void print_board( char board[WIDTH][HEIGHT]);


int main(void)
{

   char board[WIDTH][HEIGHT];

   read_file("test.m7.txt", board);
   printf("\n\n\nTwra exoume to board\n\n\n\n" );
   print_board(board);
   printf("\n\n\n\n\n" );
   return 0;
}


void read_file(char *fileName,  char board[WIDTH][HEIGHT]){

   char c;
   int i=0;
   FILE *fp;

   fp = fopen(fileName, "r");

   if (fp == NULL){
      printf("can't open file\n");/* na kanw fprintf (sterror) h kapws etsi*/
      exit(1);
   }
   while(c != EOF){
      c = fgetc(fp);
      printf("%d:%c ", c, c );
      if(( c < 0 ) && (c != EOF)){
         c += 128;
      }
      board[0][i++] = c;
   }
   fclose(fp);
}

void print_board( char board[WIDTH][HEIGHT]){

   int i, j;

   for (i=0; i<WIDTH; i++){
      printf("\n");
      for (j=0; j<HEIGHT; j++){
         printf("%c",/*(int) board[i][j],*/ board[i][j] );
      }
   }
}
