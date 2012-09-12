#ifndef SDF_CV_CAMERA_H__
#define SDF_CV_CAMERA_H__

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "Sys.h"
#include "SdfCvImage.h"

class SdfCvCamera: public virtual SdfCvImage {
protected:
	AbstractDepthMap *depthMap;
	GLuint frameTexture;

	void loadTextureFromIpl();
public:
	CvCapture* camera;
	IplImage* frame;
	GLhandleARB textureProg;

	SdfCvCamera(AbstractDepthMap *depthMap);
	~SdfCvCamera();
	void init();
	virtual void prepareFrame();
	virtual void render();
	void createTextureForFrame();

	virtual GLuint getRgbImageTexture();

	virtual GLuint getDepthMapTexture();
	
};

#endif
