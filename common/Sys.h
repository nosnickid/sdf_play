#ifndef SDF_PLAY_SYS_H_
#define SDF_PLAY_SYS_H

#ifdef __MACH__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  ifdef WIN32
#    include <windows.h>
#    include <GL/gl.h>
#    include <GL/glu.h>
#    include "glext.h"
#  else
#    include <GL/gl.h>
#    include <GL/glu.h>
#  endif
#endif


#include <SDL.h>

#ifdef WIN32
#  ifndef APIENTRYP 
#    define APIENTRYP APIENTRY *
#  endif
#endif


#endif
