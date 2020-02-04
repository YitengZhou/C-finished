#include <stdio.h>
#include <assert.h>

int isprime(int number);

int main (void)
{
  char a[26];
  int i,k,sum=0,cnt=0;
  for (i=0;i<26;i++)
  a[i]='a'+i;

  for (i=0;i<26;i++)
    {if (a[i]=='a'||a[i]=='e'||a[i]=='i'||a[i]=='o'||a[i]=='u')
    {
      for (k=0;k<26;k++)
      {sum= 2*i+k+3;

      if (isprime(sum) && (a[k]!='a'&& a[k]!='e'&& a[k]!='i'&& a[k]!='o'&& a[k]!='u'))
      {cnt++;
      printf("%5d %c%c%c\n",cnt,a[i],a[k],a[i]);}
    }
    }
    else
    {
      for (k=0;k<26;k++)
      {sum= 2*i+k+3;
      if (isprime(sum) && (a[k]=='a'||a[k]=='e'||a[k]=='i'||a[k]=='o'||a[k]=='u'))
      {cnt++;
      printf("%5d %c%c%c\n",cnt,a[i],a[k],a[i]);}
    }}}
  /* assert(isprime(2)==1);
  assert(isprime(3)==1);
  assert(isprime(4)==0);
  assert(isprime(5)==1);
  assert(isprime(13)==1);
  assert(isprime(24)==0);*/
  return 0;
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
