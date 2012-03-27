#include <highgui.h>

#include <cv.h>

 

int main(int argc, char *argv[])

{

    // Variable to store the keyboard input
//011
    char d;
//012
 
//013
    // Initialize flags to false. These flags are used for keyboard input (keys q, r, g and b)
//014
    bool Q = false;

    bool R = false;

    bool G = false;

    bool B = false;

 
   CvCapture* capture = cvCaptureFromCAM(0);

    IplImage *src_img;

    IplImage *histogram;

    IplImage *disp_img;

    IplImage *gray_img;

    IplImage *red_img, *red_histogram;

    IplImage *green_img, *green_histogram;

    IplImage *blue_img, *blue_histogram;

    int hist_size = 256;

    float range[] = {0,256};

    float *ranges[] = {range};

 

    /* cvCreateHist function is used to create the histograms. Following is the explanation of the various arguments of the function

    1. First argument: Defines how many dimensions the histogram will have. To keep things simple this tutorial only uses one dimensional histograms.
038
    2. Second argument: Defines the size of the dimensions. If we were using higher dimensional histograms we would have to pass an array of sizes, one for each dimension, but we are only using one dimension so the array we pass only has one value in it. We choose to make the size of the dimension 256 because the depth of the images we are working with is 8 bit, meaning that we will get one bin for each intensity level. We could have chosen to have less bins but the histogram would have less resolution. We could have chosen more bins but we would not show any more information.
039
    3. Third argument: Tells OpenCV how to store the data in the histogram. There are two options for this: CV_HIST_ARRAY and CV_HIST_SPARSE. The second option is useful for storing multidimensional histograms that will have most of their bins with counts of zero. Since we are using a one dimensional histogram, we don’t expect sparse data so we choose the first option.
040
    4. Fourth argument: Used to specify the range of values for each dimension.  Each range is stored as an array of two values, the minimum and maximum value for that dimension. Again we are using one dimension so it looks a bit redundant to make an array of arrays, but only with one array. We have to do this though, because this fourth argument expects an array to 2 value arrays. It is important to know how OpenCV creates the bins based on the range values and the histogram size. OpenCV will take the value supplied in ranges and break it into as many sub intervals as defined in hist_size.
041
    5. Fifth argument: Defines if the sub intervals are to be split uniformly or not. For this case we definitely want each bin to be the same width as the others so we choose this argument to be 1.
042
    */

    CvHistogram* hist_red = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);

    CvHistogram* hist_blue = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);

    CvHistogram* hist_green = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);

    double max_value = 0;

    double max_value_red = 0;

    double max_value_green = 0;

    double max_value_blue = 0;

    double find_max = 0;

 

    cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);

    cvMoveWindow("mainWin", 5, 5);

    // "histogramWin" shows the histogram

    cvNamedWindow("histogramWin", CV_WINDOW_AUTOSIZE);

    cvMoveWindow("histogramWin", 435, 5);



    printf("RGB Histogram\n\n");

    printf("To toggle red channel ON/OFF press: 'r'\n");

    printf("To toggle green channel ON/OFF press: 'g'\n");

    printf("To toggle blue channel ON/OFF press: 'b'\n");

    printf("To quit press: 'q'\n");

 


    while(1)

    {
        // Clear all max values to 0

        max_value = 0; max_value_red = 0; max_value_green = 0; max_value_blue = 0;

 


        histogram = cvLoadImage( "histogram_scale.png" );

        red_histogram = cvCreateImage( cvGetSize(histogram), IPL_DEPTH_8U, 3 );

        green_histogram = cvCreateImage( cvGetSize(histogram), IPL_DEPTH_8U, 3 );

        blue_histogram = cvCreateImage( cvGetSize(histogram), IPL_DEPTH_8U, 3 );

 

        src_img=cvQueryFrame(capture);

        disp_img = cvCreateImage(cvSize((src_img->width)/1.6,(src_img->height)/1.6),IPL_DEPTH_8U,3);

        cvResize(src_img,disp_img,CV_INTER_LINEAR);

 

        // Create 3 single channel images to store each channels data and split the source image into the RGB channels.

        /* Note that each channel is given an image with the same size (width and height) as the source. The depth is 8 bits and these new images have only one channel (since they are storing only one channel). The function cvCvtPixToPlane actually separates the source into its three channels and stores them in the images we just created.*/
090
        blue_img = cvCreateImage( cvGetSize(src_img), IPL_DEPTH_8U, 1 );
091
        green_img = cvCreateImage( cvGetSize(src_img), IPL_DEPTH_8U, 1 );
092
        red_img = cvCreateImage( cvGetSize(src_img), IPL_DEPTH_8U, 1 );
093
        cvCvtPixToPlane( src_img, blue_img, green_img, red_img, 0 );
094
 
095
        // Calculate a histogram for each channel.
096
        /*The first argument takes the image we would like to calculate the histogram for. Note that cvCalcHist can only take a one channel image. The next argument says which histogram will be populated. The third argument turns on or off accumulator mode. Since we want the histogram to update for each frame we want to clear the contents before adding new ones and the accumulator mode should be off. The final parameter is a mask that can tell cvCalcHist to unly calculate a certain part of the image. By setting the mask null we calculate for the whole image.*/
097
        cvCalcHist( &red_img, hist_red, 0, NULL );
098
        cvCalcHist( &blue_img, hist_blue, 0, NULL );
099
        cvCalcHist( &green_img, hist_green, 0, NULL );
100
 
101
        // Search through the histograms for their maximum value and store it.
102
        /* The code for finding the maximum value for the red channel is shown above. The other two channels are very similar and shown in the source code. It is done with a simple for loop that checks every value in each bin. The function used to get the values out of the histogram is cvQueryHistValue_1D. This function is made for getting values out of a one dimensional histogram. Its arguments are the histogram you want to look into, and the bin number you want to see. The reason we need to find this max value is that we will scale the y-axis of the histogram so that it fits on the screen. We do this by dividing each bin value by the max value (this is done in the next step).*/
103
        for( int i = 0; i < hist_size; i++ )
104
        {
105
            find_max = cvQueryHistValue_1D(hist_red,i);
106
            if (find_max > max_value_red)
107
            {
108
                max_value_red = find_max;
109
            }
110
        }
111
        for( int i = 0; i < hist_size; i++ )
112
        {
113
            find_max = cvQueryHistValue_1D(hist_green,i);
114
            if (find_max > max_value_green)
115
            {
116
                max_value_green = find_max;
117
            }
118
        }
119
        for( int i = 0; i < hist_size; i++ )
120
        {
121
            find_max = cvQueryHistValue_1D(hist_blue,i);
122
            if (find_max > max_value_blue)
123
            {
124
                max_value_blue = find_max;
125
            }
126
        }
127
        // The largest value in all the histograms is found.
128
        max_value = max(max(max_value_red,max_value_green),max_value_blue);
129
 
130
        // Draw the histogram for each channel, if the flag for that channel is set
131
        /* First we see an if statement that controls whether or not we draw the histogram. It is based on a flag that is set in the next step by user input. This allows the user to select which channel or combination of channels that they want to see. The next step is the scaling. The function cvScale is used. Its first argument is the source to be scaled and the second is the destination for result. Here we used the same place for source and destination meaning that the bins are scaled and then stored back in the same place. The last argument is the scale factor. A factor of 438/max_value was used because the highest bar that we want to draw is 438 pixels high (so it does not go out of the bounds of the picture we used for the axis). Inside the for loop we see a complicated line of code that draws all the rectangles. The function cvRectangle is used for this. It requires that we pass it which image to draw on, two points to define the rectangles, the colour and line thickness. We specify line thickness as -1 which means a filled rectangle. The last function used is cvAdd which is used to simplify drawind the multiple histograms with overlap. By adding colours on top of each other no extar code is required to deal with the overlap.*/
132
        if (R)
133
        {
134
            cvScale( hist_red->bins, hist_red->bins, 438/max_value);
135
            for( int i= 0; i < hist_size; i++ )
136
            {
137
                cvRectangle( red_histogram, cvPoint(i*3+ 15, 448),cvPoint(i*3+16, 448 - cvRound(cvQueryHistValue_1D(hist_red,i))),cvScalar(0x00,0x00,0xff,0), -1);
138
            }
139
            cvAdd(histogram,red_histogram,histogram,0);
140
        }
141
        if (G)
142
        {
143
            cvScale( hist_green->bins, hist_green->bins, 438/max_value);
144
            for( int i= 0; i < hist_size; i++ )
145
            {
146
                cvRectangle( green_histogram, cvPoint(i*3+ 15, 448),cvPoint(i*3+16, 448 - cvRound(cvQueryHistValue_1D(hist_green,i))),cvScalar(0x00,0xff,0x00,0), -1);
147
            }
148
            cvAdd(histogram,green_histogram,histogram,0);
149
        }
150
        if (B)
151
        {
152
            cvScale( hist_blue->bins, hist_blue->bins, 438/max_value);
153
            for( int i= 0; i < hist_size; i++ )
154
            {
155
                cvRectangle( blue_histogram, cvPoint(i*3+ 15, 448),cvPoint(i*3+16, 448 - cvRound(cvQueryHistValue_1D(hist_blue,i))),cvScalar(0xff,0x00,0x00,0), -1);
156
            }
157
            cvAdd(histogram,blue_histogram,histogram,0);
158
        }
159
 
160
        // Show the images in the windows
161
        cvShowImage("mainWin", disp_img);
162
        cvShowImage("histogramWin", histogram);
163
 
164
        // Set flags
165
        d=cvWaitKey(15);
166
        /* A simple case statement takes the input from the keyboard and sets the flags accordingly. The R, G and B flags are XOR’ed with 1 to change state each time r, g, or b is pressed. This makes r g and b into toggle switches.*/
167
        switch (d)
168
        {
169
            case 'r':   R = R^1;    break;
170
            case 'g':   G = G^1;    break;
171
            case 'b':   B = B^1;    break;
172
            case 'q':   Q = true;   break;
173
            default:    break;
174
        }
175
        if(Q)break;     //quit program
176
 
177
        // Release the images that we created
178
        cvReleaseImage(&disp_img );
179
        cvReleaseImage(&red_img );
180
        cvReleaseImage(&green_img );
181
        cvReleaseImage(&blue_img );
182
        cvReleaseImage(&red_histogram );
183
        cvReleaseImage(&green_histogram );
184
        cvReleaseImage(&blue_histogram );
185
        cvReleaseImage(&histogram );
186
    }
187
    return 0;
188
}

