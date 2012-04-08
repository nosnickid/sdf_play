#ifndef KINECT_TO_OPENCV_H__
#define KINECT_TO_OPENCV_H__

#include <windows.h>
#include "NuiApi.h"

#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


namespace orangestems { namespace kinect {


class KinectToOpenCV {
public:

	IplImage *cvRGBImage;
	IplImage *cvDepthImage;
	IplImage *DepthEdges;

	IplImage *DepthEdges2;

	RGBQUAD	*RGBDepth;

	HANDLE hThreads[2];
	HANDLE hEvents[2];
	HANDLE hStreams[2];


	~KinectToOpenCV();
	void init();
};

} }

#endif