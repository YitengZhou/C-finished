#include <stdio.h>
#include <ctype.h>

struct student{
  char *surname;
  int id;
  char grade;
};

int failures(struct student j[],int n);

int main(void)
{
  struct student class[]={
    {"Bloggs",95112174,'c'},
    {"Doobeedoo",96162578,'b'},
    {"Campbell",96112103,'f'},
    {"Johnson",96185617,'a'}
  };
  struct student *p;
  p=&class[0];
  printf("Number of fails:%d\n",failures(class,4));
  printf("%d\n",p++->id);
  printf("%d\n",p++->id);
  printf("%s:%c\n",(*p).surname,p->grade);
  return 0;
}

int failures(struct student j[],int n)
{
  int i,cnt=0;
  for (i=0;i<n;i++) {
    cnt += (tolower(j->grade)=='f');
    j++;}
  return cnt;
}
