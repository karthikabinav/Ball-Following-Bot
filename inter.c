#include "cv.h"
#include "highgui.h"
 
int
main(int argc, char** argv)
{
    IplImage* src, * res, * roi;
 
    /* usage: <prog_name> <image> */
  
 
    src = cvLoadImage("a.png", 1);
    res = cvCreateImage(cvGetSize(src), 8, 3);
    roi = cvCreateImage(cvGetSize(src), 8, 1);
 
    /* prepare the 'ROI' image */
    cvZero(roi);
 
    /* Note that you can use any shape for the ROI */
    cvCircle(
        roi,
        cvPoint(130, 100),
        50,
        CV_RGB(255, 255, 255),
        -1, 8, 0
    );
 
    /* extract subimage */
    cvAnd(src, src, res, roi);
 
    /*
     * do the main processing with subimage here.
     * in this example, we simply invert the subimage
     */
    cvNot(res, res);
 
    /* 'restore' subimage */
    IplImage* roi_C3 = cvCreateImage(cvGetSize(src), 8, 3);
    cvMerge(roi, roi, roi, NULL, roi_C3);
    cvAnd(res, roi_C3, res, NULL);
 
    /* merge subimage with original image */
    cvNot(roi, roi);
    cvAdd(src, res, res, roi);
 
    /* show result */
    cvNamedWindow("hi", 1);
    cvNamedWindow("res", 1);
    cvShowImage("hi", src);
    cvShowImage("res", res);
 
    cvWaitKey(0);
 
    /* be tidy */
cvDestroyWindow( "res" );
    cvReleaseImage(&src);
    cvReleaseImage(&res);
    cvReleaseImage(&roi);
 
    return 0;
}
 
