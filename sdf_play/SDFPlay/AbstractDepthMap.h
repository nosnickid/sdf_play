#ifndef ABSTRACT_DEPTH_MAP_H__
#define ABSTRACT_DEPTH_MAP_H__

#include "Sys.h"


	class AbstractDepthMap {
	public:
		/**
		 * Load a depth map into the texture
		 */
		virtual void loadDepthMap() = 0;
		virtual GLuint getDepthMapTexture() = 0;
	};

#endif
