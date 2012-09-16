#ifndef SDF_CV_CAMERA_H__
#define SDF_CV_CAMERA_H__

#include "Sys.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "SdfCvImage.h"

class SdfCvCamera: public virtual SdfCvImage {
protected:
	AbstractDepthMap *depthMap;
	GLuint frameTexture;

	void createTextureForFrame();
public:
	cv::VideoCapture videoCapture;
	cv::Mat vidFrame;

	SdfCvCamera(AbstractDepthMap *depthMap);
	~SdfCvCamera();
	void init();
	virtual void prepareFrame();
	void loadTextureFromIpl();

	virtual GLuint getRgbImageTexture();

	virtual GLuint getDepthMapTexture();
	
};

#endif
