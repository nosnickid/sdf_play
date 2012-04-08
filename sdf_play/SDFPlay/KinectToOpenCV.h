#ifndef KINECT_TO_OPENCV_H__
#define KINECT_TO_OPENCV_H__

#include <windows.h>
#include "NuiApi.h"

#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


class KinectToOpenCV {
private:
	bool updated[2];

	void pollRGB();
	void pollDepth();

public:

	IplImage *cvRGBImage;
	IplImage *cvDepthImage;
	IplImage *DepthEdges;

	IplImage *DepthEdges2;

	RGBQUAD	*RGBDepth;

	HANDLE hEvents[2];
	HANDLE hStreams[2];

	~KinectToOpenCV();
	void init();
	bool prepareFrame();
};


#endif