//#include "stdafx.h"
#include "cv.h"
#include <stdio.h>
#include "highgui.h"
int
g_slider_position = 0;
CvCapture* g_capture=NULL;

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
cvNamedWindow( "Example4", CV_WINDOW_AUTOSIZE );
cvNamedWindow( "Example3", CV_WINDOW_AUTOSIZE );
cvNamedWindow( "Example2", CV_WINDOW_AUTOSIZE );
cvNamedWindow( "Example1", CV_WINDOW_AUTOSIZE );
g_capture = cvCreateCameraCapture(-1);

int max_value_red=0;
int bin=256;
float range[]={0,256};
float* ranges[]={range};

CvHistogram * hist_red = cvCreateHist(1,&bin, CV_HIST_ARRAY , ranges,1);
CvHistogram * hist_green = cvCreateHist(1,&bin, CV_HIST_ARRAY , ranges,1);
CvHistogram * hist_blue = cvCreateHist(1,&bin, CV_HIST_ARRAY , ranges,1);



//int frames = (int) cvGetCaptureProperty(
//g_capture,
//CV_CAP_PROP_FRAME_COUNT
//);


/*if( frames!= 0 ) 

{
cvCreateTrackbar(
"Position",
"Example2",
&g_slider_position,
frames,
onTrackbarSlide
);
}*/

IplImage* frame ;
IplImage* frame2;
// While loop (as in Example 2) capture & show video
//...
// Release memory and destroy window
//...


while(1) 
{
frame = cvQueryFrame( g_capture );

        IplImage* disp_img = cvCreateImage(cvSize((frame->width)/1.6,(frame->height)/1.6),IPL_DEPTH_8U,3);

        cvResize(frame,disp_img,CV_INTER_LINEAR);
IplImage* Histogram = cvCreateImage( cvGetSize(frame), 8, 1 );
IplImage* blue_img = cvCreateImage( cvGetSize(frame), 8, 1 );
IplImage* green_img = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
IplImage* red_img = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
  cvCvtPixToPlane( frame, blue_img, green_img, red_img, 0 );
IplImage* red_histogram = cvCreateImage( cvGetSize(Histogram), IPL_DEPTH_8U, 3 );

frame2=cvCloneImage(frame);
//cvCvtColor(frame,frame2,CV_RGB2HSV);
//frame2=convertRGBtoHSV(frame);
int width     = frame->width;
int height    = frame->height;
int nchannels = frame->nChannels;
int step      = frame->widthStep;
//cvCopy(frame,frame2,NULL);




/* setup the pointer to access image data */
uchar *data = ( uchar* )frame->imageData;    
 
/* convert to grayscale manually */
int i, j, r, g, b, byte;
for( i = 0 ; i < height ; i++ ) {
    for( j = 0 ; j < width ; j++ ) {
        r = data[i*step + j*nchannels + 0];
        g = data[i*step + j*nchannels + 1];
        b = data[i*step + j*nchannels + 2];
       
        byte = ( r + g + b ) / 3;
       
	if((r>100)&&(g<100)&&(b<100))
	{
        data[i*step + j*nchannels + 0] = 0;
        data[i*step + j*nchannels + 1] = 0;
        data[i*step + j*nchannels + 2] = 0;
	}

	else
	{
        data[i*step + j*nchannels + 0] = 250;
        data[i*step + j*nchannels + 1] = 250;
        data[i*step + j*nchannels + 2] = 250;
	}
    }
}










if( !frame ) break;

cvCalcHist(&red_img,hist_red,0,NULL);
//cvShowImage( "Example1", hist_red );

//cvCalcHist(&red_img,hist_red,0,NULL);
//cvCalcHist(&red_img,hist_red,0,NULL);
//IplImage* imgHistRed=DrawHistogram(hist);
//cvClearHist(hist);
 max_value_red=0;
 for(  i = 0; i < bin; i++ )
        {
         int   find_max = cvQueryHistValue_1D(hist_red,i);
            if (find_max > max_value_red)
            {

                max_value_red = find_max;
            }

        }
//scaling down not required that much
 cvScale( hist_red->bins, hist_red->bins, 438/max_value_red,0);

 for(  i= 0; i < bin; i++ )
     {

	int h1=cvRound(cvQueryHistValue_1D(hist_red,i));
      cvRectangle( red_histogram, cvPoint(i*3+ 15, 448),cvPoint(i*3+16, 448-h1),cvScalar(0x00,0x00,0xff,0), -1,8,0);
      }
	cvAdd(red_histogram,Histogram,Histogram,NULL);




//cvShowImage( "Example4", red_img);
//cvShowImage( "Example3", blue_img);
//cvShowImage( "Example2", green_img);
cvShowImage( "Example1", frame2 );
cvShowImage( "Example2", red_histogram);




char c = cvWaitKey(33);
if( c == 27 ) break;
}



//printf("%d",max_value_red);
cvReleaseCapture( &g_capture );
cvDestroyWindow( "Example2" );
return(0);
}

