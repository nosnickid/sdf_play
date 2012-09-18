#ifndef SDF_PLAY_GL2D_RENDER_H__
#define SDF_PLAY_GL2D_RENDER_H__

#include "TextureDebugRenderer.h"

#include <vector>

typedef enum { 
	winding_mathsy, 
	winding_memory 
} texture_winding;

class Gl2dRender {
protected:
	typedef struct {
		GLuint texture;
		int x, y, w, h;
		texture_winding winding;
	} texture_preview;

	std::vector<Gl2dRender::texture_preview> texturePreviews;

	TextureDebugRenderer *textureDebug;

public:
	Gl2dRender();

	void render();

	void drawTexturePreview(GLuint texture, int x, int y, int w, int h, texture_winding winding);
};

#endif