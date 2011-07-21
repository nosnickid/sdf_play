#include "glsl.h"


GLhandleARB createShaderFromProgs(const GLcharARB *vertProg, const GLcharARB *fragProg) 
{
	GLhandleARB prog = glCreateProgramObjectARB();

	GLhandleARB vertex = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	glShaderSourceARB(vertex, 1, &vertProg, NULL);
	glCompileShaderARB(vertex);
	
	GLhandleARB frag = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	
	glShaderSourceARB(frag, 1, &fragProg, NULL);
	glCompileShaderARB(frag);

	glAttachObjectARB(prog, vertex);
	glAttachObjectARB(prog, frag);
	
	glLinkProgramARB(prog);

	return prog;

}