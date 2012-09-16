#include "SdfCvCamera.h"
#include "Debug.h"

#include "Sys.h"
#include "glsl.h"

SdfCvCamera::SdfCvCamera(AbstractDepthMap *depth) :
 frame(NULL), frameTexture(0), camera(NULL), depthMap(depth) {
}


SdfCvCamera::~SdfCvCamera() {
	if (this->camera != NULL) cvReleaseCapture(&this->camera);
	// if (this->frame != NULL) cvReleaseImage(&this->frame);
}

void SdfCvCamera::init() {
	this->camera = cvCaptureFromCAM(CV_CAP_ANY);
	if (!this->camera) {
		warning("SdfCvCamera::SdfCvCamera: failed to capture camera: %d", cvGetErrStatus());
	} else {
		info("Loaded camera!");
		this->createTextureForFrame();
		cvSetCaptureProperty(this->camera, CV_CAP_PROP_FRAME_WIDTH, 320);
		cvSetCaptureProperty(this->camera, CV_CAP_PROP_FRAME_HEIGHT, 240 );
	}
}

void SdfCvCamera::createTextureForFrame() {
	glGenTextures(1, &this->frameTexture);
	glBindTexture( GL_TEXTURE_2D, this->frameTexture ); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void SdfCvCamera::loadTextureFromIpl() {

	if (this->frame == NULL) return;

	glBindTexture( GL_TEXTURE_2D, this->frameTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->frame->width, this->frame->height, 0, GL_BGR, GL_UNSIGNED_BYTE, this->frame->imageData);

    checkOpenGL("Image data from cap to texture");	
}

void SdfCvCamera::prepareFrame() {
	if (this->camera) {
		this->frame = cvQueryFrame(this->camera);

		if (this->frameTexture > 0) {
			this->loadTextureFromIpl();
		}

		// cvReleaseImage(&this->frame);
		// this->frame = NULL;
	}
}

GLuint SdfCvCamera::getRgbImageTexture() {
	return this->frameTexture;
}

GLuint SdfCvCamera::getDepthMapTexture() {
	return this->depthMap->getDepthMapTexture();
}
