#include <stdio.h>
#include <stdlib.h>

#define MAXNUMBER 100

struct factor {
  int num;
  int count;
};
typedef struct factor Factor;

void Performance(char *string);
int GetFactor(long int number,Factor F[MAXNUMBER]);
void PrintResult(Factor F[MAXNUMBER],int j,char *string);
int isprime(int number);

int main(int argc,char **argv)
{
  if (argc==2)
  {
    Performance(argv[1]);
  }
  else {
    fprintf(stderr,"ERROR: Incorrect usage. try e.g. "\
    "%s 100\n",argv[0]);
  }
  return 0;
}

void Performance(char *string)
{
  Factor F[MAXNUMBER];
  long int number;
  int count;
  number=atol(string);
  count=GetFactor(number,F);
  PrintResult(F,count,string);
}

int GetFactor(long int number,Factor F[MAXNUMBER])
{
  long int cnt=2,i=0;
  F[0].num=1;F[0].count=1;
  while (number!=1){
    if (number%cnt==0){
      number=number/cnt;
      if (F[i].num!=cnt){
        i++;
        F[i].num=cnt;
        F[i].count++;
      }
      else {
        F[i].count++;
      }
    }
    else{
      do{
        cnt++;
      } while (!isprime(cnt));
    }
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

void PrintResult(Factor F[MAXNUMBER],int count,char *string){
  int i;
  printf("%s = %d",string,F[0].num);
  for (i=1;i<=count;i++){
    if (F[i].count==1){
      printf(" x %d",F[i].num);
    }
    else {
      printf(" x %d^%d",F[i].num,F[i].count);
    }
  }
  printf("\n");
}
