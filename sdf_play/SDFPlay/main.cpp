#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "SDL.h"
#include "glsl.h"
#include "oglconsole.h"
#include "FpsCamera.h"
#include "Debug.h"

#define SDFPLAY_VERSION "0.1 HEHE"

SDL_Surface *screen;

OGLCONSOLE_Console console;

GLhandleARB prog;

FpsCamera cam;

int done = 0;

void APIENTRY theGlSlErrorHandler(GLcharARB *msg) 
{
	OGLCONSOLE_Print(msg);
	OGLCONSOLE_SetVisibility(1);
	
}


void drawScene()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cam.GlMult();

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
    
    OGLCONSOLE_Output(console, "\"%s\" bad command\n", cmd);
}

int main(int argc, char *argv[])
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        fatal("Unable to init SDL: %s\n", SDL_GetError());
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
	//glOrtho(0, 800, 0, 600, 0, 1);
	gluPerspective(90, 800.0/600.0, 1, 10000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// hax
	cam.InitHax(92, 100, 170, 0, 266);

	console = OGLCONSOLE_Create();
	OGLCONSOLE_EnterKey(cmdCb);
	OGLCONSOLE_Print("SDF PLAY V%s\n", SDFPLAY_VERSION);

	init_glsl();
	glslSetErrorHandler(&theGlSlErrorHandler);

	const GLcharARB *progVert = "varying vec4 vcol; varying vec4 sinoffs; void main() { gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; vcol = gl_Color; }" ;
	const GLcharARB *progFrag = "varying vec4 vcol; varying vec4 sinoffs; void main() { gl_FragColor = vcol * vec4(0.5,0.5,0.5,1.0); }";
	prog = createShaderFromProgs(progVert, progFrag);
	
	debug("Welcoming you to the new logging world");

    SDL_Event event;

	atexit(SDL_Quit);

	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(0);

	Uint32 time = SDL_GetTicks();
	Uint32 last = time;

	GLfloat xvel = 0, yvel = 0;

	while ( !done ) {
		while ( SDL_PollEvent(&event) ) {
			if (OGLCONSOLE_SDLEvent(&event) == 0) 
			{
				switch( event.type ) 
				{
					case SDL_MOUSEMOTION:
						cam.RotateBy(event.motion.xrel, event.motion.yrel);
						break;
					case SDL_MOUSEBUTTONDOWN:
						printf("Mouse button %d pressed at (%d,%d)\n",
							   event.button.button, event.button.x, event.button.y);
						break;
					case SDL_KEYDOWN:
						switch(event.key.keysym.sym) 
						{
						case SDLK_ESCAPE:
							SDL_Event qe;
							cam.InitHax(1,1,1,1,1);
							qe.quit.type = SDL_QUIT;
							SDL_PushEvent(&qe);
							break;
						case SDLK_h:  yvel = -50; break;
						case SDLK_k:  yvel = 50; break;
						case SDLK_u:  xvel = 50; break;
						case SDLK_j:  xvel = -50; break;
						default:
							// ignore
							break;
						}
						break;
					case SDL_KEYUP:
						switch(event.key.keysym.sym)
						{
						case SDLK_h:  yvel = 0; break;
						case SDLK_k:  yvel = 0; break;
						case SDLK_u:  xvel = 0; break;
						case SDLK_j:  xvel = 0; break;
						default: // not bovvvered. 
							break;
						}
						break;
					case SDL_QUIT:
						done = 1;
						break;
					default:
						break;
				}
			}
		}
		time = SDL_GetTicks();
		if (time - last > 100) 
		{
			last = time;
			cam.MoveOnRelXY(xvel / 10.0f, yvel / 10.0f);
		}
		drawScene();
		drawConsole();
		SDL_GL_SwapBuffers();
	}

	OGLCONSOLE_Destroy(console);
	SDL_Quit();
	    

	return 0;   
}
