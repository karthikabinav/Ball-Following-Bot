#include<highgui.h>
# include <iostream>
#include<cv.h>
# include <SerialStream.h>
using namespace LibSerial;
using namespace std;

int main()
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

IplImage *frame,*hsv,*colordet,*temp,*gray_frame;
int x,y,z,i;
char c;
IplConvKernel* ker=cvCreateStructuringElementEx(7,7,3,3,CV_SHAPE_ELLIPSE,NULL);
cvNamedWindow("HOUGHCIRCLE",CV_WINDOW_AUTOSIZE);
CvMemStorage *storage=cvCreateMemStorage(0);
CvCapture *capture=cvCreateCameraCapture(1);
cvNamedWindow("Original",0);
cvNamedWindow("HSV",0); 
cvNamedWindow("ColorDet",0);
frame=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
hsv=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,3);
colordet=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
gray_frame=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
while(1)
{
frame=cvQueryFrame(capture);
cvShowImage("Original",frame);

cvCvtColor(frame,hsv,CV_BGR2HSV);
cvShowImage("HSV",hsv);
for(y=0;y<hsv->height;y++)
{
uchar* ptr=(uchar*)(hsv->imageData+y*hsv->widthStep);
for(x=0;x<(hsv->width);x++)
{

if(ptr[3*x]>178||(ptr[3*x]<5)&&ptr[3*x+1]>70&&ptr[3*x+2]>70)
{
ptr[3*x+2]=0;
}
else
{ptr[3*x+1]=0;ptr[3*x+2]=100;}
}
}
cvCvtColor(hsv,frame,CV_HSV2BGR);

cvCvtColor(frame,colordet,CV_BGR2GRAY);
cvCanny(gray_frame,gray_frame,50.0,25.0,3.0);
cvMorphologyEx(colordet,colordet,temp,NULL,CV_MOP_GRADIENT,1);
cvSmooth(colordet,colordet,CV_GAUSSIAN,5,5,1,1);
cvShowImage("HOUGHCIRCLE",gray_frame);
//cvErode(colordet,colordet,NULL,3);
//cvDilate(colordet,colordet,ker,3);
//cvCvtColor(colordet,gray_frame,CV_BGR2GRAY);
		CvSeq* circles=cvHoughCircles(colordet,storage,CV_HOUGH_GRADIENT,1.0,70.0,25.0,50.0,50,(frame->height)/2);
		//printf("%d\n",circles->total);
		for(i = 0; i < circles->total; i++ ) 
		{
			float* p = (float*) cvGetSeqElem( circles, i );
			CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
			cvCircle(colordet,pt,cvRound( p[2] ),CV_RGB(0x00,0x00,0xff),2,8,0);
	
		
		
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

cvShowImage("ColorDet",colordet);
	cvClearSeq(circles);
	//cvShowImage("HOUGHCIRCLE",frame);
c=cvWaitKey(10);
if(c==27) break;
	}

}

