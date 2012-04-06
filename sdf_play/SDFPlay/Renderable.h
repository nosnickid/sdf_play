#ifndef RENDERABLE_H__
#define RENDERABLE_H__

namespace sdf_play { namespace render {

	class Renderable {
	public:
		virtual void init() = 0;
		virtual void prepareFrame() = 0;
		virtual void render() = 0;
	};

}}

#endif
