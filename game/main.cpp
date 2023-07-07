
#include <cstdio>
#include <cstdlib>
#include "SDL.h"
#include "SDL_opengl.h"

/*
typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

#define WinWidth 1000
#define WinHeight 1000
*/

int main(int argc, char* argv[])
{
    bool running = true;
    SDL_DisplayMode mode;

    //Init SDL video
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Video initialization failed: %s\n", SDL_GetError());
        return 1;
    }
 
    int number = SDL_GetNumDisplayModes(0);
    for (int i = 0; i < number; i++)
    {
        if (SDL_GetDisplayMode(0, i, &mode) == 0)
            break;
    }
    printf("Values: %d", mode.w);
    printf(" %d\n", mode.h);
    SDL_Window *window = SDL_CreateWindow("game", 0, 0, mode.w, mode.h, 
        SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);

    while (running == true)
    {
        SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_KEYDOWN)
            {
                switch(Event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = false;
                    break;
                }
            }
            if (Event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        glViewport(0, 0, mode.w, mode.h);
        glClearColor(1.f, 0.f, 1.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();
    return 0;
}