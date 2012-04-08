#include "ManualDepthMap.h"

ManualDepthMap::ManualDepthMap() {
	this->texture = 0;
}

void ManualDepthMap::loadDepthMap() {
	glBindTexture(GL_TEXTURE_2D, this->getDepthMapTexture());

	// Manually build some array
	const int w = 320;
	const int h = 240;
	GLubyte map[w*h];
	
	for(int x = 0; x < w; x++) {
		for(int y = 0; y < h; y++) {
			map[(y*w)+(x)] = (GLubyte)(256 * (sin(2 * M_PI * x/w) + cos(2 * M_PI * y/w)));
		}
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, 1, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, map);	

	checkOpenGL("load depth map, manual");
}

GLuint ManualDepthMap::getDepthMapTexture() {
	if (this->texture == 0) {
		glGenTextures(1, &this->texture);
		glBindTexture( GL_TEXTURE_2D, this->texture ); //bind the texture to it's array
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	 return this->texture;
}
