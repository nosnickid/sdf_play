#include "SdfCvCamera.h"
#include "Debug.h"


SdfCvCamera::SdfCvCamera() :
 frame(NULL), frameTexture(0) {
	this->camera = cvCaptureFromCAM(CV_CAP_ANY);
	if (!this->camera) warning("SdfCvCamera::SdfCvCamera: failed to capture camera: %d", cvGetErrStatus());
}

SdfCvCamera::~SdfCvCamera() {
	if (this->camera != NULL) cvReleaseCapture(&this->camera);
	// if (this->frame != NULL) cvReleaseImage(&this->frame);
}

void SdfCvCamera::createTextureForFrame() {
	glGenTextures(1, &this->frameTexture);
	glBindTexture( GL_TEXTURE_2D, this->frameTexture ); //bind the texture to it's array
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void SdfCvCamera::loadTextureFromIpl() {

	if (this->frame == NULL) return;

	glBindTexture( GL_TEXTURE_2D, this->frameTexture); //bind the texture to it's array

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->frame->width, this->frame->height, 0, GL_BGR, GL_UNSIGNED_BYTE, this->frame->imageData);

    checkOpenGL("Image data from cap to texture");	
}

void SdfCvCamera::captureFrame() {
	if (this->camera) {
		this->frame = cvQueryFrame(this->camera);

		if (this->frameTexture > 0) {
			this->loadTextureFromIpl();
		}

		// cvReleaseImage(&this->frame);
		// this->frame = NULL;
	}

}