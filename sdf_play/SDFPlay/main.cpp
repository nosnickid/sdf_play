#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "SDL.h"
#include "glsl.h"


SDL_Surface *screen;

int main(int argc, char *argv[])
{
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        return 0;
    }

	screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_OPENGL);
	
	glViewport(0, 0, 800, 600);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.2f, 0.2f, 0, 1);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 800, 0, 600, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);

	glBegin(GL_TRIANGLES);

	glColor3f(1, 0, 0); glVertex3f(10, 10, 0);
	glColor3f(0, 1, 0); glVertex3f(10, 100, 0);
	glColor3f(0, 0, 1); glVertex3f(100, 10, 0);

	glEnd();
	glFlush();

	init_glsl();

	GLhandleARB vertex = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	const GLcharARB *progVert = "varying vec4 vcol; void main() { gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; vcol = gl_Color; }" ;
	glShaderSourceARB(vertex, 1, &progVert, NULL);
	glCompileShaderARB(vertex);
	
	GLhandleARB frag = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	const GLcharARB *progFrag = "varying vec4 vcol; void main() { gl_FragColor = vec4(0.1, 0.5, 0.2, 1.0) * vcol; }";
	glShaderSourceARB(frag, 1, &progFrag, NULL);
	glCompileShaderARB(frag);

	GLhandleARB prog = glCreateProgramObjectARB();
	glAttachObjectARB(prog, vertex);
	glAttachObjectARB(prog, frag);
	
	glLinkProgramARB(prog);

	glUseProgramObjectARB(prog);

	glBegin(GL_TRIANGLES);

	glColor3f(0, 1, 0); glVertex3f(110, 110, 0);
	glColor3f(0, 0, 1); glVertex3f(110, 200, 0);
	glColor3f(1, 0, 0); glVertex3f(200, 110, 0);

	glEnd();
	glFlush();

    SDL_Event event;

	SDL_GL_SwapBuffers();


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
