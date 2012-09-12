#ifndef ABSTRACT_RGB_MAP_H__
#define ABSTRACT_RGB_MAP_H__

#include "Sys.h"

class AbstractRgbImage {
public:
	/**
		* Load a depth map into the texture
		*/
	virtual GLuint getRgbImageTexture() = 0;
};

#endif
