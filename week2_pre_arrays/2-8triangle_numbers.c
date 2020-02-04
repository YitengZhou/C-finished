#include <stdio.h>
int sum(int a);
void figure(int b);
int main(void)
{
  int number,s;
  printf("Enter how many numbers your want count\n");
  scanf("%d",&number);
  s=sum(number);
  printf("Sum is %d.\n",s);
  printf("Here is figure:\n");
  figure(s);
  return 0;
}
int sum(int a)
{
  if (a==1)
  return 1;
  else
  return sum(a-1)+a;
}
void figure(int b)
{
  int i,line=1,count=0;
  for (i=1;i<=b;i++)
  { count++;
    printf("%d ",i);
    if (count==line)
    {
      printf("\n");
      line++;
      count=0;
    }
  }
}
