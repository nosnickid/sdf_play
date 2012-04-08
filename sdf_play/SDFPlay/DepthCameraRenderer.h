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
	GLint glslDepthTexture;
	GLint glslColorTexture;
	GLint glslSurfaceNormal;

	SdfCvImageKinect *kin;
	AbstractRgbImage *rgb;
	AbstractDepthMap *depth;

	~DepthCameraRenderer();
	virtual void init();
	virtual void prepareFrame();
	virtual void render();
};

#endif