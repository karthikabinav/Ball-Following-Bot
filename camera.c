#include "cv.h"
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

cvNamedWindow( "Example2", CV_WINDOW_AUTOSIZE );
cvNamedWindow( "Example1", CV_WINDOW_AUTOSIZE );
g_capture = cvCaptureFromCAM(0);
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

IplImage* frame;
IplImage* frame2;
IplImage* frame3=NULL;
// While loop (as in Example 2) capture & show video
//...
// Release memory and destroy window
//...

IplConvKernel* kernel_new = cvCreateStructuringElementEx(13,13,0,0,CV_SHAPE_ELLIPSE,NULL);
IplConvKernel* kernel_new2 = cvCreateStructuringElementEx(2,2,0,0,CV_SHAPE_ELLIPSE,NULL);


while(1) 
{
frame = cvQueryFrame( g_capture );
cvSmooth(frame , frame , CV_GAUSSIAN , 3,0,0,0);
frame2=cvCloneImage(frame);
cvCvtColor( frame, frame2, CV_BGR2HSV );
int width     = frame->width;
int height    = frame->height;
int nchannels = frame->nChannels;
int step      = frame->widthStep;
//cvCopy(frame,frame2,NULL);




/* setup the pointer to access image data */


uchar *data = ( uchar* )frame->imageData;    
uchar *data2 = ( uchar* )frame->imageData;    
 

int i, j, r, g, b, h,s,v;
for( i = 0 ; i < height ; i++ ) {
    for( j = 0 ; j < width ; j++ ) {
        b = data[i*step + j*nchannels + 0];
        g = data[i*step + j*nchannels + 1];
        r = data[i*step + j*nchannels + 2];
       
        h = data2[i*step + j*nchannels + 0];
        s = data2[i*step + j*nchannels + 1];
        v = data2[i*step + j*nchannels + 2];
       

frame3=cvCloneImage(frame);
frame3->nChannels=1;
cvCvtColor(frame,frame3,CV_BGR2GRAY);
       
//cvEqualizeHist(frame3, frame3);


	int r1=0 ,r2=0,g1=0,g2=0,b1=0,b2=0;
b1=40;
r1=120;
g1=60;
b2=100;
r2=205;
g2=165;

//       if((r<r2)&&(r>r1)&&(b<b2)&&(b>b1)&&(g<g2)&&(g>g1))

if(( (h<=180 && h>150)) && /*(v>30&&v<80)*/ (s>180 && s<220))
{
//white
        data[i*step + j*nchannels + 0] = 255;
        data[i*step + j*nchannels + 1] = 255;
        data[i*step + j*nchannels + 2] = 255;
}


else
{

        data[i*step + j*nchannels + 0] = 0;
        data[i*step + j*nchannels + 1] = 0;
        data[i*step + j*nchannels + 2] = 0;
}



    }

}






cvErode(frame , frame , kernel_new , 1);
cvDilate(frame , frame , NULL , 1);
//hue=13
//value
//

if( !frame ) break;
cvShowImage( "Example2", frame2 );
cvShowImage( "Example1", frame );
char c = cvWaitKey(33);
if( c == 27 ) break;
}
cvReleaseCapture( &g_capture );
cvDestroyWindow( "Example2" );
return(0);
}

