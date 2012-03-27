//#include "stdafx.h"
#include "cv.h"
#include <stdio.h>
#include "highgui.h"
# include <SerialStream.h>
# include <iostream>
using namespace LibSerial;
using namespace std;
CvCapture* g_capture=NULL;





int main( int argc, char** argv ) 

{

cout<<"hello"<<endl;
SerialStream my_serial_stream("/dev/ttyUSB0", ios_base::in | ios_base::out);
cout<<"one"<<endl;
my_serial_stream.SetBaudRate( SerialStreamBuf::BAUD_2400 ) ;
cout<<"two"<<endl;
my_serial_stream.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
cout<<"three"<<endl;
my_serial_stream.SetNumOfStopBits(1) ;
cout<<"it has good propability of working "<<endl;

g_capture = cvCreateCameraCapture(0);

int max_value_red=0;
int bin=256;
float range[]={0,256};
float* ranges[]={range};

CvHistogram * hist_red = cvCreateHist(1,&bin, CV_HIST_ARRAY , ranges,1);
CvHistogram * hist_green = cvCreateHist(1,&bin, CV_HIST_ARRAY , ranges,1);
CvHistogram * hist_blue = cvCreateHist(1,&bin, CV_HIST_ARRAY , ranges,1);


IplConvKernel* kernel_ver = cvCreateStructuringElementEx(6,2,0,0,CV_SHAPE_RECT,NULL);
IplConvKernel* kernel_hor = cvCreateStructuringElementEx(2,6,0,0,CV_SHAPE_RECT,NULL);

IplImage* frame ;
IplImage* frame2;

IplImage* frame_rgb = cvCreateImage(cvSize(640,480),8,3);


while(1) 
{
frame = cvQueryFrame( g_capture );


IplImage* frame_new = cvCreateImage(cvSize(640,480),8,3);

IplImage* frame_result = cvCreateImage(cvSize(640,480),8,3);


IplImage* Histogram = cvCreateImage( cvSize(640,480), 8, 1 );
IplImage* blue_img = cvCreateImage( cvSize(640,480), 8, 1 );
IplImage* green_img = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1 );
IplImage* red_img_normal = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1 );
IplImage* blue_img_normal = cvCreateImage( cvSize(640,480), 8, 1 );
IplImage* green_img_normal = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1 );
IplImage* red_img = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1 );
IplImage* grey_img = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1 );


//changing
frame_rgb=frame;
cvCvtColor( frame, frame, CV_BGR2HSV );
//cha//nging



//cvCvtPixToPlane( frame, blue_img, green_img, red_img, 0 );
//IplImage* red_histogram = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 3 );

frame2=cvCloneImage(frame);
int width     = frame->width;
int height    = frame->height;
int nchannels = frame->nChannels;
int step      = frame->widthStep;
int step_red      = red_img->widthStep;
int step_blue      = blue_img->widthStep;
int step_green     = green_img->widthStep;
int step_red_normal      = red_img_normal->widthStep;
int step_blue_normal     = blue_img_normal->widthStep;
int step_green_normal    = green_img_normal->widthStep;

//normalizing the histograms


//	cvEqualizeHist( red_img, red_img_normal );

	//cvEqualizeHist( blue_img, blue_img_normal );
	//  cvEqualizeHist( green_img, green_img_normal );


//normalized the histograms


//printf("hi");
// setup the pointer to access image data 
uchar *data_old = ( uchar* )frame_rgb->imageData;  
uchar *data = ( uchar* )frame_new->imageData;  
uchar *data_result = ( uchar* )frame_result->imageData;    
uchar *data_red= ( uchar* )red_img->imageData;    
uchar *data_red_normal= ( uchar* )red_img_normal->imageData;   
uchar *data_blue_normal= ( uchar* )red_img_normal->imageData;   
uchar *data_green_normal= ( uchar* )red_img_normal->imageData;    
uchar *data_blue = ( uchar* )blue_img->imageData;    
uchar *data_green = ( uchar* )green_img->imageData;    

 
int i, j, r, g, b, byte;
for( i = 0 ; i < height ; i++ ) {
    for( j = 0 ; j < width ; j++ ) {


	r = data_red_normal[i*step_red_normal + j];
	g = data_green_normal[i*step_green_normal + j];
	b = data_blue_normal[i*step_blue_normal + j];

	
         data[i*step + j*nchannels + 0]=b;
	 data[i*step + j*nchannels + 1]=g;
         data[i*step + j*nchannels + 2]=r;



    }
}

//frame_new =cvClone(frame);
//uchar *data = ( uchar* )frame_new->imageData;  

int h,s,v;


for( i = 0 ; i < height ; i++ ) {
    for( j = 0 ; j < width ; j++ ) {




	 //r=
         b=data_old[i*step + j*nchannels + 0];
	 g=data_old[i*step + j*nchannels + 1];
         r=data_old[i*step + j*nchannels + 2];

         h=data[i*step + j*nchannels + 0];
	 s=data[i*step + j*nchannels + 1];
         v=data[i*step + j*nchannels + 2];
	int sum =(b+g+r)/3;

//if(( (h<=270 && h>230)&&(s<275&&s>200)&&(v<300&&v>210)) ||( (h<=360 && h>330)))//&&(s<275&&s>200)&&(v<300&&v>210))) 
//if( (h<=200 && h>175)&&(s<275&&s>180)&&(v<220&&v>120)) 
//if((h<260&&h>180)&&(b<170)&&(g<170)&&(r>150))//&&(v<250))	
if((h<180&&h>50)&&(r<175&&r>100)&&(g<150)&&(sum<100))
{
//blcak is positive signal
        data_result[i*step + j*nchannels + 0] = 0;
        data_result[i*step + j*nchannels + 1] = 0;
        data_result[i*step + j*nchannels + 2] = 0;
	}

	else    
	{
        data_result[i*step + j*nchannels + 0] = 250;
        data_result[i*step + j*nchannels + 1] = 250;
        data_result[i*step + j*nchannels + 2] = 250;
	}

}}





IplImage* circle = cvCreateImage( cvSize(640,480), 8, 1 );

IplImage* frame2 = cvCreateImage( cvSize(640,480), 8, 3 );
frame2=cvCloneImage(frame);
IplImage* src = cvCreateImage( cvSize(640,480), 8, 1 );
//IplImage* cirlce =cvCreateImage( cvSize(640,480), 8, 1 );
 //cvErode(frame_result , frame , kernel_ver , 1);
 //cvErode(frame_result , frame , kernel_hor , 1);
cvCvtColor( frame, circle, CV_BGR2GRAY );
//IplImage* src =NULL;

{
 src=cvCloneImage(circle);
 CvMemStorage* storage = cvCreateMemStorage(0);
 cvSmooth(circle, circle, CV_GAUSSIAN, 5, 5, 0, 0);
 CvSeq* results = cvHoughCircles(
    circle, 
    storage, 
    CV_HOUGH_GRADIENT, 
    4, 
    circle->width/10, 10, 270, 35, 80 
  );
 i = 0;
int j=0;


 for( i = 0; i < results->total; i++ ) 
 {
    
    float* p = (float*) cvGetSeqElem( results, i );
    printf("ball x=%f y=%f r=%f \n\r",p[0],p[1],p[2]);
    CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
    cvCircle( 
      src,
      pt, 
      cvRound( p[2] ),
      CV_RGB(0xff,0,0),3,8,0
    );
   
   
 if(p[0]>350)
	{
	printf("right\n");
	char r='r';
	my_serial_stream<<r;

	char s;
        my_serial_stream>>s;
	//cout<<s<<endl;

	}
    else if(p[0]<300)
	{
	printf("left\n");
        char r='l';
	my_serial_stream<<r;
	
	char s ;

	my_serial_stream>>s;
	//cout<<s<<endl;
	}

    else
	{
	char r='s';
	my_serial_stream<<r;
	printf("center\n");
	char s;
	//my_serial_stream>>s;

	}






  }














//String a="blue_hsv_equlaized";
cvNamedWindow("hist_hsv", CV_WINDOW_AUTOSIZE );
cvShowImage( "hist_hsv",frame2);


cvNamedWindow( "original", CV_WINDOW_AUTOSIZE );
cvShowImage( "original", src);
/*

cvNamedWindow( "original_hsv", CV_WINDOW_AUTOSIZE );
cvCvtColor( frame, frame, CV_BGR2HSV );
cvShowImage( "original_hsv",frame);


cvNamedWindow( "histogram_hsv", CV_WINDOW_AUTOSIZE );
cvCvtColor( frame_new, frame_new, CV_BGR2HSV );
cvShowImage( "histogram_hsv", frame_new );
//cvShowImage( "Example2", red_histogram);


  cvNamedWindow( "HoughCircles", 1 );
  cvShowImage( "HoughCircles", src);
*/
//cvCvtColor( frame_new, frame, CV_HSV2BGR );
cvNamedWindow( "result", CV_WINDOW_AUTOSIZE );
cvShowImage( "result", frame_rgb);


cvNamedWindow( "Contours 2", 1 );
	cvReleaseImage( &frame);
	//cvReleaseImage( &img_8uc3 );
	//cvReleaseImage( &img_edge );



}








//contour trying




//frame=(frame_new);

	//IplImage* frame = cvLoadImage( "a.png", 0 );
	IplImage* img_edge = cvCreateImage( cvSize(640,480), 8, 1 );
	IplImage* img_8uc3 = cvCreateImage( cvSize(640,480), 8, 3 );
	//changing to gray scale
	cvCvtColor( frame_rgb, grey_img, CV_BGR2GRAY );

	cvThreshold( grey_img, img_edge, 100, 250, CV_THRESH_BINARY );//minimum
//	cvThreshold( img_edge, img_edge, 200, 250, CV_THRESH_BINARY_INV );//maximum

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* first_contour = NULL;

	int Nc = cvFindContours(
		img_edge,
		storage,
		&first_contour,
		sizeof(CvContour),
		CV_RETR_LIST ,
		 CV_CHAIN_APPROX_SIMPLE,cvPoint(0, 0) );

	int n=0;
	//printf( "Total Contours Detected: %d\n", Nc );
	CvScalar red = CV_RGB(250,0,0);
	CvScalar blue = CV_RGB(0,0,250);
	CvSeq* c2=first_contour;

	//cvCvtColor( circle, img_8uc3, CV_GRAY2BGR );
	cvDrawContours(
			img_8uc3,
			c2,
			red,		// Red
			blue,		// Blue
			1,			// Vary max_level and compare results
			2,
			8 ,cvPoint(0,0));

		cvShowImage( "Contours 2", img_8uc3 );
	

	//printf( "Finished all contours.\n");


	//cvWaitKey(100000);





//contour trying
char c = cvWaitKey(33);
if( c == 27 ) break;




//printf("%d",max_value_red);
cvReleaseCapture( &g_capture );
cvDestroyWindow( "Example4" );
cvDestroyWindow( "Example3" );
cvDestroyWindow( "Example2" );
cvDestroyWindow( "Example1" );

	cvDestroyWindow( "Contours 2" );
return(0);
}

