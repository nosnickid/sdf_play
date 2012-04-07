#ifndef SDF_CV_CAMERA_H__
#define SDF_CV_CAMERA_H__

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"
#include "Renderable.h"

using namespace sdf_play::render;

class SdfCvCamera: public Renderable {
protected:
	void loadTextureFromIpl();	
public:
	CvCapture* camera;
	IplImage* frame;
	GLuint frameTexture;
	GLhandleARB textureProg;

	SdfCvCamera();
	~SdfCvCamera();
	void init();
	virtual void prepareFrame();
	virtual void render();
	void createTextureForFrame();
	
};

#endif
