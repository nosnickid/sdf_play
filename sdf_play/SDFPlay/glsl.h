#ifndef SDFPLAY_GLSL_H_
#define SDFPLAY_GLSL_H_

#ifdef _WIN32
#include <windows.h>

#ifndef APIENTRYP 
#define APIENTRYP APIENTRY *
#endif

#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include "glext.h"

extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
extern PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
extern PFNGLLINKPROGRAMARBPROC glLinkProgramARB;

void init_glsl();


#endif