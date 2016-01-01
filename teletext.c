
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HEIGHT 25
#define WIDTH 40


void read_file (char *filename, unsigned char board[HEIGHT][WIDTH]);
void print_board(unsigned char board[HEIGHT][WIDTH]);


int main(void)
{

   unsigned char board[HEIGHT][WIDTH];
   int a=20;

   read_file("lfc.m7.txt", board);
   printf("\n\n\nTwra exoume to board\n\n\n\n" );
   print_board(board);
   printf("\n\n\n\n\n" );
   printf("a=%d\n", a);
   a = a << 2;
   printf("a=%d\n", a);
   return 0;
}


void read_file(char *fileName, unsigned char board[HEIGHT][WIDTH]){

   char c;
   int i, j;
   FILE *fp;

   fp = fopen(fileName, "rb");

   if (fp == NULL){
      printf("can't open file\n");
      exit(1);
   }
   for (i=0; i<HEIGHT; i++){
      for (j=0; j<WIDTH; j++){
         fread(&board[i][j], sizeof(unsigned char), 1, fp);
         if( board[i][j] < 128 ){
            board[i][j] += 128;
         }
         /*printf("%d:%c ", board[i][j], board[i][j]);*/
      }
   }
   /*while (c != EOF){
      c = fgetc(fp);
      printf("%d:%c ", c, c);
      if(( c < 0 ) && (c != EOF)){
         c += 128;
      }
      board[0][i++] = c;
   }*/
   fclose(fp);
}

void print_board(unsigned char board[HEIGHT][WIDTH]){

   int i, j;

   for (i=0; i<HEIGHT; i++){
      printf("\n");
      for (j=0; j<WIDTH; j++){
         printf("%c",/*(int) board[i][j],*/ board[i][j] - 128 );
      }
   }
}
