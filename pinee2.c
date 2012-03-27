is it a #include<highgui.h>
#include<cv.h>
using namespace LibSerial;
using namespace std;

void main()
{
IplImage *frame,*hsv,*colordet,*temp,*gray_frame;
int x,y,z,i;
char c;
IplConvKernel* ker=cvCreateStructuringElementEx(7,7,3,3,CV_SHAPE_ELLIPSE,NULL);
cvNamedWindow("HOUGHCIRCLE",CV_WINDOW_AUTOSIZE);
CvMemStorage *storage=cvCreateMemStorage(0);
CvCapture *capture=cvCreateCameraCapture(0);
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
	
		}
cvShowImage("ColorDet",colordet);
	cvClearSeq(circles);
	//cvShowImage("HOUGHCIRCLE",frame);
c=cvWaitKey(10);
if(c==27) break;
	}

}

