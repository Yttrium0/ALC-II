#include <stdio.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <math.h>

float correspLevel(IplImage *img1, IplImage *img2, int height, int width);

int CAMSensor()
{
    IplImage *image;
    IplImage *gray1;
    IplImage *gray2;
    CvCapture *capture;
    float corresp;
    int mouv = 0;
    capture = cvCreateCameraCapture(CV_CAP_ANY);
    
    image = cvQueryFrame(capture);
    CvSize dim = cvGetSize(image);

    gray1 = cvCreateImage(dim,IPL_DEPTH_8U,1);
    gray2 = cvCreateImage(dim,IPL_DEPTH_8U,1);

    cvCvtColor(image, gray1, CV_BGR2GRAY);
    image = cvQueryFrame(capture);
    cvCvtColor(image, gray2, CV_BGR2GRAY);

    if(correspLevel(gray1, gray2, (int)dim.height, (int)dim.width) < 85){
        return 1;
    }

    cvReleaseCapture(&capture);
    cvFree(&image);
    cvFree(&gray1);
    cvFree(&gray2);
    
    return 0;
}

float correspLevel(IplImage *img1, IplImage *img2, int height, int width){
    float nbPixDiff = 0;
    float nbPixTot = height*width;
    unsigned char *pixImg1, *pixImg2, *pixFinal;
    
    for(int k = 0; k != height; k++){
        for (int i = 0; i != width; i++){
            pixImg1 = cvPtr2D(img1, k, i, NULL);
            pixImg2 = cvPtr2D(img2, k, i, NULL);
            
            if(abs(*pixImg1 - *pixImg2) < 30){
                nbPixDiff++;
            }  
        }
    }
    return ((nbPixDiff/nbPixTot)-0.99)*10000;
}