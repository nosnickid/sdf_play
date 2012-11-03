#include "SdfPlayApp.h"
#include "RWText.h"

#define SDFPLAY_VERSION "0.5-opencv-objectsyeah"

static void APIENTRY theGlSlErrorHandler(const GLcharARB *program, const GLcharARB *msg) 
{
	fatal("glsl error: %s\nin: %s", program, msg);
}

static SdfPlayApp *consoleApp = NULL;

static void cmdCb(OGLCONSOLE_Console console, char *cmd)
{
	if (consoleApp != NULL) {
		consoleApp->consoleCallback(console, cmd);
	}
}

void SdfPlayApp::consoleCallback(OGLCONSOLE_Console console, const char *cmd) {
	if (!strncmp(cmd, "quit", 4)) {
		this->done = true; 
	} else if (!strncmp(cmd, "camspam", 7)) {
		OGLCONSOLE_Output(console, "camspam: %s\n", activeCam->spam().c_str());
	} else if (!strncmp(cmd, "tone", 4)) {
		// OGLCONSOLE_Output(console, "camspam: %s\n", activeCam->spam().c_str());
		char input[50];
		if (sscanf(cmd, "tone %c", &input) == 1) {
			switch(input[0]) {
			case '#':
				this->dtmf.playTone(DtmfAudioSource::KEY_HASH);
				break;
			case '*':
				this->dtmf.playTone(DtmfAudioSource::KEY_ASTERISK);
				break;
			case '0':
				this->dtmf.playTone(DtmfAudioSource::KEY_0);
				break;
			case '1':
				this->dtmf.playTone(DtmfAudioSource::KEY_1);
				break;
			case '2':
				this->dtmf.playTone(DtmfAudioSource::KEY_2);
				break;
			case '3':
				this->dtmf.playTone(DtmfAudioSource::KEY_3);
				break;
			case '4':
				this->dtmf.playTone(DtmfAudioSource::KEY_4);
				break;
			case '5':
				this->dtmf.playTone(DtmfAudioSource::KEY_5);
				break;
			case '6':
				this->dtmf.playTone(DtmfAudioSource::KEY_6);
				break;
			case '7':
				this->dtmf.playTone(DtmfAudioSource::KEY_7);
				break;
			case '8':
				this->dtmf.playTone(DtmfAudioSource::KEY_8);
				break;
			case '9':
				this->dtmf.playTone(DtmfAudioSource::KEY_9);
				break;
			case 'a':
			case 'A':
				this->dtmf.playTone(DtmfAudioSource::KEY_A);
				break;
			case 'b':
			case 'B':
				this->dtmf.playTone(DtmfAudioSource::KEY_B);
				break;
			case 'c':
			case 'C':
				this->dtmf.playTone(DtmfAudioSource::KEY_C);
				break;
			case 'd':
			case 'D':
				this->dtmf.playTone(DtmfAudioSource::KEY_D);
				break;
			default:
				this->dtmf.playTone(0);
				info("usage: tone [0-9a-d*#]");
			}
		} else {
			this->dtmf.playTone(0);
		}
	} else {
		OGLCONSOLE_Output(console, "\"%s\" bad command\n", cmd);
	}
}

void SdfPlayApp::initSdlGl(void) {
	if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
		fatal("Unable to init SDL: %s\n", SDL_GetError());
		return;
	}

	this->screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_OPENGL);

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

	glslSetErrorHandler(&theGlSlErrorHandler);
	init_glsl();
	init_glframebufferext();

	checkOpenGL("bindings");
}

void SdfPlayApp::initGlSlPrograms(void) {

	const GLcharARB *progVert = "varying vec4 vcol; varying vec4 sinoffs; void main() { gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; vcol = gl_Color; }" ;
	const GLcharARB *progFrag = "varying vec4 vcol; varying vec4 sinoffs; void main() { gl_FragColor = vcol * vec4(0.5,0.5,0.5,1.0); }";
	this->prog = createShaderFromProgs(progVert, progFrag);
	checkOpenGL("shitty prog");

	glUseProgramObjectARB(0);
}

void SdfPlayApp::initConsole(void) {
	this->console = OGLCONSOLE_Create();
	OGLCONSOLE_EnterKey(cmdCb);
	OGLCONSOLE_Print("SDF PLAY V%s\n", SDFPLAY_VERSION);
	consoleApp = this;
}

void SdfPlayApp::initSpotlight(void) {
	// hax
	this->viewCam.InitHax(92, 100, 170, 0, 266);
	this->activeCam = &viewCam;

	this->spotlight = new Spotlight();
	this->spotlight->position.InitHax(75, 70, 125, -7, 261);
}

void SdfPlayApp::initDepthCamera(void) {
	this->dc = new DepthCameraRenderer();
	this->dc->init();
}

void SdfPlayApp::init(void) {
	this->initSdlGl();
	this->initConsole();
	this->initGlSlPrograms();
	this->initSpotlight();
	this->initDepthCamera();

	this->qrCodeParser = new QrCodeParser();
	this->gl2d = new Gl2dRender();
	this->audio = new SdfPlaySdlAudio();
	this->audio->init();

	this->audio->registerAudioSource(&this->dtmf);
	//this->dtmf.playTone(DtmfAudioSource::KEY_A);

	rwtext.init();
}

SdfPlayApp::~SdfPlayApp(void) {
	delete this->qrCodeParser;
	delete this->spotlight;
	if (this->dc) delete this->dc;
	consoleApp = NULL;
	OGLCONSOLE_Destroy(this->console);
	SDL_Quit();
}

void SdfPlayApp::run(void) {
	atexit(SDL_Quit);

	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(0);

	Uint32 time = SDL_GetTicks();
	Uint32 last = time;

	cameraVelocity[0] = cameraVelocity[1] = 0;

	debug("SdfPlayApp::run");
	
	this->done = false;

   
	while ( !this->done ) {
		this->handleSdlEventLoop();
		time = SDL_GetTicks();
		if (time - last > 50) 
		{
			last = time;
			activeCam->MoveOnRelXY(cameraVelocity[0] / 15.0f, cameraVelocity[1] / 15.0f);
		}

		this->fps.tickFrame();

		render();
		SDL_GL_SwapBuffers();
	}

}

void SdfPlayApp::handleSdlEventLoop(void) {
	SDL_Event event;

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
						qe.quit.type = SDL_QUIT;
						SDL_PushEvent(&qe);
						break;
					case SDLK_h:  cameraVelocity[1] = -50; break;
					case SDLK_k:  cameraVelocity[1] = 50; break;
					case SDLK_u:  cameraVelocity[0] = 50; break;
					case SDLK_j:  cameraVelocity[0] = -50; break;
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
					case SDLK_h:  cameraVelocity[1] = 0; break;
					case SDLK_k:  cameraVelocity[1] = 0; break;
					case SDLK_u:  cameraVelocity[0] = 0; break;
					case SDLK_j:  cameraVelocity[0] = 0; break;
					default: // not bovvvered. 
						break;
					}
					break;
				case SDL_QUIT:
					this->done = true;
					break;
				default:
					break;
			}
		}
	}
}

void SdfPlayApp::render(void) {
	this->drawScene();

	// Draw console.
	glUseProgramObjectARB(0);
	OGLCONSOLE_Draw();
}

void SdfPlayApp::drawScene()
{
	// render the light POV(s?)
	/*spotlight->PrepareRender();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawTriangles(false);
	spotlight->RenderDone();

    checkOpenGL("render shadow depth");
	*/
	dc->prepareFrame();

	// int previewSize = 250;
	// this->gl2d->drawTexturePreview(dc->depth->getDepthMapTexture(),  0,  600 - previewSize, previewSize, previewSize, winding_mathsy);
	// this->gl2d->drawTexturePreview(spotlight->depthBuffer,          previewSize, 600 - previewSize, previewSize, previewSize);
	// this->gl2d->drawTexturePreview(dc->rgb->getRgbImageTexture(),    previewSize * 2, 600 - previewSize, previewSize, previewSize, winding_memory);
	this->gl2d->drawTexturePreview(dc->rgb->getRgbImageTexture(),    0, 600 - 240, 320, 240, winding_memory);

	this->gl2d->render();

	rwtext.render();

	checkOpenGL("render depth preview");

	// and render the scene as we actually see it.

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 800.0/600.0, 1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	viewCam.GlMult();

	//drawTriangles(true);

	// textureDebug->render(dc->rgb->getRgbImageTexture(),             previewSize * 2, 600 - previewSize, previewSize, previewSize);

	// this->qrCodeParser->parseOpenCvImage(dynamic_cast<SdfCvCamera*>(dc->image)->frame);

	checkOpenGL("render lighting scene");

	dc->render();

	checkOpenGL("render depth cam scene");
}
