#ifndef SDF_PLAY_TEAPOT_SHADOW_SCENE_H__
#define SDF_PLAY_TEAPOT_SHADOW_SCENE_H__

#include "Sys.h"
#include "glsl.h"
#include "Spotlight.h"

class TeapotShadowScene {
protected:
	GLhandleARB shadowProg;

public:
	TeapotShadowScene();

	void render(bool islit, Spotlight *spotlight);
};

#endif