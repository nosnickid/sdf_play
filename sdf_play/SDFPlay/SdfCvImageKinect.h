#ifndef SDF_CV_IMAGE_KINECT_H__
#define SDF_CV_IMAGE_KINECT_H__

#include "KinectToOpenCV.h"
#include "SdfCvImage.h"
#include "Sys.h"
#include "glsl.h"


class SdfCvImageKinect: public SdfCvImage {
protected:
	KinectToOpenCV *kinToCv;

public:
	void init();
	void prepareFrame();

	void loadRgbImage();
	void loadDepthMap();

};


#endif