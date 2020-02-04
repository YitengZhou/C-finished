#include <stdio.h>
#include <ctype.h>
int isvowel(int a);
int main(void)
{
int c;
while ((c= getchar())!=EOF)
{
if (isvowel(c))
putchar(toupper(c));
else
putchar(tolower(c));
}
return 0;}

int isvowel(int a)
{
int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10;
a1='a';a2='e';a3='o';a4='u';a5='v';
a6='A';a7='E';a8='O';a9='U';a10='V';
if (a==a1||a==a2||a==a3||a==a4||a==a5||a==a6||a==a7||a==a8||a==a9||a==a10)
return 1;
else
return 0;
}
