#include "DepthCameraRenderer.h"
#include "ManualDepthMap.h"
#include "glsl.h"

using namespace sdf_play::render;

DepthCameraRenderer::~DepthCameraRenderer() {
	delete this->cam;
	delete this->depth;
}

void DepthCameraRenderer::init() {
	this->cam = new SdfCvCamera();
	this->cam->init();
	this->depth = new ManualDepthMap();
	this->depth->loadDepthMap();

	// depthProg = createShaderFromProgs(vertProg, fragProg);
}

void DepthCameraRenderer::prepareFrame() {
	this->cam->prepareFrame();
}

void DepthCameraRenderer::render() {
	this->cam->render();
}
