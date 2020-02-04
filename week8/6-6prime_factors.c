#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "neillsdl2.h"

#define MAXNUMBER 100
#define CIRCLE 360
#define RECTSIZE 40
#define MILLISECONDDELAY 1000
#define PI 3.14159265

struct factor {
  int num;
  int count;
};
typedef struct factor Factor;

void Performance(char *string);
int GetFactor(long int number,Factor F[MAXNUMBER]);
void PrintResult(Factor F[MAXNUMBER],int j,char *string);
int isprime(int number);
void ShowSDL(Factor F[MAXNUMBER],long int number,int count);

int main(int argc,char **argv)
{
  if (argc==2)
  {
    Performance(argv[1]);
  }
  else {
    fprintf(stderr,"ERROR: Incorrect usage. try e.g. "\
    "%s 100\n",argv[0]);
  }
  return 0;
}

void Performance(char *string)
{
  Factor F[MAXNUMBER];
  long int number;
  int count;
  number=atol(string);
  count=GetFactor(number,F);
  PrintResult(F,count,string);
  ShowSDL(F,number,count);
}

int GetFactor(long int number,Factor F[MAXNUMBER])
{
  long int cnt=2,i=0;
  F[0].num=1;F[0].count=1;
  while (number!=1){
    if (number%cnt==0){
      number=number/cnt;
      if (F[i].num!=cnt){
        i++;
        F[i].num=cnt;
        F[i].count++;
      }
      else {
        F[i].count++;
      }
    }
    else{
      do{
        cnt++;
      } while (!isprime(cnt));
    }
  }
  return i;
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

void PrintResult(Factor F[MAXNUMBER],int count,char *string){
  int i;
  if (count==1){
    printf("This is prime!\n");
  }
  printf("%s = %d",string,F[0].num);
  for (i=1;i<=count;i++){
    if (F[i].count==1){
      printf(" x %d",F[i].num);
    }
    else {
      printf(" x %d^%d",F[i].num,F[i].count);
    }
  }
  printf("\n");
}

/* showing with SDL*/
void ShowSDL(Factor F[MAXNUMBER],long int number,int count)
{
  SDL_Simplewin sw;
  fntrow fontdata[FNTCHARS][FNTHEIGHT];
  double angle1,angle2,angle3,r,x1,y1,x2,y2,x3,y3;
  int i,j,k,n,temp;
  angle1 = CIRCLE/F[count].num;
  angle3 = CIRCLE/F[count-1].count;
  angle2 = CIRCLE/F[count-1].num;
  r = 200/number;
  printf("%d\n",count);
  Neill_SDL_Init(&sw);
  do{
    n=0;
    SDL_Delay(MILLISECONDDELAY);
    Neill_SDL_ReadFont(fontdata,"mode7.fnt");
    /* Choose a random colour, a mixture of red, green and blue. */

    for (i=0;i<F[count].num;i++){
      x1=400+200*cos((angle1*i)*PI/180.0);
      y1=300+200*sin((angle1*i)*PI/180.0);
      temp=F[count-1].count;
      while (temp!=1){
        for (k=0;k<F[count-1].count;k++){
          x2=x1+200/F[count-1].count*cos((angle3*k)*PI/180.0);
          y2=y1+200/F[count-1].count*sin((angle3*k)*PI/180.0);
          for (j=0;j<F[count-1].num;j++){
            Neill_SDL_SetDrawColour(&sw,255,255-n*(255/number),255);
            x3=x2+r*F[count-1].num*cos((angle2*j)*PI/180.0);
            y3=y2+r*F[count-1].num*sin((angle2*j)*PI/180.0);
            Neill_SDL_RenderFillCircle(sw.renderer,x3,y3,r);
            n++;
          }
        }
        temp--;
      }

    }
    /* comments */
    Neill_SDL_DrawString(&sw,fontdata,"STEP",435,175);

    Neill_SDL_UpdateScreen(&sw);

    /* Has anyone pressed ESC or killed the SDL window ?
      Must be called frequently - it's the only way of escaping  */
    Neill_SDL_Events(&sw);

  }while(!sw.finished);

  /* Clear up graphics subsystems */
  SDL_Quit();
  atexit(SDL_Quit);
}
