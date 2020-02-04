#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_NUMBER 4

void permute(char a[],char b[],char c[],int l,int r);
void swap (char *a,char *b);

int main(int argc, char **argv)
{
  char first[WORD_NUMBER+1],last[WORD_NUMBER+1],sequence[WORD_NUMBER]="1234";
  if (argc!=3) {
    fprintf(stderr,"Can't find dictionary, try %s cold warm\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  if ((int)strlen(argv[1])!=WORD_NUMBER||(int)strlen(argv[2])!=WORD_NUMBER){
    fprintf(stderr,"Please enter two 4 letter words\n");
    exit(EXIT_FAILURE);
  }
  strcpy(first,argv[1]);
  strcpy(last,argv[2]);
  printf("%s->%s\n",first,last);
  permute(first,last,sequence,0,WORD_NUMBER-1);
  return 0;
}

void permute(char a[],char b[],char c[],int l,int r)
{
  int i,j;
  static int cnt=1;
  if (l==r) {
    printf("This is %d way array is [%c%c%c%c]\n",cnt++,c[0],c[1],c[2],c[3]);
    printf("%s\n",a);
    for (i=0;i<WORD_NUMBER;i++) {
      swap(a+(c[i]-'1'),b+(c[i]-'1'));
      printf("%s\n",a);
    }
    for (i=0;i<WORD_NUMBER;i++) {
      swap(a+i,b+i);
    }
  }
  else {
    for (j=l;j<=r;j++) {
    swap(c+j,c+l);
    permute(a,b,c,l+1,r);
    swap(c+j,c+l);
    }
  }
}

void swap (char *a,char *b)
{
  char temp;
  temp=*a;
  *a=*b;
  *b=temp;
}
