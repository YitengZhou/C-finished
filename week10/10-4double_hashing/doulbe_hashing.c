#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STARTNUMBER 53
#define MAXWORDS 25 /* 有一个单词是20个字符 */
#define MAXNUMBER 20000

struct block{
  int number;
  char word[MAXWORDS];
};
typedef struct block Block;

struct hash{
  Block *b;
  int count;
  int size;
};
typedef struct hash Hash;

Hash* Performance(char *string);
int SearchHash(Hash *h,char *word);
Hash* initial_hash(int s);
void Order(Hash *h,char *str);
int GetKey(char *str);
int GetKey1(char *str,int index);
int GetKey2(char *str,int index);
void PrintHash(Hash *h);
Hash* ReCreathash(Hash *h);
void Reorder(Hash *h,Hash *new);
int NewPrime(int number);
int isprime(int number);

int main(int argc,char **argv)
{
  Hash *h;
  int result;
  if (argc==3){
      h=Performance(argv[1]);
      result=SearchHash(h,argv[2]);
      if (result){
        printf("The average number for searching \"%s\" : %d\n"\
        ,argv[2],result);
      }
      else{
        printf("%s not in hash table\n",argv[2]);
      }
  }
  else{
    fprintf(stderr,"ERROR: Incorrect usage. try e.g. "\
    "%s 34words.txt apple\n",argv[0]);
  }
  return 0;
}

Hash* Performance(char *string)
{
  Hash *h;
  char str[MAXWORDS];
  FILE *fp;

  h=initial_hash(STARTNUMBER);
  fp=fopen(string,"r");
  if (fp==NULL){
    fprintf(stderr,"Failed to open the file.\n");
    exit(EXIT_FAILURE);
  }
  while (fscanf(fp,"%s",str)==1 && h->count<MAXNUMBER){
    Order(h,str);
    if (h->count*100/h->size>=60){
      h=ReCreathash(h);
    }
  }
  fclose(fp);
  PrintHash(h);
  printf("There are %d words in the file\n",h->count);
  return h;
}

int SearchHash(Hash *h,char *word)
{
  int key,i=0,flag=0;
  key=GetKey1(word,h->size);
  while (flag==0){
    if (h->b[key].number==0){
      return 0;
    }
    else{
      if (strcmp(word,h->b[key].word)==0){
        return i+1;
      }
      else{
        printf("It full because %d,%s(%d)\n",key,h->b[key].word,h->b[key].number);
        i++;
        key=(GetKey1(word,h->size)+i*GetKey2(word,h->size))%h->size;
      }
    }
  }
  return 0;
}

Hash* initial_hash(int s)
{
  Hash *h;
  h = (Hash *) calloc(1,sizeof(Hash));
  if(h == NULL){
     fprintf(stderr,"Creation of Hashing table Failed\n");
  }
  h->b = (Block*) calloc(s, sizeof(Block));
  if(h->b == NULL){
     fprintf(stderr,"Creation of Hashing table Failed\n");
  }
  h->size = s;
  return h;
}

void Order(Hash *h,char *str)
{
  int key,i=0,index,flag=0;
  index=h->size;
  key=GetKey1(str,index);
  if (h->b[key].number==0){
    strcpy(h->b[key].word,str);
    h->b[key].number=GetKey(str);
    h->count++;
  }
  else{
    while (flag==0){
      i++;
      key=(GetKey1(str,index)+i*GetKey2(str,index))%h->size;
      if (h->b[key].number==0){
        strcpy(h->b[key].word,str);
        h->b[key].number=GetKey(str);
        h->count++;
        flag=1;
      }
    }
  }
}

int GetKey(char *str)
{
  int i,length=strlen(str);
  double sum=0;
  if (length>5){
    length=5;
  }
  for (i=0;i<length;i++){
    sum+=(double)(str[i]-'a')*pow(26.0,length-i-1);
  }
  return (int)sum;
}

int GetKey1(char *str,int index)
{
  int i,length=strlen(str);
  double sum=0;
  if (length>5){
    length=5;
  }
  for (i=0;i<length;i++){
    sum+=(double)(str[i]-'a')*pow(26.0,length-i-1);
  }
  return (int)sum%index;
}

int GetKey2(char *str,int index)
{
  int i,newindex,length=strlen(str);
  double sum=0;
  if (length>5){
    length=5;
  }
  for (i=0;i<length;i++){
    sum+=(double)(str[i]-'a')*pow(26.0,length-i-1);
  }
  /*newprime double the index, newindex is about half index prime */
  newindex=NewPrime(index/4);
  return newindex-(int)sum%newindex;
}

void PrintHash(Hash *h)
{
  int i;
  for (i=0;i<h->size;i++){
    printf("%d:%s(%d)\n",i,h->b[i].word,h->b[i].number);
  }
}

Hash* ReCreathash(Hash *h)
{
  int newsize;
  Hash *new;
  newsize=NewPrime(h->size);
  printf("%d\n",newsize);
  new=initial_hash(newsize);
  Reorder(h,new);
  free(h->b);
  free(h);
  return new;
}

void Reorder(Hash *h,Hash *new)
{
  int i;
  for (i=0;i<h->size;i++){
    if (h->b[i].number!=0){
      Order(new,h->b[i].word);
    }
  }
}

int NewPrime(int s)
{
  int i=2*s;
  while (!isprime(i)){
    i++;
  }
  return i;
}

int isprime(int number)
{
  int i;
  if (number ==2)
  return 1;
  if (number ==3)
  return 1;
  for (i=2;i<number;i++)
    {if (number%i==0)
    return 0;}
  return 1;
}
