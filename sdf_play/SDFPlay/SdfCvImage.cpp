#include "SdfCvImage.h"

void SdfCvImage::init() {
	this->createTextureForFrame();
}
void SdfCvImage::render() {
}

GLuint SdfCvImage::getRgbImageTexture() {
	return this->frameTexture;
}

void SdfCvImage::createTextureForFrame() {
	glGenTextures(1, &this->frameTexture);
	glBindTexture( GL_TEXTURE_2D, this->frameTexture ); //bind the texture to it's array
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

