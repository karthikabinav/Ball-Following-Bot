
#include "cv.h"
//#include <stdio.h>
#include "highgui.h"

int main(int argc, char *argv[])
{
	// Set up images
	IplImage* img = cvLoadImage("a.png",1);
	IplImage* back_img = cvCreateImage( cvGetSize( img ), IPL_DEPTH_8U, 1 );

	// Compute HSV image and separate into colors
	IplImage* hsv = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );
	cvCvtColor( img, hsv, CV_BGR2HSV );

	IplImage* h_plane = cvCreateImage( cvGetSize( img ), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize( img ), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize( img ), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane };
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );

	// Build and fill the histogram
	int h_bins = 30, s_bins = 32;
	CvHistogram* hist;
	{
		int hist_size[] = { h_bins, s_bins };
		float h_ranges[] = { 0, 180 };
		float s_ranges[] = { 0, 255 };
		float* ranges[] = { h_ranges, s_ranges };
		hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	}
	cvCalcHist( planes, hist, 0, 0 ); // Compute histogram
	cvNormalizeHist( hist, 20*255 ); // Normalize it

	cvCalcBackProject( planes, back_img, hist );// Calculate back projection
	cvNormalizeHist( hist, 1.0 ); // Normalize it

	
	
	// Create an image to visualize the histogram
	int scale = 10;
	IplImage* hist_img = cvCreateImage( cvSize( h_bins * scale, s_bins * scale ), 8, 3 );
	cvZero ( hist_img );

	// populate the visualization
	float max_value = 0;
	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );

int h=0;
int s=0;
	for(  h = 0; h < h_bins; h++ )
{
		for(  s = 0; s < s_bins; s++ )
{
			float bin_val = cvQueryHistValue_2D( hist, h, s );
			int intensity = cvRound( bin_val * 255 / max_value );
			cvRectangle( hist_img, cvPoint( h*scale, s*scale) ,cvPoint( (h+1)*scale - 1, (s+1)*scale - 1),CV_RGB( intensity, intensity, intensity ), -1,8,0);
		}
	}

	// Show original
	cvNamedWindow( "Source", 1) ;
	cvShowImage( "Source", img );

	// Show back projection
	cvNamedWindow( "Back Projection", 1) ;
	cvShowImage( "Back Projection", back_img );

	// Show histogram equalized
	cvNamedWindow( "H-S Histogram", 1) ;
	cvShowImage( "H-S Histogram", hist_img );

	cvWaitKey(0);

	cvReleaseImage( &img );
	cvReleaseImage( &back_img );
	cvReleaseImage( &hist_img );

	return 0;
}
