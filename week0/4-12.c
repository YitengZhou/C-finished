#include <stdio.h>
#define NDEBUG
#include <assert.h>
#define HOWMANYNUMBERYOULIKE 3000

enum bool{false,true};
typedef enum bool bool;

bool isprime(int n);
void test(void);

int main(void)
{
  int i,cnt;
  i=2;
  cnt=1;
  while (cnt<= HOWMANYNUMBERYOULIKE)
  {
    if (isprime(i)==1){
    printf("%4d: number %d is prime\n",cnt,i);
    cnt++;
    }
    i++;
  }
  test();
  return 0;
}

void test(void)
{
  assert(isprime(1)==0);
  assert(isprime(2)==1);
  assert(isprime(3)==1);
  assert(isprime(4)==0);
  assert(isprime(100)==0);
  assert(isprime(37)==1);
}

/*return whether n is prime */
bool isprime(int n)
{
  int i;
  if (n==1)
  return false;
  if (n==2)
  return true;
  else
    for (i=2;i<n;i++)
    {
      if (n%i==0)
      return false;
    }
    return true;
}
