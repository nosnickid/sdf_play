#ifndef DEPTH_CAMERA_RENDERER_H__
#define DEPTH_CAMERA_RENDERER_H__

#include "AbstractDepthMap.h"
#include "AbstractRgbImage.h"
#include "SdfCvImage.h"
#include "SdfCvCamera.h"
#include "SdfCvImageKinect.h"
#include "Renderable.h"

class DepthCameraRenderer: public Renderable {
public:
	GLhandleARB depthProg;

	SdfCvImage* image;
	AbstractRgbImage* rgb;
	AbstractDepthMap* depth;

	DepthCameraRenderer();
	~DepthCameraRenderer();
	virtual void init();
	virtual void prepareFrame();
	virtual void render();
};

#endif