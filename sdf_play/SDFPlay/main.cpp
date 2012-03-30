#include <stdlib.h>
#include "Sys.h"
#include "glsl.h"
#include "Spotlight.h"
#include "glframebufferext.h"
#include "oglconsole.h"
#include "FpsCamera.h"
#include "Debug.h"
#include "teapot.h"

#pragma warning(disable:4996)


#include "opencv\cv.h"
#include "opencv\highgui.h"

#define SDFPLAY_VERSION "0.3-glfbext"

SDL_Surface *screen;

OGLCONSOLE_Console console;

GLhandleARB prog;
GLhandleARB textureProg;
GLhandleARB shadowProg;


Spotlight *spotlight;
FpsCamera viewCam;
FpsCamera *activeCam = &viewCam;

int done = 0;

void APIENTRY theGlSlErrorHandler(GLcharARB *msg) 
{
	fatal(msg);
}

void scaleAndOffs(float scale, float x, float y, float z) 
{
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glScalef(scale, scale, scale);
	glTranslatef(x, y, z);
	glActiveTextureARB(GL_TEXTURE0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScalef(scale, scale, scale);
	glTranslatef(x, y, z);
}

void unscale() 
{
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPopMatrix();
	glActiveTextureARB(GL_TEXTURE0);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}



GLfloat bias[] = { 0.5f, 0, 0, 0,     0, 0.5f, 0, 0,    0, 0, 0.5f, 0,     0.5f, 0.5f, 0.5f, 1 };

void drawTriangles(bool isLit)
{
	if (isLit) {
		glUseProgramObjectARB(shadowProg);
		glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		glLoadIdentity();
		glMultMatrixf(bias);		
		glMultMatrixf(spotlight->projMatrix);
		glMultMatrixf(spotlight->mviewMatrix);

		glActiveTextureARB(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, spotlight->depthBuffer);
	} else {
		glUseProgramObjectARB(0);
	}

	glMatrixMode(GL_MODELVIEW);

	glColor4f(1.f, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.f,   0.f, -0.f);    
	glVertex3f(100.f, 0.f, 0.f);     
	glVertex3f(0.f,   100.f, -0.f); 
	
	glVertex3f(100.f, 0.f, -0.f);    
	glVertex3f(0.f,   100.f, 0.f);   
	glVertex3f(100.f, 100.f, -0.f);

	glEnd();

	glBegin(GL_TRIANGLES);

	glColor3f(1, 1, 0); glVertex3f(110, 110, 0);
	glColor3f(1, 0, 1); glVertex3f(110, 200, 0);
	glColor3f(0, 1, 1); glVertex3f(200, 110, 0);

	glEnd();

	for(int i = 0; i < 5; i++) 
	{
		for(int j = 0; j < 5; j++) 
		{
			scaleAndOffs(1.5f, 25 + i * 5.f, 25 + j * 5.f, 50);
			drawTeapot();
			unscale();
		}
	}
}

void drawScene()
{
	// render the light POV(s?)
	spotlight->PrepareRender();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawTriangles(false);
	spotlight->RenderDone();

    checkOpenGL("render shadow depth");

	// draw the straight light texture ortho so we can easily debug it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 600);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgramObjectARB(textureProg);
	glBindTexture(GL_TEXTURE_2D, spotlight->depthBuffer);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2d(0, 0);
	glTexCoord2d(1, 0); glVertex2d(300, 0);
	glTexCoord2d(1, 1); glVertex2d(300, 200);
	glTexCoord2d(0, 1); glVertex2d(0, 200);
	glEnd();

    checkOpenGL("render depth preview");

	// and render the scene as we actually see it.

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 800.0/600.0, 1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	viewCam.GlMult();

	drawTriangles(true);

    checkOpenGL("render scene nicely");

}

void drawConsole()
{
	glUseProgramObjectARB(0);
	OGLCONSOLE_Draw();
}


void render()
{
	drawScene();
	drawConsole();
}

void cmdCb(OGLCONSOLE_Console console, char *cmd)
{
    if (!strncmp(cmd, "quit", 4)) 
	{
		done = true; 
		return;
	}

	if (!strncmp(cmd, "camspam", 7))
	{
		OGLCONSOLE_Output(console, "camspam: %s\n", activeCam->spam().c_str());
		return;
	}

	if (!strncmp(cmd,"cap",3)) 
	{
		CvCapture* cap = cvCaptureFromCAM( CV_CAP_ANY );
		if (!cap) fatal("Cap didn't work");
		return;
	}

    OGLCONSOLE_Output(console, "\"%s\" bad command\n", cmd);
}

int main(int argc, char *argv[])
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        fatal("Unable to init SDL: %s %d %d\n", SDL_GetError(), argc, argv);
        return 0;
    }

	screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_OPENGL);

	glViewport(0, 0, 800, 600);
	glClearColor(0,0,0,0);
	
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 1);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    checkOpenGL("init");

	console = OGLCONSOLE_Create();
	OGLCONSOLE_EnterKey(cmdCb);
	OGLCONSOLE_Print("SDF PLAY V%s\n", SDFPLAY_VERSION);

	glslSetErrorHandler(&theGlSlErrorHandler);
	init_glsl();
	init_glframebufferext();

    checkOpenGL("bindings");

	const GLcharARB *progVert = "varying vec4 vcol; varying vec4 sinoffs; void main() { gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; vcol = gl_Color; }" ;
	const GLcharARB *progFrag = "varying vec4 vcol; varying vec4 sinoffs; void main() { gl_FragColor = vcol * vec4(0.5,0.5,0.5,1.0); }";
	prog = createShaderFromProgs(progVert, progFrag);
	checkOpenGL("shitty prog");

	const GLcharARB *tprogVert = "varying vec2 texcoord; void main() { gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; texcoord = vec2(gl_MultiTexCoord0); }" ;
	const GLcharARB *tprogFrag = "varying vec2 texcoord; uniform sampler2D texture; void main() { gl_FragColor = vec4(texture2D(texture, texcoord)[0]); }";
	textureProg = createShaderFromProgs(tprogVert, tprogFrag);
	checkOpenGL("texture prog");

	const GLcharARB *sprogVert = "\
		varying vec4 vertCol; varying vec4 lightTexPos; \
		void main() { \
			gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; \
			lightTexPos = gl_TextureMatrix[7] * gl_Vertex; \
			lightTexPos = lightTexPos / lightTexPos[3]; \
			vertCol = gl_Color; \
		}";
	const GLcharARB *sprogFrag = "\
		 varying vec4 vertCol; varying vec4 lightTexPos; uniform sampler2D depthTexture;  \
		 void main() { \
		     vec4 lightFactor; \
			 gl_FragColor = vertCol * 0.1;  \
			 if ((lightTexPos[0] >= 0) && (lightTexPos[0] <= 1) && (lightTexPos[1] >= 0) && (lightTexPos[1] <= 1)) { \
			 lightFactor = texture2D(depthTexture, vec2(lightTexPos));\
				gl_FragColor = gl_FragColor + vertCol * lightFactor; \
			 } \
		 }";

    //const GLcharARB *sprogFrag = "varying vec4 vertCol; varying vec4 lightTexPos; void main() { gl_FragColor = vertCol; } ";

	shadowProg = createShaderFromProgs(sprogVert, sprogFrag);
    checkOpenGL("shadow prog");

	glUseProgramObjectARB(0);


	// hax
	viewCam.InitHax(92, 100, 170, 0, 266);

	spotlight = new Spotlight();
	spotlight->position.InitHax(75, 70, 125, -7, 261);
	
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
						activeCam->RotateBy(event.motion.xrel, event.motion.yrel);
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
							viewCam.InitHax(1,1,1,1,1);
							qe.quit.type = SDL_QUIT;
							SDL_PushEvent(&qe);
							break;
						case SDLK_h:  yvel = -50; break;
						case SDLK_k:  yvel = 50; break;
						case SDLK_u:  xvel = 50; break;
						case SDLK_j:  xvel = -50; break;
                        case SDLK_y:  if (activeCam == &viewCam) { activeCam = &spotlight->position; } else { activeCam = &viewCam; } 
                            break;
                        case SDLK_i: OGLCONSOLE_SetVisibility(1); break;
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
			activeCam->MoveOnRelXY(xvel / 10.0f, yvel / 10.0f);
		}

		render();
		SDL_GL_SwapBuffers();
	}

	OGLCONSOLE_Destroy(console);
	SDL_Quit();
	    

	return 0;   
}
