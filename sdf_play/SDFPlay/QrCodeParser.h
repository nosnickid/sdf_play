#ifndef SDF_PLAY_QR_CODE_PARSER_H__
#define SDF_PLAY_QR_CODE_PARSER_H__

#include "opencv/cv.h"
#include "opencv/highgui.h"

class QrCodeParser {
public:
	void parseOpenCvImage(IplImage *image);

};


#endif