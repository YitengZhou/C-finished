#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CSEC (double)(CLOCKS_PER_SEC)
#define MAXLENTH 30
#define MAXNUMBER 10000

struct word{
  char str[MAXLENTH];
  struct word *next;
};
typedef struct word Word;

Word* AllocateNode(char *s);
void Printlist(Word *l);
void performance(char *file_name);

int main(int argc,char **argv)
{
  if (argc==2)
  {
    performance(argv[1]);
  }
  else {
    printf("ERROR: Incorrect usage. try e.g. "\
    "%s sternaig\n",argv[0]);
  }
  return 0;
}

void performance(char *file_name)
{
  FILE *fp;
  Word *start,*current,*order,*mid,*new;
  char temp[MAXLENTH];
  int cnt=0;
  clock_t c1,c2;
  if ((fp=fopen(file_name,"r"))==NULL){
    printf("Fail to open the file\n");
    exit(EXIT_FAILURE);
  }
  c1= clock();
  if (fscanf(fp,"%s",temp)==1) {
    start=current=AllocateNode(temp);
    cnt++;
  }
  else exit(EXIT_FAILURE);

  while (fscanf(fp,"%s",temp)==1){
    if (strcmp(temp,current->str)>0){
      current->next=AllocateNode(temp);
      current=current->next;
    }
    else {
      order=start;
      while (strcmp(temp,order->str)>0){
        mid=order;
        order=order->next;
      }
      if (order!=start){
        new=AllocateNode(temp);
        new->next=order;
        mid->next=new;
      }
      else {
        new=AllocateNode(temp);
        new->next=order;
        start=new;
      }
    }
    cnt++;
  }
  fclose(fp);
  c2= clock();
  Printlist(start);
  printf("There are %d words\n",cnt);
  printf("Time of creating list:%f\n",(double)(c2-c1)/CSEC);
}

Word* AllocateNode(char *s)
{
  Word *p;
  p=(Word *)malloc(sizeof(Word));
  if (p==NULL) {
    printf("Cannot allocate Node\n");
    exit(EXIT_FAILURE);
  }
  strcpy(p->str,s);
  p->next=NULL;
  return p;
}

void Printlist(Word *l)
{
  int cnt=1;
  do {
    printf("%d:%s\n",cnt++,l->str);
    l=l->next;
  } while (l!=NULL);
}
