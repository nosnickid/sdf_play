#ifndef MANUAL_DEPTH_MAP_H__
#define MANUAL_DEPTH_MAP_H__

#include "Sys.h"
#include "AbstractDepthMap.h"
#include "Debug.h"


#define _USE_MATH_DEFINES
#include <cmath>

namespace sdf_play { namespace render {

	class ManualDepthMap: public AbstractDepthMap {
	protected:
		GLuint texture;
	public:
		ManualDepthMap();
		void loadDepthMap();
		GLuint getDepthMapTexture();
	};

}}


#endif
