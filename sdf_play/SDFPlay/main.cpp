#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "SDL.h"
#include "glsl.h"
#include "oglconsole.h"

SDL_Surface *screen;

OGLCONSOLE_Console console;

GLhandleARB prog;

int done = 0;


void drawScene()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgramObjectARB(0);

	glBegin(GL_TRIANGLES);

	glColor3f(1, 0, 0); glVertex3f(10, 10, 0);
	glColor3f(0, 1, 0); glVertex3f(10, 100, 0);
	glColor3f(0, 0, 1); glVertex3f(100, 10, 0);

	glEnd();
	glFlush();

	glUseProgramObjectARB(prog);

	glBegin(GL_TRIANGLES);

	glColor3f(1, 1, 0); glVertex3f(110, 110, 0);
	glColor3f(1, 0, 1); glVertex3f(110, 200, 0);
	glColor3f(0, 1, 1); glVertex3f(200, 110, 0);

	glEnd();
}

void drawConsole()
{
	glUseProgramObjectARB(0);
	OGLCONSOLE_Draw();
}

void cmdCb(OGLCONSOLE_Console console, char *cmd)
{
    if (!strncmp(cmd, "quit", 4)) done = TRUE;

    /*else if (!strncmp(cmd, "add", 3))
    {
        int a, b;
        if (sscanf(cmd, "add %i %i", &a, &b) == 2)
        {
            OGLCONSOLE_Output(console, "%i + %i = %i\n", a, b, a+b);
            return;
        }

        OGLCONSOLE_Output(console, "usage: add INT INT\n");
        return;
    }*/
    
    OGLCONSOLE_Output(console, "I don't understand this command: %s\n", cmd);
}

int main(int argc, char *argv[])
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        return 0;
    }

	screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_OPENGL);
	
	glViewport(0, 0, 800, 600);
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 1);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);


	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 800, 0, 600, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	console = OGLCONSOLE_Create();
	OGLCONSOLE_EnterKey(cmdCb);
	OGLCONSOLE_Print("SDF PLAY V0.1 (LOL)");

	init_glsl();

	const GLcharARB *progVert = "varying vec4 vcol; void main() { gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; vcol = gl_Color; }" ;
	const GLcharARB *progFrag = "varying vec4 vcol; void main() { gl_FragColor = vcol * vec4(0.5,0.5,0.5,1.0); }";
	prog = createShaderFromProgs(progVert, progFrag);


    SDL_Event event;


	atexit(SDL_Quit);

	while ( !done ) {
		while ( SDL_PollEvent(&event) ) {
			if (OGLCONSOLE_SDLEvent(&event) == 0) 
			{
				switch( event.type ) 
				{
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
						if (event.key.keysym.sym == SDLK_BACKQUOTE) 
						{
							OGLCONSOLE_SetVisibility(1);
						}

						if (event.key.keysym.sym == SDLK_ESCAPE) 
						{
							SDL_Event qe;
							qe.quit.type = SDL_QUIT;
							SDL_PushEvent(&qe);
						}
						break;
					case SDL_QUIT:
						done = 1;
						break;
				}
			}
		}
		drawScene();
		drawConsole();
		SDL_GL_SwapBuffers();
	}

	OGLCONSOLE_Destroy(console);
	SDL_Quit();
	    

	return 0;   
}
