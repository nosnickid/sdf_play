#include "SdfCvImage.h"

void SdfCvImage::init() {
	this->createTextureForRgb();
	this->createTextureForDepth();
}

GLuint SdfCvImage::getRgbImageTexture() {
	return this->rgbTexture;
}

GLuint SdfCvImage::getDepthMapTexture() {
	return this->depthTexture;
}

void SdfCvImage::createTextureForRgb() {
	glGenTextures(1, &this->rgbTexture);
	glBindTexture( GL_TEXTURE_2D, this->rgbTexture ); //bind the texture to its array
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void SdfCvImage::createTextureForDepth() {
	glGenTextures(1, &this->depthTexture);
	glBindTexture( GL_TEXTURE_2D, this->depthTexture); //bind the texture to its array
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}