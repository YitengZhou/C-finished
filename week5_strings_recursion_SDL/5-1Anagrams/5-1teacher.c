#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define ALPHA 26
#define MAXLEN 40
#define MAXWORDS 500000

typedef enum bool {false,true} bool;

struct word{
  char s[MAXLEN];
  int l;
};
typedef struct word word;

bool isAnagram(word * w1,word * w2);
void test(void);
void fillword(word *w,char *s);

int main(int argc,char **argv)
{
  static word list[MAXWORDS];
  FILE *fp;
  int i=0;
  char str[MAXLEN];
  int nw;
  word argvw;

  if(argc !=2){
    fprintf(stderr,"Usage: %s <word>\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  if ((fp=fopen("eng_370k_shuffle.txt","r"))==NULL) {
    fprintf(stderr,"Can't find dictionary\n");
    exit(EXIT_FAILURE);
  }

/*运用fscanf读入并放到struct*/
  while (fscanf(fp,"%s",str)==1){
    fillword(&list[i],str);
    i++;
  }

  nw=i;
  printf("Read %d words\n",nw);
  test();
  fillword(&argvw, argv[1]);

  for (i=0;i<nw;i++) {
    if (isAnagram(&list[i],&argvw))
    {
      printf("%s\n",list[i].s);
    }
  }
  fclose(fp);
  return 0;
}

bool isAnagram(word * w1,word * w2)
{
  short hist[ALPHA]={0};
  int i;

  if ((w1->l)!=(w2->l)){
    return false;
  }

  for (i=0;i<(w1->l);i++){
    hist[(w1->s)[i]-'a']++;
    hist[(w2->s)[i]-'a']--; /*一加一减*/
  }
  for (i=0;i<ALPHA;i++){
    if(hist[i]!=0){
      return false;
    }
  }
  return true;
}
/*
void test(void)
{
  assert(isAnagram("elvis","lives")==true);
  assert(isAnagram("elvis","fives")==false);
  assert(isAnagram("lives","elvis")==true);
}*/

void fillword(word *w,char *s){
  strcpy(w->s,s);
  w->l=strlen(s);
}

void test(void)
{
word w1 = {"elvis",5};
word w2 = {"lives",5};
word w3 = {"flvis",5};
assert(isAnagram(&w1,&w2)==true);
assert(isAnagram(&w2,&w3)==false);
assert(isAnagram(&w2,&w1)==true);
}
