#ifndef SDF_PLAY_SPOTLIGHT_H__
#define SDF_PLAY_SPOTLIGHT_H__

#include "Sys.h"
#include "FpsCamera.h"
#include "glframebufferext.h"


/**
 * Some shit facts:
 * (a) we've got 800x600 hardcoded as the gluPerspective call
 */

class Spotlight {
protected:
	GLfloat *lightMatrix;
public:
	GLuint frameBuffer;
	GLuint depthBuffer;
	FpsCamera position;
	GLfloat *projMatrix;
	GLfloat *mviewMatrix;
	
	Spotlight();
	~Spotlight();

	void RenderDone();
	void PrepareRender();

	GLfloat* GetLightMatrix();
	
};

#endif