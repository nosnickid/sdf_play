#include "SdfCvImageKinect.h"

#include "Debug.h"

void SdfCvImageKinect::init() {
	SdfCvImage::init();

	this->kinToCv = new KinectToOpenCV();
	this->kinToCv->init();
}

void SdfCvImageKinect::loadRgbImage() {
	IplImage *frame = this->kinToCv->cvRGBImage;

	glBindTexture( GL_TEXTURE_2D, this->rgbTexture); //bind the texture to it's array
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame->width, frame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->imageData);
	checkOpenGL("rgb cap from kinect");	
}

void SdfCvImageKinect::loadDepthMap() {
	IplImage *frame = this->kinToCv->cvDepthImage;

	glBindTexture( GL_TEXTURE_2D, this->depthTexture); //bind the texture to it's array
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE16, frame->width, frame->height, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, frame->imageData);
	checkOpenGL("depth cap from kinect");	
}

void SdfCvImageKinect::prepareFrame() {
	if (this->kinToCv->prepareFrame()) {
		this->loadRgbImage();
		this->loadDepthMap();
	}
}

