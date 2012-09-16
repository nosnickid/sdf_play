#ifndef SDF_CV_IMAGE_H__
#define SDF_CV_IMAGE_H__


#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "Sys.h"
#include "glext.h"
#include "AbstractDepthMap.h"
#include "AbstractRgbImage.h"
#include "Renderable.h"

class SdfCvImage: public virtual AbstractRgbImage, public virtual AbstractDepthMap {
public:

	GLuint rgbTexture;
	GLuint depthTexture;

	virtual void init();
	virtual void prepareFrame() = 0;

	virtual GLuint getRgbImageTexture();

	virtual GLuint getDepthMapTexture();

	void createTextureForRgb();
	void createTextureForDepth();

};

#endif
