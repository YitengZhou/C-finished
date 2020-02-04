#include <stdio.h>
#include <ctype.h>

struct date {
  short day;
  short month;
  short year;
};

struct person {
  char name[25];
  struct date data;
};

void print_person_copy (struct person p);
void print_person_point (struct person *p);

int main (void)
{
  struct person k;
  printf("Enter you name:\n");
  scanf("%s",k.name);
  printf("Enter you Birthday:\n");
  scanf("%hd/%hd/%hd",&k.data.day,&k.data.month,&k.data.year);

  print_person_copy(k);
  print_person_point(&k);
  return 0;
}

void print_person_copy(struct person p)
{
  printf("Name \"%s\", Born %02d/%02d/%d\n",p.name, p.data.day, p.data.month, p.data.year);
}

void print_person_point(struct person *p)
{
  printf("Name \"%s\", Born %02hd/%02hd/%hd\n",p->name, p->data.day, p->data.month, p->data.year);
}
