#ifndef SDF_PLAY_SYS_H_
#define SDF_PLAY_SYS_H

#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#ifndef APIENTRYP 
#define APIENTRYP APIENTRY *
#endif

#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#endif


#endif
