#include <stdlib.h>
#include "SDL.h"

SDL_Surface *screen;

int main(int argc, char *argv[])
{
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        return 0;
    }

	screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_OPENGL);

    SDL_Event event;

    while ( SDL_WaitEvent(&event) ) {
        switch (event.type) {
            case SDL_MOUSEMOTION:
                printf("Mouse moved by %d,%d to (%d,%d)\n", 
                       event.motion.xrel, event.motion.yrel,
                       event.motion.x, event.motion.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                printf("Mouse button %d pressed at (%d,%d)\n",
                       event.button.button, event.button.x, event.button.y);
                break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					SDL_Event qe;
					qe.quit.type = SDL_QUIT;
					SDL_PushEvent(&qe);
				}
				break;
            case SDL_QUIT:
                exit(0);
        }
    }
    
    
	
	atexit(SDL_Quit);

	return 0;   
}