#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_NUMBER 4

void permute(char a[],char b[],char c[],int l,int r);
void swap (char *a,char *b);
int in_file(char *s);
int same(char a[],char b[],char c[]);

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
  permute(first,last,sequence,0,WORD_NUMBER-1-same(first,last,sequence));
  return 0;
}

void permute(char a[],char b[],char c[],int l,int r)
{
  int i,j,flag=0,same=0;
  static int cnt=1;
  if (l==r) {
    for (i=0;i<WORD_NUMBER;i++) {
      if (a[c[i]-'1']!=b[c[i]-'1']){
        swap(a+(c[i]-'1'),b+(c[i]-'1'));
        flag += in_file(a);
        }
      else {
        same +=1;
      }
    }
    for (i=0;i<WORD_NUMBER;i++) {
      swap(a+i,b+i);
    }
    /*判断单词是否全在文件ladder=5情况，如有一个字母相同需same=1,其他在文件中*/
    if ((same==1 && flag==3)||flag ==4)  {
      printf("This is %d way array is [%c%c%c%c]\n",cnt++,c[0],c[1],c[2],c[3]);
      printf("%s\n",a);
      for (i=0;i<WORD_NUMBER;i++) {
        if (a[c[i]-'1']!=b[c[i]-'1']){
          swap(a+(c[i]-'1'),b+(c[i]-'1'));
          printf("%s\n",a);
        }
      }
      for (i=0;i<WORD_NUMBER;i++) {
        swap(a+i,b+i);
      }
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

int in_file(char *s)
{
  FILE *fp;
  char list[WORD_NUMBER+1];
  if ((fp=fopen("34words.txt","r"))==NULL){
    fprintf(stderr,"Can't find file\n");
    exit(EXIT_FAILURE);
  }
  while (fscanf(fp,"%s",list)==1){
    if (strcmp(s,list)==0) {
      fclose(fp);
      return 1;
    }
  }
  fclose(fp);
  return 0;
}

/*判断是否单词中有否相同的字母，有的话将其移到最后一位并返回1减少循环次数*/
int same(char a[],char b[],char c[])
{
  int i;
  for (i=0;i<WORD_NUMBER;i++) {
    if (a[i]==b[i])
    {
      swap(c+3,c+i);
      return 1;
    }
  }
  return 0;
}


void swap (char *a,char *b)
{
  char temp;
  temp=*a;
  *a=*b;
  *b=temp;
}
