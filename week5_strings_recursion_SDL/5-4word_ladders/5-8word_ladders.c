/*本题吃亏在in_file定义的数组为WORD_NUMBER,造成很多4word单词读入失败，从而造成一系列问题*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_NUMBER 3
#define ALPHA 26
#define MAX 200

struct list{
  char word[WORD_NUMBER+1];
  int ladder;
};
typedef struct list List;

int get_list1(char a[], List *l);
int get_list2(char a[], List *l,List *x);
int get_list3(char a[], List *l);
void swap (char *a,char *b);
int in_file(char *s);
int Not_in_list1(char a[],List *l);

int main(int argc, char **argv)
{
  char first[WORD_NUMBER+1],last[WORD_NUMBER+1];
  List a[MAX];
  List b[MAX];
  List c[MAX];
  int l1,l2,i,cnt=1;
  int num1,num2,num_1;
  if (argc!=3) {
    fprintf(stderr,"Can't find dictionary, try %s cat dog\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  l1=strlen(argv[1]);
  l2=strlen(argv[2]);
  if (l1!=WORD_NUMBER||l2!=WORD_NUMBER){
    fprintf(stderr,"Please enter two 3 letter words\n");
    exit(EXIT_FAILURE);
  }
  strcpy(first,argv[1]);
  strcpy(last,argv[2]);
  num1=get_list1(first,a);
  for (i=0;i<num1;i++) {
    printf("%d:%s ladder=%d\n",cnt++,a[i].word,a[i].ladder);
  }
  num2=get_list2(first,b,a);
  cnt=1;
  for (i=0;i<num2;i++) {
    printf("%d:%s ladder=%d\n",cnt++,b[i].word,b[i].ladder);
  }
  cnt=1;
  num_1=get_list3(last,c);
  for (i=0;i<num_1;i++) {
    printf("%d:%s ladder=%d\n",cnt++,c[i].word,c[i].ladder);
  }
  return 0;
}

int get_list1(char a[], List *l) {
  int length;
  int i,j,cnt=0;
  char alpha[ALPHA];
  length=strlen(a);
  for (i=0;i<ALPHA;i++) {
    alpha[i]='a'+i;
  }
  for (j=0;j<length;j++){
    for (i=0;i<ALPHA;i++) {
      swap(a+j,alpha+i);
      if (a[j]!=alpha[i])
      {
        if (in_file(a)) {
          strcpy(l[cnt].word,a);
          l[cnt].ladder=1;
          cnt++;
        }
      }
      swap(a+j,alpha+i);
    }
  }
  return cnt;
}

int get_list2(char a[], List *l, List *x) {
  int length;
  int i,k,cnt=0;
  char alpha1[ALPHA];
  char alpha2[ALPHA];
  length=strlen(a);
  for (i=0;i<ALPHA;i++) {
    alpha1[i]='a'+i;
    alpha2[i]='a'+i;
  }
  for (i=0;i<ALPHA;i++) {
    for (k=0;k<ALPHA;k++) {
      swap(a+0,alpha1+i);
      swap(a+1,alpha2+k);
      if (a[0]!=alpha1[i] || a[1]!=alpha2[k])
      {
        if (in_file(a)&& Not_in_list1(a,x)) {
          strcpy(l[cnt].word,a);
          l[cnt].ladder=2;
          cnt++;
        }
      }
      swap(a+0,alpha1+i);
      swap(a+1,alpha2+k);
    }
  }

  for (i=0;i<ALPHA;i++) {
    for (k=0;k<ALPHA;k++) {
      swap(a+1,alpha1+i);
      swap(a+2,alpha2+k);
      if (a[1]!=alpha1[i]||a[2]!=alpha2[k])
      {
        if (in_file(a)&& Not_in_list1(a,x)) {
          strcpy(l[cnt].word,a);
          l[cnt].ladder=2;
          cnt++;
        }
      }
      swap(a+1,alpha1+i);
      swap(a+2,alpha2+k);
    }
  }

  for (i=0;i<ALPHA;i++) {
    for (k=0;k<ALPHA;k++) {
      swap(a+0,alpha1+i);
      swap(a+2,alpha2+k);
      if (a[0]!=alpha1[i]||a[2]!=alpha2[k])
      {
        if (in_file(a)&& Not_in_list1(a,x)) {
          strcpy(l[cnt].word,a);
          l[cnt].ladder=2;
          cnt++;
        }
      }
      swap(a+0,alpha1+i);
      swap(a+2,alpha2+k);
    }
  }

  return cnt;
}

int get_list3(char a[], List *l) {
  int length;
  int i,j,cnt=0;
  char alpha[ALPHA];
  length=strlen(a);
  for (i=0;i<ALPHA;i++) {
    alpha[i]='a'+i;
  }
  for (j=0;j<length;j++){
    for (i=0;i<ALPHA;i++) {
      swap(a+j,alpha+i);
      if (a[j]!=alpha[i])
      {
        if (in_file(a)) {
          strcpy(l[cnt].word,a);
          l[cnt].ladder=-1;
          cnt++;
        }
      }
      swap(a+j,alpha+i);
    }
  }
  return cnt;
}

int Not_in_list1(char a[],List *l)
{
  int i;
  for (i=0;i<MAX;i++)
  {
    if (strcmp(a,l[i].word)==0)
    {
      return 0;
    }
  }
  return 1;
}

int in_file(char *s)
{
  FILE *fp;
  char list[5];
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

void swap (char *a,char *b)
{
  char temp;
  temp=*a;
  *a=*b;
  *b=temp;
}
