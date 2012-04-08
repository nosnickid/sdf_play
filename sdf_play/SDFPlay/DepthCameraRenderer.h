#ifndef DEPTH_CAMERA_RENDERER_H__
#define DEPTH_CAMERA_RENDERER_H__

#include "AbstractDepthMap.h"
#include "SdfCvCamera.h"
#include "Renderable.h"

namespace sdf_play { namespace render {

	class DepthCameraRenderer: public Renderable {
	public:
		GLhandleARB depthProg;
		GLint glslDepthTexture;
		GLint glslColorTexture;
		GLint glslSurfaceNormal;

		SdfCvCamera *cam;
		AbstractDepthMap *depth;

		~DepthCameraRenderer();
		virtual void init();
		virtual void prepareFrame();
		virtual void render();
	};

}}


#endif
