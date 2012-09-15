#ifndef SDF_PLAY_TEXTURE_DEBUG_RENDERER_H__
#define SDF_PLAY_TEXTURE_DEBUG_RENDERER_H__

#include "Sys.h"

/**
 * Draws a quad with the specified texture on it, at the 2d coords specified.
 *
 * Pretty useful for debugging if textures are playing up.
 */
class TextureDebugRenderer {
protected:
	GLhandleARB textureProg;

public:
	TextureDebugRenderer();
	void render(GLuint texture, int x, int y, int w, int h);
};


#endif