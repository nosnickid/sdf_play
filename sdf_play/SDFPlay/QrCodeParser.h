#ifndef SDF_PLAY_QR_CODE_PARSER_H__
#define SDF_PLAY_QR_CODE_PARSER_H__

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class QrCodeParser {
public:
	void parseOpenCvImage(IplImage *image);

};


#endif
