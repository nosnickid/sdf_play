#include "SdfCvCamera.h"
#include "Debug.h"

#include "Sys.h"
#include "glsl.h"

SdfCvCamera::SdfCvCamera(AbstractDepthMap *depth) :
	frameTexture(0), depthMap(depth) {
}


SdfCvCamera::~SdfCvCamera() {
	if (this->videoCapture.isOpened()) this->videoCapture.release();
	if (this->frameTexture > 0) glDeleteTextures(1, &this->frameTexture);
}

void SdfCvCamera::init() {
	this->videoCapture = cv::VideoCapture(CV_CAP_ANY);

	if (!this->videoCapture.isOpened()) {
		warning("SdfCvCamera::SdfCvCamera: failed to capture camera: %d", cvGetErrStatus());
	} else {
		info("Loaded camera!");
		this->createTextureForFrame();
		this->videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
		this->videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
		info("Camera operating at %f fps", this->videoCapture.get(CV_CAP_PROP_FPS));
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

	glBindTexture( GL_TEXTURE_2D, this->frameTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->vidFrame.cols, this->vidFrame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, this->vidFrame.data);

	checkOpenGL("Image data from cap to texture");
}

void SdfCvCamera::prepareFrame() {
	if (this->videoCapture.isOpened() && this->videoCapture.grab()) {
		/*this->videoCapture >> vidFrame;

		if (this->frameTexture > 0) {
			this->loadTextureFromIpl();
		}*/
	}
}

GLuint SdfCvCamera::getRgbImageTexture() {
	return this->frameTexture;
}

GLuint SdfCvCamera::getDepthMapTexture() {
	return this->depthMap->getDepthMapTexture();
}
