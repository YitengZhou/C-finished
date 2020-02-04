#include <stdio.h>

void bubble_sort(int b[],int s);

int main(void)
{
  int i;
  int a[]={3,4,1,2,9,0};

  bubble_sort(a,6);

  for (i=0;i<6;i++)
  {
    printf("%d ",a[i]);
  }
  printf("\n");
  return 0;
}

void bubble_sort(int b[],int s)
{
  int i,temp;
  int change;

  do{
    change=0; /*重置change=0，若有change就继续，无change时就跳出，也可用双层嵌套*/
    for (i=0;i<s-1;i++)
    {
      if (b[i]>b[i+1])
      {
        temp=b[i];
        b[i]=b[i+1];
        b[i+1]=temp;
        change++;
      }
    }
  }while (change);
}
