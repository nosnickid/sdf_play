#ifndef SDFPLAY_GLSL_H_
#define SDFPLAY_GLSL_H_

#ifdef _WIN32
#include <windows.h>

#ifndef APIENTRYP 
#define APIENTRYP APIENTRY *
#endif

#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"

extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
extern PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
extern PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;

typedef void (APIENTRYP PFNGLSLERRORHANDLER) (GLcharARB *message);

void init_glsl();

void glslSetErrorHandler(PFNGLSLERRORHANDLER handler);


GLhandleARB createShaderFromProgs(const GLcharARB *vertProg, const GLcharARB *fragProg);


#endif