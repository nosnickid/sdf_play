#ifndef SDF_CV_CAMERA_H__
#define SDF_CV_CAMERA_H__

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "Sys.h"
#include "Renderable.h"
#include "AbstractRgbImage.h"

class SdfCvCamera: public Renderable, public virtual AbstractRgbImage {
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
