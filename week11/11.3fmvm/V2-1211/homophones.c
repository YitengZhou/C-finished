/* This program could read the dictionary file "cmudict.txt",
   and store the data in two maps, search phomemes using map1
   and search matching words using map2, print all homophones
   with same number of phonemes and free all memory */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>

#include "fmvm.h"

#define MAXLEN 200
#define CSEC (double)(CLOCKS_PER_SEC)

void Performance(char* key,int number);
void CreatMap(mvm* map1,mvm* map2,int number);
int ReadWord(char *word,char *read);
void ReadPhoneme(char *phoneme,char *read,int hash_position);
char* Extract(char* phoneme,int number);
int CountPhoneme(char* phoneme);
void PrintRhyme(mvm* map1,mvm* map2,char *key,int number);
void test(void);

int main(int argc,char **argv)
{
  /* Number 3 is assumed if no -n flag */
  int i=1,number=3;
  if (argc>=2)
  {
    /* Identify the -n flag specifies */
    if (strcmp(argv[i],"-n")==0){
      number=atoi(argv[++i]);
      if (number==0){
        ON_ERROR("ERROR: Incorrect number of phonemes.\n");
      }
      i++;
    }
    while (i<argc){
      Performance(argv[i++],number);
    }
    test();
  }
  else {
    fprintf(stderr,"ERROR: Incorrect usage. try e.g. "\
    "%s -n 3 RHYME\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  return 0;
}

/* Read from file and create map1 and map2,
   print all homophones result and free memory */
void Performance(char* key,int number)
{
  mvm *map1,*map2;
  /*clock_t c1,c2,c3,c4,c5,c6,c7,c8;*/
  /*c1= clock();*/
  map1=mvm_init();
  map2=mvm_init();
  /*c2= clock();*/
  /*c3= clock();*/
  CreatMap(map1,map2,number);
  /*c4= clock();*/
  /*c5= clock();*/
  PrintRhyme(map1,map2,key,number);
  /*c6= clock();*/
  /*c7= clock();*/
  mvm_free(&map1);
  mvm_free(&map2);
  /*c8= clock();*/
  /*
  printf("Initial Map time:%fs\n",(double)(c2-c1)/CSEC);
  printf("Creat Map1 Map2 time:%fs\n",(double)(c4-c3)/CSEC);
  printf("Searching and Print:%fs\n",(double)(c6-c5)/CSEC);
  printf("Free two map :%fs\n",(double)(c8-c7)/CSEC);*/
}

/* Read from file and identify key/value insert into maps */
void CreatMap(mvm* map1,mvm* map2,int number)
{
  FILE *fp;
  char read[MAXLEN];
  char word[MAXLEN],phoneme[MAXLEN];
  int hash_position;
  fp=fopen("cmudict.txt","r");
  if (fp==NULL){
    ON_ERROR("Fail to open the file\n");
  }
  while (fgets(read,MAXLEN,fp)){
    hash_position=ReadWord(word,read);
    ReadPhoneme(phoneme,read,hash_position);
    Extract(phoneme,number);
    mvm_insert(map1, word, phoneme);
    mvm_insert(map2, phoneme, word);
  }
  /*PrintHashtable(map1);
  PrintHashtable(map2);*/
  fclose(fp);
}

/* Read word and return hash position */
int ReadWord(char *word,char *read)
{
  int hash_position=0,j=0;
  while (read[hash_position]!='#'){
    word[j++]=read[hash_position++];
  }
  word[hash_position++]='\0';
  return hash_position;
}

/* Read phoneme after hash and befor '\n' */
void ReadPhoneme(char *phoneme,char *read,int hash_position)
{
  int i=hash_position,j=0;
  while (read[i]!='\n'){
    phoneme[j++]=read[i++];
  }
  /* Because every line before '\n' have a '\r' character,
     instead of this character to '\0'.
     If new file haven't this '\r',
     last character before '\n' must be a letter or digit */
  if (!isalnum(phoneme[j-1])){
    phoneme[j-1]='\0';
  }
  else {
    phoneme[j]='\0';
  }
}

/* Return the correct number of phonemes,
   which is assumed with -n flag (default, 3 is assumed) */
char* Extract(char* phoneme,int number)
{
  char temp[MAXLEN];
  int cnt,i,j=0,length=strlen(phoneme);
  cnt=CountPhoneme(phoneme);
  if (cnt<=number){
    return phoneme;
  }
  else{
    i=length-1;
    while (number>0){
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

/* Return how many phonemes word has */
int CountPhoneme(char* phoneme)
{
  int i,cnt=1,length=strlen(phoneme);
  for (i=0;i<length;i++){
    if (phoneme[i]==' '){
      cnt++;
    }
  }
  return cnt;
}

/* Print the list of words to be matched */
void PrintRhyme(mvm* map1,mvm* map2,char *key,int number)
{
  int i,count=0;
  char** list;
  /* In case search for word that is not in the file */
  if (mvm_search(map1,key)==NULL){
    ON_ERROR("Search word not in the file, check again\n");
  }

  list=mvm_multisearch(map2,mvm_search(map1,key),&count);
  printf("%s (%s) : ",key,mvm_search(map1,key));
  for (i=0;i<count;i++){
    printf("%s ",list[i]);
  }

  /* If use -n flag greater than the number of phonemes */
  if (CountPhoneme(mvm_search(map1,key))<number){
    printf("(Because -n flag (%d) greater than "\
    "the number of phonemes, the results are undefined "\
    "by only match those phonemes)\n",number);
  }

  printf("\n");
  free(list);
}

/* Test and assert function */
void test(void)
{
  char word1[MAXLEN]="DILEMMAS#D AH0 L EH1 M AH0 Z\n";
  char word2[MAXLEN]="KOOKY#K UW1 K IY0\r\n";
  char read[MAXLEN];
  int i;

  /* test function ReadWord return correct hash position */
  assert(ReadWord(read,word1)==9);
  i=strcmp(read,"DILEMMAS");
  assert(i==0);
  i=strcmp(word1,"DILEMMAS#D AH0 L EH1 M AH0 Z\n");
  assert(i==0);

  assert(ReadWord(read,word2)==6);
  i=strcmp(read,"KOOKY");
  assert(i==0);
  i=strcmp(word2,"KOOKY#K UW1 K IY0\r\n");
  assert(i==0);

  /* test function ReadPhoneme with correct read
     with special case with \r or without \r before \n */
  ReadPhoneme(read,word1,9);
  i=strcmp(read,"D AH0 L EH1 M AH0 Z");
  assert(i==0);
  i=strcmp(word1,"DILEMMAS#D AH0 L EH1 M AH0 Z\n");
  assert(i==0);

  ReadPhoneme(read,word2,6);
  i=strcmp(read,"K UW1 K IY0");
  assert(i==0);
  i=strcmp(word2,"KOOKY#K UW1 K IY0\r\n");
  assert(i==0);

  /* test function Extract with correct read */
  Extract(read,6);
  i=strcmp(read,"K UW1 K IY0");
  assert(i==0);
  Extract(read,5);
  i=strcmp(read,"K UW1 K IY0");
  assert(i==0);
  Extract(read,4);
  i=strcmp(read,"K UW1 K IY0");
  assert(i==0);
  Extract(read,3);
  i=strcmp(read,"UW1 K IY0");
  assert(i==0);
  Extract(read,2);
  i=strcmp(read,"K IY0");
  assert(i==0);
  Extract(read,1);
  i=strcmp(read,"IY0");
  assert(i==0);

  /* test function Extract with correct read */
  assert(CountPhoneme(read)==1);
  assert(CountPhoneme(word1)==7);
  assert(CountPhoneme(word2)==4);
}
