#include <stdio.h>
#include <stdlib.h>
#include "neillsdl2.h"

#define RECTSIZE 40
#define MILLISECONDDELAY 1000
#define GAP 5
#define ORIGIN 100
#define TARGET 300
#define CHAR 112

#define FNTWIDTH (sizeof(fntrow)*8)
#define FNTHEIGHT 18
#define FNTCHARS 96
#define FNT1STCHAR 32

/* red-1 =255,0,0 */
/* orange-2 =255,125,0 */
/* yellow-3 =255,255,0 */
/* green-4 = 0,255,0 */
/* blue-5 = 0,0,255 */
/* navy-6 = 0,255,255 */
/* purple-7 = 255,0,255 */
/* white-8 = 255,255,255 */
/* black-' ' = 0,0,0 */

void ChangeColor(SDL_Simplewin sw,char ch);

int main(void)
{
   SDL_Simplewin sw;
   SDL_Rect rectangle;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];
   int i,j;
   char parent[3][3] = {{'1',' ','3'},{'4','2','5'},{'6','7','8'}};
   char target[3][3] = {{'1','2','3'},{'4','5','6'},{'7','8',' '}};
   char simple[3][3] = {{'1','2','3'},{'4',' ','5'},{'6','7','8'}};
   rectangle.w = RECTSIZE;
   rectangle.h = RECTSIZE;

   Neill_SDL_Init(&sw);
   do{

      SDL_Delay(MILLISECONDDELAY);
      Neill_SDL_ReadFont(fontdata,"mode7.fnt");

      /* Filled Rectangle, fixed size, random position */
      for (i=0;i<3;i++){
        for (j=0;j<3;j++){
          ChangeColor(sw,parent[i][j]);
          rectangle.x = ORIGIN + j*(GAP+RECTSIZE);
          rectangle.y = ORIGIN + i*(GAP+RECTSIZE);
          SDL_RenderFillRect(sw.renderer, &rectangle);
          Neill_SDL_DrawChar(&sw,fontdata,parent[i][j],CHAR+j*(GAP+RECTSIZE),CHAR+i*(GAP+RECTSIZE));
        }
      }


      for (i=0;i<3;i++){
        for (j=0;j<3;j++){
          ChangeColor(sw,target[i][j]);
          rectangle.x = ORIGIN + j*(GAP+RECTSIZE);
          rectangle.y = TARGET + i*(GAP+RECTSIZE);
          SDL_RenderFillRect(sw.renderer, &rectangle);
          Neill_SDL_DrawChar(&sw,fontdata,target[i][j],CHAR+j*(GAP+RECTSIZE),CHAR+200+i*(GAP+RECTSIZE));
        }
      }

      for (i=0;i<3;i++){
        for (j=0;j<3;j++){
          ChangeColor(sw,simple[i][j]);
          rectangle.x = 400 + j*(GAP+RECTSIZE);
          rectangle.y = 200 + i*(GAP+RECTSIZE);
          SDL_RenderFillRect(sw.renderer, &rectangle);
          Neill_SDL_DrawChar(&sw,fontdata,simple[i][j],CHAR+300+j*(GAP+RECTSIZE),CHAR+100+i*(GAP+RECTSIZE));
        }
      }


      Neill_SDL_UpdateScreen(&sw);
      /* Has anyone pressed ESC or killed the SDL window ?
         Must be called frequently - it's the only way of escaping  */
      Neill_SDL_Events(&sw);

   }while(!sw.finished);

   /* Clear up graphics subsystems */
   SDL_Quit();
   atexit(SDL_Quit);

   return 0;
}

void ChangeColor(SDL_Simplewin sw,char ch)
{
  switch (ch)
  {
    case '1':{Neill_SDL_SetDrawColour(&sw, 255,0,0);break;}
    case '2':{Neill_SDL_SetDrawColour(&sw, 255,125,0);break;}
    case '3':{Neill_SDL_SetDrawColour(&sw, 255,255,0);break;}
    case '4':{Neill_SDL_SetDrawColour(&sw, 0,255,0);break;}
    case '5':{Neill_SDL_SetDrawColour(&sw, 0,0,255);break;}
    case '6':{Neill_SDL_SetDrawColour(&sw, 0,255,255);break;}
    case '7':{Neill_SDL_SetDrawColour(&sw, 255,0,255);break;}
    case '8':{Neill_SDL_SetDrawColour(&sw, 255,255,255);break;}
    case ' ':{Neill_SDL_SetDrawColour(&sw, 0,0,0);break;}
  }
}
