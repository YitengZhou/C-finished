/*This WIREWORLD program could become cartoon
  to show the next generation .*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "neillncurses.h"
/* 2D-array 40 * 40 cells value */
#define CELL 40

void Display_output(char *file_name);
void Read_from_file(FILE *fp, char arr[CELL][CELL]);
void Next_generation(char arr[CELL][CELL]);
char Conduction(char before[CELL][CELL],int i,int j);
int Electron_heads(char arr[CELL][CELL],int m,int n);
char Access(char arr[CELL][CELL],int m,int n);
void test();

int main(int argc,char **argv)
{
  if (argc==2) {
    Display_output(argv[1]);
  }
  else {
    fprintf(stderr,"ERROR: Incorrect usage. try e.g. "\
    "%s wirewcircuit1.txt\n",argv[0]);
  }
  return 0;
}

/* Open the file and read the character
  finally on the screen */
void Display_output(char *file_name)
{
  FILE *ifp;
  char show[CELL][CELL];
  NCURS_Simplewin sw;
  ifp=fopen(file_name,"r");
  if (ifp==NULL){
    fprintf(stderr,"Failed to open the file.\n");
    exit(0);
  }
  Read_from_file(ifp,show);
  fclose(ifp);
  test();
  Neill_NCURS_Init(&sw);
  /* For the character 'H'-heads is blue, 't'-tails is red,
    'c'-copper is yellow and background is black */
  Neill_NCURS_CharStyle(&sw, "H", COLOR_BLUE, COLOR_BLACK,A_BOLD);
  Neill_NCURS_CharStyle(&sw, "t", COLOR_RED, COLOR_BLACK, A_BOLD);
  Neill_NCURS_CharStyle(&sw, "c", COLOR_YELLOW, COLOR_BLACK, A_BOLD);

  do{

     Neill_NCURS_PrintArray(&show[0][0], CELL, CELL, &sw);
     Next_generation(show);
     /*  Delay in 1/200s of a second */
     Neill_NCURS_Delay(200.0);
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
  while ((ch=getc(fp))!=EOF){
    if (ch=='\n') {
      i++;
      j=0;
    }
    else {
      arr[i][j]=ch;
      j++;
    }
  }
}

/* Get next 2D generation */
void Next_generation(char arr[CELL][CELL])
{
  int i,j;
  char next[CELL][CELL];
  for (i=0;i<CELL;i++){
    for (j=0;j<CELL;j++) {
      next[i][j]=Conduction(arr,i,j);
    }
  }
  for (i=0;i<CELL;i++){
    for (j=0;j<CELL;j++) {
      arr[i][j]=next[i][j];
    }
  }
}

/* Following the 8-surrounding rules */
char Conduction(char before[CELL][CELL],int i,int j)
{
  int head_number;
  switch (before[i][j])
  {
    case ' ': return ' ';
    case 'c':
      head_number=Electron_heads(before,i,j);
      if (head_number==1||head_number==2){
        return 'H';
      }
      else {
        return 'c';
      }
    case 'H': return 't';
    case 't': return 'c';
    case '\0': return '\0'; /*This case just for test function */
    default:
      fprintf(stderr,"The input file include invalid character! "\
      "Check the file again.\n");
      exit(0);
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

void test(void)
{
  char board[CELL][CELL]={{'c','H','c','c','c'},
                          {'H','H','H','c','c'},
                          {'H','c','H','t',' '},
                          {'H','H','H','c','c'},
                          {'c','c','c','c',' '}};

/*text function _Electron_heads with correct number of heads return */
  assert(Electron_heads(board,0,0)==3);
  assert(Electron_heads(board,0,4)==0);
  assert(Electron_heads(board,2,1)==8);
  assert(Electron_heads(board,4,4)==0);
  assert(Electron_heads(board,0,3)==1);

/*text function _Conduction with correct char return */
  assert(Conduction(board,0,0)=='c');
  assert(Conduction(board,0,1)=='t');
  assert(Conduction(board,0,3)=='H');
  assert(Conduction(board,2,1)=='c');
  assert(Conduction(board,2,3)=='c');
  assert(Conduction(board,2,4)==' ');

/*text function _Access with correct char return */
  assert(Access(board,0,0)=='c');
  assert(Access(board,1,1)=='H');
  assert(Access(board,4,4)==' ');
  assert(Access(board,-1,-1)=='\0');
  assert(Access(board,4,-1)=='\0');
  assert(Access(board,20,20)=='\0');
  assert(Access(board,50,50)=='\0');

/*text function _Next_generation
    NEXT board[CELL][CELL]={{'c','t','c','H','c'},
                            {'t','t','t','H','c'},
                            {'t','c','t','c',' '},
                            {'t','t','t','H','c'},
                            {'H','c','H','H',' '}};*/
  Next_generation(board);
  assert(board[0][0]=='c');
  assert(board[1][1]=='t');
  assert(board[0][3]=='H');
  assert(board[1][3]=='H');
  assert(board[4][0]=='H');
  assert(board[4][1]=='c');
  assert(board[4][4]==' ');
  assert(board[5][5]=='\0');
}
