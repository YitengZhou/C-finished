/*This WIREWORLD program could create a new file to display the board
  for 1000 generations using plain text.*/
/*本程序使用break*/
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
  for (i=0;i<100;i++){
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
  do {
    do {
      ch = (arr[i][j]=getc(fp));
      j++;
    } while (ch != EOF && ch !='\n');
    i++;
    j=0;
  } while (ch!=EOF);
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
          fprintf(stderr,"The input file include invalid character! "\
          "Check the file again.\n");
          exit (1);
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

void test(void)
{
  char board[CELL][CELL]={{'c','H','c','c','c'},
                          {'H','H','H','c','c'},
                          {'H','c','H','c','c'},
                          {'H','H','H','c','c'},
                          {'c','c','c','c','\0'}};
  assert(Electron_heads(board,0,0)==3);
  assert(Electron_heads(board,0,4)==0);
  assert(Electron_heads(board,2,1)==8);
  assert(Electron_heads(board,4,4)==0);
  assert(Access(board,0,0)=='c');
  assert(Access(board,1,1)=='H');
  assert(Access(board,4,4)=='\0');
  assert(Access(board,-1,-1)=='\0');
  assert(Access(board,4,-1)=='\0');
}
