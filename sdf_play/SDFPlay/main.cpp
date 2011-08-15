#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "SDL.h"
#include "glsl.h"
#include "glframebufferext.h"
#include "oglconsole.h"
#include "FpsCamera.h"

#define SDFPLAY_VERSION "0.2-glfbext"

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

GLenum frameBuffer;
GLuint textureId;
GLenum depthBuffer;

void initGraphicsShit() 
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	// We want a temp depth buffer please.
	glGenRenderbuffersEXT(1, &depthBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, 1024, 1024);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	// Via this.
	glGenFramebuffersEXT(1, &frameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureId, 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) OGLCONSOLE_Print("glCheckFramebufferStatusEXT failed!");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}

void drawTriangles() 
{
	glUseProgramObjectARB(0);

	glBegin(GL_TRIANGLES);

	for(int i = 0; i < 10; i++) 
		for(int j = 0; j < 10; j++) 
		{
			glColor3f(1.f, 0.f, 0.f); glVertex3f(i*10.f,         j*10.f,        0.f);
			glColor3f(0.f, 1.f, 0.f); glVertex3f(i*10.f,         j*10.f + 10.f, 0.f);
			glColor3f(0.f, 0.f, 1.f); glVertex3f(i*10.f + 10.f, j*10.f,         0.f);
			
			glColor3f(0.f, 1.f, 0.f); glVertex3f(i*10.f,         j*10.f + 10.f, 0.f);
			glColor3f(1.f, 1.f, 1.f); glVertex3f(i*10.f + 10.f,  j*10.f + 10.f, 0.f);
			glColor3f(0.f, 0.f, 1.f); glVertex3f(i*10.f + 10.f, j*10.f, 0.f);
		}

	glEnd();

	glUseProgramObjectARB(prog);

	glBegin(GL_TRIANGLES);

	glColor3f(1, 1, 0); glVertex3f(110, 110, 0);
	glColor3f(1, 0, 1); glVertex3f(110, 200, 0);
	glColor3f(0, 1, 1); glVertex3f(200, 110, 0);

	glEnd();

	glUseProgramObjectARB(0);
}

void drawScene()
{
	// Attach our background framebuffer and render into it.

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glViewport(0, 0, 1024,1024);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, 0, 1);
    
	glMatrixMode(GL_MODELVIEW);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);

	// lol cornflower blue.
	glClearColor(100/256.f,149/256.f,237/256.f, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawTriangles();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 800.0/600.0, 1, 10000);


	glClearColor(0,0,0,0);

	cam.GlMult();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glGenerateMipmapEXT(GL_TEXTURE_2D);

	glColor4d(1,1,1,1);

	glBegin(GL_TRIANGLES);
	glTexCoord2d(0, 0); glVertex3f(0.f,   0.f, -1.f); 
	glTexCoord2d(1, 0); glVertex3f(800.f, 0.f, -1.f); 
	glTexCoord2d(0, 1); glVertex3f(0.f,   600.f, -1.f); 

	glTexCoord2d(1, 0); glVertex3f(800.f, 0.f, -1.f); 
	glTexCoord2d(0, 1); glVertex3f(0.f,   600.f, -1.f); 
	glTexCoord2d(1, 1); glVertex3f(800.f, 600.f, -1.f);

	glEnd();

	glFlush();

	glDisable(GL_TEXTURE_2D);


}

void drawConsole()
{
	glUseProgramObjectARB(0);
	OGLCONSOLE_Draw();
}

void cmdCb(OGLCONSOLE_Console console, char *cmd)
{
    if (!strncmp(cmd, "quit", 4)) 
	{
		done = TRUE;
		return;
	}

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
    
    OGLCONSOLE_Output(console, "\"%s\" bad command\n", cmd);
}

int main(int argc, char *argv[])
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		fprintf(stderr, "Also some extra shit: %d %s", argc, argv[0]);
        return 0;
    }

	screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_OPENGL);
	
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 1);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// hax
	cam.InitHax(92, 100, 170, 0, 266);

	console = OGLCONSOLE_Create();
	OGLCONSOLE_EnterKey(cmdCb);
	OGLCONSOLE_Print("SDF PLAY V%s\n", SDFPLAY_VERSION);

	glslSetErrorHandler(&theGlSlErrorHandler);
	init_glsl();
	init_glframebufferext();


	initGraphicsShit();

	

	const GLcharARB *progVert = "varying vec4 vcol; varying vec4 sinoffs; void main() { gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; vcol = gl_Color; }" ;
	const GLcharARB *progFrag = "varying vec4 vcol; varying vec4 sinoffs; void main() { gl_FragColor = vcol * vec4(0.5,0.5,0.5,1.0); }";
	prog = createShaderFromProgs(progVert, progFrag);

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
