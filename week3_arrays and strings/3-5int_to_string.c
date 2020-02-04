#include <stdio.h>

void int2string(int a,char list[]);

int main (void)
{
  int i;
  char s[256];
  scanf("%d",&i);
  int2string(i,s);
  printf("%s\n",s);
  return 0;
}

void int2string(int a,char list[])
{
  int i,len=0,number=a,temp;
  char x[10]={'0','1','2','3','4','5','6','7','8','9'};
  while (number!=0)
  {
    number /= 10;
    len++;
  }

  if (a>0)
  { list[0]='+';
    for (i=1;i<=len;i++)
    {
      list[i]=x[a%10];
      a=a/10;
    }
    for (i=1;i<=len/2;i++)
      {temp=list[i];
      list[i]=list[len-i+1];
      list[len-i+1]=temp;
      }
    list[len+1]='\0';
  }
  else if (a<0)
  {list[0]='-';
  a=-1*a;
  for (i=1;i<=len;i++)
  {
    list[i]=x[a%10];
    a=a/10;
  }
  for (i=1;i<=len/2;i++)
    {temp=list[i];
    list[i]=list[len-i+1];
    list[len-i+1]=temp;
    }
  list[len+1]='\0';
  }

  else
  {list[0]='0';
  list[1]='\0';}
}
