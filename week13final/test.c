#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL.h"

#define main main


int getMouseKeyEven(void *opaque) {
    SDL_Event ev;
    int quit = 0;
    while (!quit) 
    {
        while (SDL_PollEvent(&ev))
        {
            if (SDL_KEYDOWN == ev.type) // SDL_KEYUP
            {
                if (SDLK_DOWN == ev.key.keysym.sym) 
                {
                    printf("SDLK_DOWN ...............\n");

                }else if (SDLK_UP == ev.key.keysym.sym) 
                {
                    printf("SDLK_UP ...............\n");

                }else if (SDLK_LEFT == ev.key.keysym.sym) 
                {
                    printf("SDLK_LEFT ...............\n");

                }else if (SDLK_RIGHT == ev.key.keysym.sym) 
                {
                    printf("SDLK_RIGHT ...............\n");
                }
            }
            else if (SDL_MOUSEBUTTONDOWN == ev.type) 
            {
                if(SDL_BUTTON_LEFT == ev.button.button)
                {
                    int px = ev.button.x;
                    int py = ev.button.y;
                    printf("x, y %d %d ...............\n", px, py);

                }
                else if(SDL_BUTTON_RIGHT == ev.button.button)
                {
                    int px = ev.button.x;
                    int py = ev.button.y;
                    printf("x, y %d %d ...............\n", px, py);
                }
            }
            else if (SDL_MOUSEMOTION == ev.type) 
            {
                int px = ev.motion.x;
                int py = ev.motion.y;

                printf("x, y %d %d ...............\n", px, py);
            }
            else if (SDL_QUIT == ev.type) 
            {
                printf("SDL_QUIT ...............\n");
                return 1;
            }
        }
    }

    return 0;
}
int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO)) {  
        printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
        return -1;
    } 
    SDL_Window *screen; 
    //SDL 2.0 Support for multiple windows
    screen = SDL_CreateWindow("Simplest Video Play SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600,SDL_WINDOW_OPENGL);
    if(!screen) {  
        printf("SDL: could not create window - exiting:%s\n",SDL_GetError());  
        return -1;
    }
    while (1) {
        SDL_Delay(10);
        if (getMouseKeyEven(NULL)){
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}