#include "TextureDebugRenderer.h"

#include "Debug.h"
#include "glsl.h"

TextureDebugRenderer::TextureDebugRenderer() {
	const GLcharARB *tprogVert = "\
		varying vec2 texcoord; \
		void main() {\
			gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; \
			texcoord = vec2(gl_MultiTexCoord0); \
		}";
	const GLcharARB *tprogFrag = "\
		varying vec2 texcoord; \
		uniform sampler2D texture; \
		void main() { \
			gl_FragColor = texture2D(texture, texcoord); \
		}";
	this->textureProg = createShaderFromProgs(tprogVert, tprogFrag);
	checkOpenGL("texture prog");
}

void TextureDebugRenderer::render(GLuint texture, int x, int y, int w, int h) {
	glUseProgramObjectARB(textureProg);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2i(x, y);
	glTexCoord2d(1, 0); glVertex2i(x+w, y);
	glTexCoord2d(1, 1); glVertex2i(x+w, y+h);
	glTexCoord2d(0, 1); glVertex2i(x, y+h);
	glEnd();
}