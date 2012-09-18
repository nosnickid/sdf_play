#include "Gl2dRender.h"

#include "Sys.h"

Gl2dRender::Gl2dRender() {
	textureDebug = new TextureDebugRenderer();
}

void Gl2dRender::render() {
	// draw the straight light texture ortho so we can easily debug it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, 800, 0, 900);
	glOrtho(0, 800, 0, 600, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<texture_preview>::iterator it;
	for(it = this->texturePreviews.begin(); it != this->texturePreviews.end(); it++) {
		if ((*it).winding == winding_mathsy) {
			this->textureDebug->renderMathsy((*it).texture, (*it).x, (*it).y, (*it).w, (*it).h);
		} else {
			this->textureDebug->renderMemory((*it).texture, (*it).x, (*it).y, (*it).w, (*it).h);
		}
	}

	this->texturePreviews.clear();
}

void Gl2dRender::drawTexturePreview(GLuint texture, int x, int y, int w, int h, texture_winding winding) {
	Gl2dRender::texture_preview preview;

	preview.texture = texture;
	preview.x = x;
	preview.y = y;
	preview.w = w;
	preview.h = h;
	preview.winding = winding;
	this->texturePreviews.push_back(preview);
}
