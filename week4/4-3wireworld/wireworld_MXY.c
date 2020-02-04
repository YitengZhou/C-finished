/*This WIREWORLD program could create a new file to display the board
  for 1000 generations using plain text.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neillncurses.h"
/* 2D-array 40 * 41 (cells value + '\n') */
#define CELL 40

void Display_output(char *file_name);
void Read_from_file(FILE *fp, char arr[CELL][CELL]);
void Next_generation(char arr[CELL][CELL]);
void Conduction(char before[CELL][CELL],char after[CELL][CELL]);
int Electron_heads(char arr[CELL][CELL],int m,int n);
char Access(char arr[CELL][CELL],int m,int n);

int main(int argc,char **argv)
{
  if (argc==2) {
    Display_output(argv[1]);
  }
  else {
    printf("ERROR: Incorrect usage. try e.g. "\
    "%s wirewcircuit1.txt\n",argv[0]);
  }
  return 0;
}
/* Open the file and read the character
  finally output into file and on the screen */
void Display_output(char *file_name)
{
  FILE *ifp;
  int i=0;
  char list[CELL][CELL];
  char show[CELL][CELL];
  NCURS_Simplewin sw;
  ifp=fopen(file_name,"r");
  if (ifp==NULL){
    printf("Failed to open two filenames.");
    exit(0);
  }
  Read_from_file(ifp,list);
  fclose(ifp);

  Neill_NCURS_Init(&sw);
  /* For the character 'H', 'E' etc. use red foreground colour, black background,
     and the alternative character set */
  Neill_NCURS_CharStyle(&sw, "H", COLOR_BLUE, COLOR_BLACK,A_BOLD);
  /* For the characters 'j', 'k', 'l' etc. use green foreground colour, black background,
     in bold with the alternative character set */
  Neill_NCURS_CharStyle(&sw, "t", COLOR_RED, COLOR_BLACK, A_BOLD);
  Neill_NCURS_CharStyle(&sw, "c", COLOR_YELLOW, COLOR_BLACK, A_BOLD);

  do{
    for (i=0;i<CELL;i++) {
      memcpy(show[i],list[i], CELL);
    }
     Neill_NCURS_PrintArray(&show[0][0], CELL, CELL, &sw);
     Next_generation(list);
     /*  Delay in 1/1000s of a second */
     Neill_NCURS_Delay(1000.0);
     /* Test for mouse click, or ESC key */
     Neill_NCURS_Events(&sw);
  }while(!sw.finished);

  /* Call this function if we exit() anywhere in the code */
  atexit(Neill_NCURS_Done);

  exit(EXIT_SUCCESS);
}

/* Read the codes from file */
void Read_from_file(FILE *fp, char arr[CELL][CELL])
{
  int i=0,j=0;
  char ch;
  do {
    do {
      ch = (arr[i][j]=getc(fp));
      j++;
    } while (ch != EOF && ch !='\n');
    i++;
    j=0;
  } while (ch!=EOF);
}

/* Get next 2D generation */
void Next_generation(char arr[CELL][CELL])
{
  int i,j;
  char next[CELL][CELL];

  Conduction(arr,next);
  for (i=0;i<CELL;i++) {
    for (j=0;j<CELL;j++) {
      arr[i][j]=next[i][j];
    }
  }
}

/* Following the 8-surrounding rules */
void Conduction(char before[CELL][CELL],char after[CELL][CELL])
{
  int i,j,head_number;
  for (i=0;i<CELL;i++){
    for (j=0;j<CELL;j++) {
      head_number=Electron_heads(before,i,j);
      switch (before[i][j])
      {
        case ' ': after[i][j]=' '; break;
        case 'c':
          if (head_number==1||head_number==2){
            after[i][j]='H';
          }
          else {
            after[i][j]='c';
          }
          break;
        case 'H': after[i][j]='t'; break;
        case 't': after[i][j]='c'; break;
        default:
          after[i][j]=' ';
      }
    }
  }
}

/* Count the number of electron heads */
int Electron_heads(char arr[CELL][CELL],int m,int n)
{
  int i,j,number=0;
  for (i=-1;i<=1;i++) {
    for (j=-1;j<=1;j++) {
      if (Access(arr,i+m,j+n)=='H') {
        number++;
      }
    }
  }
  return number;
}

/* Avoid crossing the boundary */
char Access(char arr[CELL][CELL],int m,int n)
{
  if (m<0) return '\0';
  if (m>=CELL) return '\0';
  if (n<0) return '\0';
  if (n>=CELL) return '\0';
  return arr[m][n];
}
