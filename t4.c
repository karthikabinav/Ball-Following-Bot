#include "cv.h"
#include "highgui.h"
int
g_slider_position = 0;
CvCapture* g_capture
= NULL;
void onTrackbarSlide(int pos) 
{
cvSetCaptureProperty(
g_capture,
CV_CAP_PROP_POS_FRAMES,
pos
);




}
int main( int argc, char** argv ) 

{
cvNamedWindow( "Example2", CV_WINDOW_AUTOSIZE );
g_capture = cvCreateFileCapture( "avatar.rmvb" );
int frames = (int) cvGetCaptureProperty(
g_capture,
CV_CAP_PROP_FRAME_COUNT
);


if( frames!= 0 ) 

{
cvCreateTrackbar(
"Position",
"Example2",
&g_slider_position,
frames,
onTrackbarSlide
);
}
IplImage* frame;
// While loop (as in Example 2) capture & show video
//...
// Release memory and destroy window
//...


while(1) 
{
frame = cvQueryFrame( g_capture );
if( !frame ) break;
cvShowImage( "Example2", frame );
char c = cvWaitKey(33);
if( c == 27 ) break;
}
cvReleaseCapture( &g_capture );
cvDestroyWindow( "Example2" );
return(0);
}

