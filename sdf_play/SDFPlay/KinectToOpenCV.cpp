#include "KinectToOpenCV.h"

#pragma comment(lib, "Kinect10.lib")
#pragma comment(lib, "opencv_core231.lib")
#pragma comment(lib, "opencv_imgproc231.lib")
#pragma comment(lib, "opencv_highgui231.lib")


namespace orangestems { namespace kinect {

DWORD WINAPI RGBImage( LPVOID lpParam );
DWORD WINAPI DepthImage( LPVOID lpParam );
RGBQUAD Nui_ShortToQuad( unsigned short s );

void KinectToOpenCV::init() {
	this->cvRGBImage = cvCreateImage( cvSize( 640, 480 ), IPL_DEPTH_8U, 4 );
	this->cvDepthImage = cvCreateImage( cvSize( 320, 240 ), IPL_DEPTH_8U, 4 );
	this->DepthEdges = cvCreateImage( cvSize( 320, 240 ), IPL_DEPTH_8U, 1 );

	this->DepthEdges2 = cvCreateImage( cvSize( 320, 240 ), IPL_DEPTH_8U, 1 );

	this->RGBDepth = new RGBQUAD[ 320 * 240 ];

	this->hEvents[0] = CreateEvent( NULL, TRUE, FALSE, NULL );
	this->hEvents[1] =  CreateEvent( NULL, TRUE, FALSE, NULL );
	this->hStreams[0] = this->hStreams[1] = 0;

	NuiImageStreamOpen( NUI_IMAGE_TYPE::NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION::NUI_IMAGE_RESOLUTION_640x480, NULL, 2, hEvents[0], &hStreams[0] );
	NuiImageStreamOpen( NUI_IMAGE_TYPE::NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION::NUI_IMAGE_RESOLUTION_320x240, NULL, 2, hEvents[1], &hStreams[1] );

	this->hThreads[0] = CreateThread( NULL, 0, RGBImage, (LPVOID*)(this), NULL, NULL);
	this->hThreads[1] = CreateThread( NULL, 0, DepthImage, (LPVOID*)(this), NULL, NULL);

}

KinectToOpenCV::~KinectToOpenCV() {
	NuiShutdown();
}

DWORD WINAPI RGBImage( LPVOID lpParam )
{
	KinectToOpenCV *ko = (KinectToOpenCV *) lpParam;

	while ( true )
	{
		WaitForSingleObject( ko->hEvents[0], INFINITE);
		//printf( "[+] Got RGB Frame!\n" );

		const NUI_IMAGE_FRAME *RGBFrame;
		NuiImageStreamGetNextFrame( ko->hStreams[0], NULL, &RGBFrame );

		NUI_LOCKED_RECT lockedRGB;
		RGBFrame->pFrameTexture->LockRect( NULL, &lockedRGB, NULL, NULL );
		
		cvSetData( ko->cvRGBImage, (unsigned char*)lockedRGB.pBits, lockedRGB.Pitch );
		// cvShowImage( "RGB", ko->cvRGBImage );
		// if ( cvWaitKey( 1 ) == 'x' ){ break; }

		NuiImageStreamReleaseFrame( ko->hStreams[0], RGBFrame );
	}

	return 0;
}

DWORD WINAPI DepthImage( LPVOID lpParam )
{
	KinectToOpenCV *ko = (KinectToOpenCV *) lpParam;

	while ( true )
	{
		WaitForSingleObject( ko->hEvents[1], INFINITE);
		printf( "[+] Got Depth Frame!\n" );

		const NUI_IMAGE_FRAME *DepthFrame;
		NuiImageStreamGetNextFrame( ko->hStreams[1], NULL, &DepthFrame );
		
		NUI_LOCKED_RECT lockedDepth;
		DepthFrame->pFrameTexture->LockRect( NULL, &lockedDepth, NULL, NULL );
		
		unsigned char* Buffer = (unsigned char*) lockedDepth.pBits;
		
		RGBQUAD* rgbLoop =  ko->RGBDepth;
		unsigned short* BufferLoop = (unsigned short*) Buffer;

		for ( int y = 0; y < 240; y++ )
		{
			for ( int x = 0; x < 320; x++ )
			{
				RGBQUAD Quad = Nui_ShortToQuad( *BufferLoop );
				BufferLoop++;

				*rgbLoop = Quad;
				rgbLoop++;
			}
		}

		cvSetData( ko->cvDepthImage, (unsigned char*) ko->RGBDepth, ko->cvDepthImage->widthStep );
		//cvCvtColor( cvDepthImage, DepthEdges, CV_RGB2GRAY);
		//cvCanny(  DepthEdges, DepthEdges2, 100, 100, 3 );
		
		// cvShowImage( "Depth",  ko->cvDepthImage );
		// if ( cvWaitKey( 1 ) == 'x' ){ break; }

		NuiImageStreamReleaseFrame( ko->hStreams[1], DepthFrame );
	}

	return 0;
}


RGBQUAD Nui_ShortToQuad( unsigned short s )
{
	USHORT RealDepth = s & 0xffff;
	USHORT Player =  0;

	// transform 13-bit depth information into an 8-bit intensity appropriate
	// for display (we disregard information in most significant bit)
	BYTE l = 255 - (BYTE)(256*RealDepth/0x0fff);

	RGBQUAD q;
	q.rgbRed = q.rgbBlue = q.rgbGreen = 0;

	switch( Player )
	{
	case 0:
		q.rgbRed = l / 2;
		q.rgbBlue = l / 2;
		q.rgbGreen = l / 2;
		break;
	case 1:
		q.rgbRed = l;
		break;
	case 2:
		q.rgbGreen = l;
		break;
	case 3:
		q.rgbRed = l / 4;
		q.rgbGreen = l;
		q.rgbBlue = l;
		break;
	case 4:
		q.rgbRed = l;
		q.rgbGreen = l;
		q.rgbBlue = l / 4;
		break;
	case 5:
		q.rgbRed = l;
		q.rgbGreen = l / 4;
		q.rgbBlue = l;
		break;
	case 6:
		q.rgbRed = l / 2;
		q.rgbGreen = l / 2;
		q.rgbBlue = l;
		break;
	case 7:
		q.rgbRed = 255 - ( l / 2 );
		q.rgbGreen = 255 - ( l / 2 );
		q.rgbBlue = 255 - ( l / 2 );
	}

	return q;
}

	

}}

