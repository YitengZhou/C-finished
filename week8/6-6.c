#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "neillsdl2.h"

#define MAXNUMBER 100
#define CIRCLE 360
#define RECTSIZE 40
#define MILLISECONDDELAY 1000
#define CIRCLE_SIZE 200
#define CENTRE_X 400
#define CENTRE_Y 300

struct factor {
  int num;
  int count;
};
typedef struct factor Factor;

void Performance(char *string);
int GetFactor(long int number,Factor F[MAXNUMBER]);
int PrintResult(Factor F[MAXNUMBER],int show[],int j,char *string);
int isprime(int number);
void ShowSDL(int show[]);
void DisplayFactor(SDL_Simplewin* sw,int show[],int flag, int x, int y, int r);


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
  int show[MAXNUMBER]={0};
  long int number;
  int count,n;
  number=atol(string);
  count=GetFactor(number,F);
  n=PrintResult(F,show,count,string);
  ShowSDL(show);
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

int PrintResult(Factor F[MAXNUMBER],int show[],int count,char *string){
  int i,j,cnt=0;
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
  while (i>0){
    for (j=0;j<F[i].count;j++){
      show[cnt]=F[i].num;
      cnt++;
    }
    i--;
  }
  show[cnt++]=1;
  return cnt;
}

/* showing with SDL*/
void ShowSDL(int show[])
{
  SDL_Simplewin sw;
  fntrow fontdata[FNTCHARS][FNTHEIGHT];
  Neill_SDL_Init(&sw);
  do{
    SDL_Delay(MILLISECONDDELAY);
    Neill_SDL_ReadFont(fontdata,"mode7.fnt");
    /* Choose a random colour, a mixture of red, green and blue. */
    DisplayFactor(&sw,show,0,CENTRE_X, CENTRE_Y, CIRCLE_SIZE);

    Neill_SDL_UpdateScreen(&sw);
    /* Has anyone pressed ESC or killed the SDL window ?
      Must be called frequently - it's the only way of escaping  */
    Neill_SDL_Events(&sw);

  }while(!sw.finished);

  /* Clear up graphics subsystems */
  SDL_Quit();
  atexit(SDL_Quit);
}

void DisplayFactor(SDL_Simplewin* sw,int show[],int flag, int x, int y, int r)
{
  int i;
  double angle;
  double n;
  int newx, newy,newr;

  if (show[flag] == 1) {
     Neill_SDL_SetDrawColour(sw, 255, 255, 255);
     Neill_SDL_RenderFillCircle(sw->renderer, x, y, r);
     return;
  }

  n = (double) show[flag];
  for (i = 0; i < show[flag]; i++) {
     newx = newy = newr = 0;
     angle = i * ((2 * M_PI) / n);
     newx = x + r * sin(angle);
     newy = y + r * cos(angle);
     newr = (r / n);
     DisplayFactor(sw,show,flag+1, newx, newy, newr);
  }
}
