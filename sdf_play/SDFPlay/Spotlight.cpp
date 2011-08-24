#include "Spotlight.h"
#include "Debug.h"

Spotlight::Spotlight()
{

	// capture coloring info later for debug if interesting?
	/*glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);*/

    glGetError();

	// We want a temp depth buffer please.
	glGenTextures(1, &this->depthBuffer);
    if (int i = glGetError() != GL_NO_ERROR) { warning("Depth buffer fail: %x\n", i); }
	glBindTexture(GL_TEXTURE_2D, this->depthBuffer);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Via this.
	glGenFramebuffersEXT(1, &this->frameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->frameBuffer);
	// glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureId, 0);

	// Tell it we want no draw buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, this->depthBuffer, 0);
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) fatal("::SpotLight glCheckFramebufferStatusEXT failed!");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	this->lightMatrix = new GLfloat[16];

	this->projMatrix = new GLfloat[16];
	this->mviewMatrix = new GLfloat[16];
}

Spotlight::~Spotlight()
{
	delete this->lightMatrix;
	delete this->projMatrix;
	delete this->mviewMatrix;
}

void Spotlight::PrepareRender()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	position.GlMult();

    glViewport(0, 0, 1024,1024);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 800.0/600.0, 1, 10000);
    
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
}

void Spotlight::RenderDone()
{
	glGetFloatv(GL_MODELVIEW_MATRIX, this->mviewMatrix);
	checkOpenGL("Spotlight::retrieve modelview");
	glGetFloatv(GL_PROJECTION_MATRIX, this->projMatrix);
	checkOpenGL("Spotlight::retrieve projection");
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

GLfloat* Spotlight::GetLightMatrix()
{
	// this is so dodgé.
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	this->position.GlMult();

	glGetFloatv(GL_MODELVIEW_MATRIX, this->lightMatrix);

	glPopMatrix();
	
	return this->lightMatrix;
}
