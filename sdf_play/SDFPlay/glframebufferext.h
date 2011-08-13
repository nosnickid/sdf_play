#ifndef SDFPLAY_GLEXTFRAMEBUFFER_H_
#define SDFPLAY_GLEXTFRAMEBUFFER_H_

#ifdef _WIN32
#include <windows.h>

#ifndef APIENTRYP 
#define APIENTRYP APIENTRY *
#endif

#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"

extern PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;


void init_glframebufferext();


#endif
