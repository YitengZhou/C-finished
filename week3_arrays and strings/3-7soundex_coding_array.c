#include <stdio.h>
#include <ctype.h>
void Soundex(char ar[]);
void read_in(char line[]);
char list[6][8]={{'b','f','p','v'},{'c','g','j','k','q','s','x','z'},{'d','t'},{'l'},{'m','n'},{'r'},};

int main(void)
{
  int i=0,j;
  char surname[100];
  read_in(surname);
  printf("%c",toupper(surname[0]));
  Soundex(surname);
  return 0;
}

void read_in(char line[])
{
  char c,i=0,j;
  while ((c=getchar())!='\n'&&c!=EOF)
  {
    line[i]=tolower(c);
    i++;
  }
  line[i]='\0';

}

void Soundex(char ar[])
{
  int i=0,x,y,cnt=0;
  int row=-1,flag; /*row用于记录字母的soundex, flag用于记录第几个数字*/
  for (x=0;x<6;x++)
  {
    for (y=0;y<8;y++)
    {
      if (ar[i]==list[x][y])
      row=x;}
    }
    flag=0;
  for (i=1;(ar[i]&&cnt<3);i++)
  {
    for (x=0;x<6;x++)
    {
      for (y=0;y<8;y++)
      {
        if (ar[i]==list[x][y] && (x!=row || (i-flag>1 && (i-flag!=2 && ar[i-1]!='w' && ar[i-1]!='h'))))
        {
        printf("%d",x+1);
        row=x;
        flag=i;
        cnt++;
        break;}
      }
    }
  }
  switch (cnt)
  {case 0: printf("000");break;
  case 1: printf("00");break;
  case 2: printf("0");break;}
}
