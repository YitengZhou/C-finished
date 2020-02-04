#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "mvm.h"

#define MAXLEN 100

void Performance(void);
void InitialMap(mvm* map1,mvm* map2,int number);
char* Extract(char* phoneme,int number);
void test(void);

int main(int argc,char **argv)
{
  if (argc>=2)
  {
    Performance();
    test();
  }
  else {
    fprintf(stderr,"ERROR: Incorrect usage. try e.g. "\
    "%s -n 3 RHYME\n",argv[0]);
  }
  return 0;
}

void Performance(void)
{
  mvm *map1,*map2;
  mvmcell *print1,*print2;
  int number=3;

  map1=mvm_init();
  map2=mvm_init();
  InitialMap(map1,map2,number);

  print1=map1->head;
  print2=map2->head;

  while (print1!=NULL && print2!=NULL){
    printf("(%s):[%s]\n",print1->key,print1->data);
    printf("(%s):[%s]\n",print2->key,print2->data);
    print1=print1->next;
    print2=print2->next;
  }
}

void InitialMap(mvm* map1,mvm* map2,int number)
{
  FILE *fp;
  char word[MAXLEN],phoneme[MAXLEN];
  char ch;
  int i=0,flag=0;
  fp=fopen("cmudict.txt","r");
  if (fp==NULL){
    ON_ERROR("Fail to open the file\n");
  }
  while ((ch=getc(fp))!=EOF){
    if (ch!='\n' && ch!='#'){
      if (flag==0){
        word[i++]=ch;
      }
      if (flag==1){
        phoneme[i++]=ch;
      }
    }
    else {
      if (flag==0){
        word[i]='\0';
      }
      /* Because every line before '\n' have a unknown character,
         instead of this unknown to '\0'.
         If new file haven't this unknown character,
         this character must be a letter or digit */
      if (flag==1){
        if (!isalnum(phoneme[i-1])){
          phoneme[i-1]='\0';
        }
        else {
          phoneme[i]='\0';
        }
        mvm_insert(map1, word, phoneme);
        mvm_insert(map2, phoneme, word);
      }
    }
    if (ch=='#'){
      flag=1;
      i=0;
    }
    if (ch=='\n'){
      flag=0;
      i=0;
    }
  }
  fclose(fp);
  printf("%d",number);
}

char* Extract(char* phoneme,int number)
{
  char temp[MAXLEN];
  int cnt=1,i,j=0,length=strlen(phoneme);
  for (i=0;i<length;i++){
    if (phoneme[i]==' '){
      cnt++;
    }
  }
  if (cnt<number){
    return NULL;
  }
  else{
    i=length-1;
    while (number>0 && i>=0){
      temp[j++]=phoneme[i--];
      if (phoneme[i]==' '){
        number--;
      }
    }
    for (i=0;i<j;i++){
      phoneme[i]=temp[j-i-1];
    }
    phoneme[i]='\0';
    return phoneme;
  }
}

void test(void)
{
  char a[MAXLEN]="K AH0 M P L IY1 T N AH0 S";
  Extract(a,4);
  assert(strcmp(a,"T N AH0 S")==0);
  Extract(a,2);
  assert(strcmp(a,"AH0 S")==0);
  Extract(a,2);
  assert(strcmp(a,"AH0 S")==0);
  assert(Extract(a,5)==NULL);
}
