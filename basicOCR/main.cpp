#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <ctype.h>
#include "basicOCR.h"
#endif

IplImage* imagen;
int red,green,blue;
IplImage* screenBuffer;
int drawing;
int r,last_x, last_y;

void draw(int x,int y){
	//Draw a circle where is the mouse
	cvCircle(imagen, cvPoint(x,y), r, CV_RGB(red,green,blue), -1, 4, 0);
	//Get clean copy of image
	screenBuffer=cvCloneImage(imagen);
	cvShowImage( "Demo", screenBuffer );
}

void drawCursor(int x, int y){
	//Get clean copy of image
	screenBuffer=cvCloneImage(imagen);
	//Draw a circle where is the mouse
	cvCircle(screenBuffer, cvPoint(x,y), r, CV_RGB(0,0,0), 1, 4, 0);
}


/*************************
* Mouse CallBack
* event: 
*	#define CV_EVENT_MOUSEMOVE      0
*	#define CV_EVENT_LBUTTONDOWN    1
*	#define CV_EVENT_RBUTTONDOWN    2
*	#define CV_EVENT_MBUTTONDOWN    3
*	#define CV_EVENT_LBUTTONUP      4
*	#define CV_EVENT_RBUTTONUP      5
*	#define CV_EVENT_MBUTTONUP      6
*	#define CV_EVENT_LBUTTONDBLCLK  7
*	#define CV_EVENT_RBUTTONDBLCLK  8
*	#define CV_EVENT_MBUTTONDBLCLK  9
*
* x, y: mouse position
*
* flag:
*	#define CV_EVENT_FLAG_LBUTTON   1
*	#define CV_EVENT_FLAG_RBUTTON   2
*	#define CV_EVENT_FLAG_MBUTTON   4
*	#define CV_EVENT_FLAG_CTRLKEY   8
*	#define CV_EVENT_FLAG_SHIFTKEY  16
*	#define CV_EVENT_FLAG_ALTKEY    32
*
**************************/

void on_mouse( int event, int x, int y, int flags, void* param )
{
	last_x=x;
	last_y=y;
	drawCursor(x,y);
    //Select mouse Event
	if(event==CV_EVENT_LBUTTONDOWN)
        {
			drawing=1;
			draw(x,y);
		}
    else if(event==CV_EVENT_LBUTTONUP)
		{
			//drawing=!drawing;
			drawing=0;
		}
	else if(event == CV_EVENT_MOUSEMOVE  &&  flags & CV_EVENT_FLAG_LBUTTON)
		{
			if(drawing)
				draw(x,y);
		}
}



int main( int argc, char** argv )
{
    printf( "Basic OCR by David Millan Escriva | Damiles\n"
		"Hot keys: \n"
	"\tr - reset image\n"
	"\t+ - cursor radio ++\n"
	"\t- - cursor radio --\n"
	"\ts - Save image as out.png\n"
	"\tc - Classify image, the result in console\n"
        "\tESC - quit the program\n");
	drawing=0;
	r=10;
	red=green=blue=0;
	last_x=last_y=red=green=blue=0;
	//Create image
	imagen=cvCreateImage(cvSize(128,128),IPL_DEPTH_8U,1);
	//Set data of image to white
	cvSet(imagen, CV_RGB(255,255,255),NULL);
	//Image we show user with cursor and other artefacts we need
	screenBuffer=cvCloneImage(imagen);
	
	//Create window
    	cvNamedWindow( "Demo", 0 );

	cvResizeWindow("Demo", 128,128);
	//Create mouse CallBack
	cvSetMouseCallback("Demo",&on_mouse, 0 );


	//////////////////
	//My OCR
	//////////////////
	basicOCR ocr;
	
	//Main Loop
    for(;;)
    {
		int c;

        cvShowImage( "Demo", screenBuffer );
        c = cvWaitKey(10);
        if( (char) c == 27 )
            break;
	if( (char) c== '+' ){
		r++;
		drawCursor(last_x,last_y);
	}
	if( ((char)c== '-') && (r>1) ){
		r--;
		drawCursor(last_x,last_y);
	}
	if( (char)c== 'r'){
		cvSet(imagen, cvRealScalar(255),NULL);
		drawCursor(last_x,last_y);
	}
	if( (char)c== 's'){
		cvSaveImage("out.png", imagen);
	}
	if( (char)c=='c'){
		ocr.classify(imagen,1);
	}
		
    }

    cvDestroyWindow("Demo");

    return 0;
}

#ifdef _EiC
main(1,"mouseEvent.c");
#endif
