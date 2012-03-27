#include "cv.h"
#include "highgui.h"
int main( int argc, char** argv )

{










IplImage* image = cvLoadImage( argv[1],1 );
cvNamedWindow( "Example4-in" );
cvNamedWindow( "Example4-out" );
// Create a window to show our input image
cvShowImage( "Example4-in", image );

IplImage* out = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3);
//first to get the size fo the image , second to get the type of the image,third is number //of channels
// Do the smoothing
//
cvSmooth( image, out, CV_GAUSSIAN, 3, 3 );//here 3 into 3 means that the area of smoothening is 3 into 3
// Show the smoothed image in the output window
//
cvShowImage( "Example4-out", out );
// Be tidy
//
cvReleaseImage( &out );
// Wait for the user to hit a key, then clean up the windows
//
cvWaitKey( 0 );
cvDestroyWindow( "Example4-in" );
cvDestroyWindow( "Example4-out" );
}



