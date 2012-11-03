#include "glsl.h"

PFNGLSLERRORHANDLER gHandler;

void glslReportError(const char *prog, const char *msg) 
{
	if (gHandler != NULL) (*gHandler)(prog, msg);
}


GLhandleARB createShaderFromProgs(const GLcharARB *vertProg, const GLcharARB *fragProg) 
{
	GLhandleARB prog = glCreateProgramObjectARB();

	char infobuffer[1000];
	int infobufferlen = 0;
	int compileStatus[] = { 0 };
	GLuint error;
	
	GLhandleARB vertex = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	if ((error = glGetError()) != GL_NO_ERROR) { glslReportError("glCreateShaderObjectARB", vertProg); return NULL; }

	glShaderSourceARB(vertex, 1, &vertProg, NULL);
	glCompileShaderARB(vertex);
	glGetObjectParameterivARB(vertex, GL_OBJECT_COMPILE_STATUS_ARB, compileStatus);
	if (compileStatus[0] == 0) 
	{
		glGetInfoLogARB(vertex, 999, &infobufferlen, infobuffer);
		infobuffer[infobufferlen] = 0;
		glslReportError(vertProg, infobuffer);
		return NULL;
	}
	
	GLhandleARB frag = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	
	glShaderSourceARB(frag, 1, &fragProg, NULL);
	glCompileShaderARB(frag);
	glGetObjectParameterivARB(frag, GL_OBJECT_COMPILE_STATUS_ARB, compileStatus);
	if (compileStatus[0] == 0) 
	{
		glGetInfoLogARB(frag, 999, &infobufferlen, infobuffer);
		infobuffer[infobufferlen] = 0;
		glslReportError(fragProg, infobuffer);
		return NULL;
	}


	glAttachObjectARB(prog, vertex);
	glAttachObjectARB(prog, frag);
	
	glLinkProgramARB(prog);
	glGetObjectParameterivARB(prog, GL_OBJECT_LINK_STATUS_ARB, compileStatus);
	if (compileStatus[0] == 0) 
	{
		glGetInfoLogARB(prog, 999, &infobufferlen, infobuffer);
		infobuffer[infobufferlen] = 0;
		glslReportError(vertProg, infobuffer);
		return NULL;
	}

	return prog;

}


void glslSetErrorHandler(PFNGLSLERRORHANDLER handler)
{
	gHandler = handler;
}
