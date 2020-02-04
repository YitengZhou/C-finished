/*This WIREWORLD program could create a new file to display the board
  for 1000 generations using plain text.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* 2D-array 40 * 40 cells value */
#define CELL 40

/* The number of generations in wireworld is 1000 */
#define GENERATIONS 1000

void test(void);
void Display_output(char *file_name);
void Name_output_file(char *file_name, char outfile[]);
void Read_from_file(FILE *fp, char arr[CELL][CELL]);
void Write_to_file(FILE *fp, char arr[CELL][CELL]);
void Next_generation(char arr[CELL][CELL]);
char Conduction(char before[CELL][CELL],int i,int j);
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
  FILE *ifp,*ofp;
  int i=0;
  char outfile[CELL];
  char list[CELL][CELL];

  Name_output_file(file_name,outfile);
  ifp=fopen(file_name,"r");
  ofp=fopen(outfile,"w");
  if (ifp==NULL ||ofp==NULL){
    printf("Failed to open two filenames.");
    exit(0);
  }
  Read_from_file(ifp,list);

  for (i=0;i<=GENERATIONS;i++) {
    Write_to_file(ofp,list);
    Next_generation(list);
  }
  test();
  fclose(ifp);
  fclose(ofp);
}

/* Add string "output" before .txt after the program name */
void Name_output_file(char *file_name, char outfile[])
{
  int i;
  strcpy(outfile,file_name);
  for (i=0;i<CELL;i++){
    if (outfile[i]=='.') {
      outfile[i]='\0';
    }
  }
  strcat(outfile,"output.txt");
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

/* Output on the screen and into file */
void Write_to_file(FILE *fp, char arr[CELL][CELL])
{
  int i,j;
  static int cnt=0;
  fprintf(fp,"The %d generation:\n",cnt);
  printf("The %d generation:\n",cnt);
  for (i=0;i<CELL;i++) {
    for (j=0;j<CELL;j++) {
      fprintf(fp,"%c",arr[i][j]);
      printf("%c",arr[i][j]);
    }
    fprintf(fp,"\n");
    printf("\n");
  }
  cnt++;
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
  head_number=Electron_heads(before,i,j);
  switch (before[i][j])
  {
    case ' ': return ' ';
    case 'c':
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
      exit (0);
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
