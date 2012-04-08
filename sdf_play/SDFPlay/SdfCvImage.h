#ifndef SDF_CV_IMAGE_H__
#define SDF_CV_IMAGE_H__


#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "Sys.h"
#include "glext.h"
#include "AbstractRgbImage.h"
#include "Renderable.h"

class SdfCvImage: public virtual Renderable, public virtual AbstractRgbImage {
public:

	GLuint frameTexture;

	virtual void init();
	virtual void prepareFrame() = 0;
	virtual void render();

	virtual void loadRgbImage() = 0;
	virtual GLuint getRgbImageTexture();

	void createTextureForFrame();

};

#endif
