#ifndef SDF_CV_CAMERA_H__
#define SDF_CV_CAMERA_H__

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"


class SdfCvCamera {
protected:
	void loadTextureFromIpl();	
public:
	CvCapture* camera;
	IplImage* frame;
	GLuint frameTexture;

	SdfCvCamera();
	~SdfCvCamera();
	void captureFrame();
	void createTextureForFrame();
	
};

#endif
