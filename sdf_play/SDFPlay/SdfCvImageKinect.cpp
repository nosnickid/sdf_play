#include "SdfCvImageKinect.h"

#include "Debug.h"

void SdfCvImageKinect::init() {
	SdfCvImage::init();

	this->kinToCv = new KinectToOpenCV();
	this->kinToCv->init();
}

void SdfCvImageKinect::loadRgbImage() {
	IplImage *frame = this->kinToCv->cvRGBImage;

	glBindTexture( GL_TEXTURE_2D, this->frameTexture); //bind the texture to it's array
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame->width, frame->height, 0, GL_BGR, GL_UNSIGNED_BYTE, frame->imageData);
	checkOpenGL("Image data from cap to texture");	
}

void SdfCvImageKinect::prepareFrame() {
	if (this->kinToCv->prepareFrame()) {
		this->loadRgbImage();
	}
}

