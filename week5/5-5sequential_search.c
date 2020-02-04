#include <stdio.h>
#include <string.h>

#define NUMPEOPLE 6

struct key{
  char *name;
  int age;
};

typedef struct key Key;

int FindAge(char *name,Key *l,int n);

int main(void)
{
  int i,j;
  Key a[NUMPEOPLE]={
    {"Ackerby",21},
    {"Bloggs",25},
    {"Chumley",26},
    {"Dalton",25},
    {"Eggson",22},
    {"Fulton",41},
  };
  i=FindAge("Eggson",a,NUMPEOPLE);
  j=FindAge("Campbell",a,NUMPEOPLE);
  printf("%d %d\n",i,j);
  return 0;
}

/*int FindAge(char *name,Key *l,int n)
{
  int i;
  for (i=0;i<n;i++) {
    if (strcmp(l[i].name,name)==0){
      return l[i].age;
    }
  }
  return -1;
}*/

int FindAge(char *name,Key *l,int n)
{
  int j,m;
  for (j=0;j<n;j++) {
    m=strcmp(name,l[j].name);
    if (m==0){
      return l[j].age;
    }
    if (m<0) {
      return -1;
    }
  }
  return -1;
}
