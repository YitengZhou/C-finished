#include <stdio.h>
#include <ctype.h>

int main(void)
{
  int cnt=1;
  char i,j,k;
  for (i='a';i<='z';i++)
    {if (i!='a'&&i!='e'&& i!='i'&&i!='o'&&i!='u')
      {for (j='a';j<='z';j++)
        {if (j=='a'||j=='e'||j=='i'||j=='o'||j=='u')
        {for (k='a';k<='z';k++)
        {if (k!='a'&&k!='e'&&k!='i'&&k!='o'&&k!='u')
          {printf("%5d %c%c%c\n",cnt,i,j,k);
          cnt++;}
        }
        }
      }
      }
    }
  return 0;
}
