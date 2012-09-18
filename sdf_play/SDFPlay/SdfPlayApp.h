#ifndef SDF_PLAY_APP_H__
#define SDF_PLAY_APP_H__

#include "Sys.h"
#include "glsl.h"
#include "Spotlight.h"
#include "glframebufferext.h"
#include "oglconsole.h"
#include "FpsCamera.h"
#include "Debug.h"
#include "teapot.h"
#include "DepthCameraRenderer.h"
#include "QrCodeParser.h"
#include "Gl2dRender.h"

#include "opencv/cv.h"
#include "opencv/highgui.h"

class SdfPlayApp {
protected:
	void initSdlGl(void);
	void initConsole(void);
	void initGlSlPrograms(void);
	void initSpotlight(void);
	void initDepthCamera(void);

	void render();
	void drawScene();

	bool done;
	bool doQrParse;

	Gl2dRender *gl2d;

public:
	SDL_Surface *screen;
	OGLCONSOLE_Console console;

	GLhandleARB prog;

	Spotlight* spotlight;
	FpsCamera viewCam;
	FpsCamera *activeCam;
	DepthCameraRenderer *dc;
	QrCodeParser *qrCodeParser;

	void init(void);
	void run(void);

	void consoleCallback(OGLCONSOLE_Console console, const char *cmd);
};


#endif