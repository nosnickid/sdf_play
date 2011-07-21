#include "glsl.h"

PFNGLSLERRORHANDLER gHandler;

void glslReportError(char *msg) 
{
	if (gHandler != NULL) (*gHandler)(msg);
}


GLhandleARB createShaderFromProgs(const GLcharARB *vertProg, const GLcharARB *fragProg) 
{
	GLhandleARB prog = glCreateProgramObjectARB();

	char infobuffer[1000];
	int infobufferlen = 0;
	int compileStatus[] = { 0 };
	
	GLhandleARB vertex = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	// if ((error = glGetError()) != GL_NO_ERROR) { glslReportError("glCreateShaderObjectARB"); return NULL; }

	glShaderSourceARB(vertex, 1, &vertProg, NULL);
	glCompileShaderARB(vertex);
	glGetObjectParameterivARB(vertex, GL_OBJECT_COMPILE_STATUS_ARB, compileStatus);
	if (compileStatus[0] == 0) 
	{
		glGetInfoLogARB(vertex, 999, &infobufferlen, infobuffer);
		infobuffer[infobufferlen] = 0;
		glslReportError(infobuffer);
	}
	
	GLhandleARB frag = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	
	glShaderSourceARB(frag, 1, &fragProg, NULL);
	glCompileShaderARB(frag);
	glGetObjectParameterivARB(frag, GL_OBJECT_COMPILE_STATUS_ARB, compileStatus);
	if (compileStatus[0] == 0) 
	{
		glGetInfoLogARB(frag, 999, &infobufferlen, infobuffer);
		infobuffer[infobufferlen] = 0;
		glslReportError(infobuffer);
	}


	glAttachObjectARB(prog, vertex);
	glAttachObjectARB(prog, frag);
	
	glLinkProgramARB(prog);
	// if ((error = glGetError()) != GL_NO_ERROR) { glslReportError("glLinkProgram"); return NULL; }

	return prog;

}


void glslSetErrorHandler(PFNGLSLERRORHANDLER handler)
{
	gHandler = handler;
}