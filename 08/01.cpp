/*

1. Create a program that (1) reads frames from a video, (2) turns the result to grayscale,
and (3) performs Canny edge detection on the image. Display all three
stages of processing in three different windows, with each window appropriately
named for its function.
a. Display all three stages of processing in one image. (Hint: create another
image of the same height but three times the width as the video frame. Copy
the images into this, either by using pointers or (more cleverly) by creating
three new image headers that point to the beginning of and to one-third and
two-thirds of the way into the imageData. Then use Mat::copyTo().)

b. Write appropriate text labels describing the processing in each of the three
slots.

*/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>


using std::cout; using std::endl;
using namespace cv;

const char* canny_window = "Canny";
const char* raw_window = "Raw";
const char* grayscale_window = "Grayscale";
const char* composite_window = "Composite";

struct Images
{
    Mat raw;
    Mat grayscale;
    Mat canny;
};

void show_separate(const Images& stuff)
{
    imshow(raw_window, stuff.raw);
    imshow(grayscale_window, stuff.grayscale);
    imshow(canny_window, stuff.canny);
}

void show_composite(Images& stuff)
{
    Mat composite_image ( stuff.raw.rows, stuff.raw.cols*3, stuff.raw.type() );
    
    cvtColor(stuff.grayscale, stuff.grayscale, COLOR_GRAY2BGR);
    cvtColor(stuff.canny, stuff.canny, COLOR_GRAY2BGR);
    
    Mat roi_raw     ( composite_image, Rect { 0, 0, stuff.raw.cols, stuff.raw.rows} );
    Mat roi_gs      { composite_image, Rect { stuff.raw.cols, 0, stuff.raw.cols, stuff.raw.rows} };
    Mat roi_canny   { composite_image, Rect { stuff.raw.cols*2, 0, stuff.raw.cols, stuff.raw.rows} };
    
    stuff.raw.copyTo(roi_raw);
    stuff.grayscale.copyTo(roi_gs);
    stuff.canny.copyTo(roi_canny);
    
    /*putText(
        cv::Mat& img, // Image to be drawn on
        const string& text, // write this (often from cv::format)
        cv::Point origin, // Upper-left corner of text box
        int fontFace, // Font (e.g., cv::FONT_HERSHEY_PLAIN)
        double fontScale, // size (a multiplier, not "points"!)
        cv::Scalar color, // Color, RGB form
        int thickness = 1, // Thickness of line
        int lineType = 8, // Connectedness, 4 or 8
        bool bottomLeftOrigin = false // true='origin at lower left'
    );*/
    
    putText(roi_raw, "base image", Point{0, 25}, FONT_HERSHEY_PLAIN, 1.25, Scalar {170, 10, 200}, 2);
    putText(roi_gs, "grayscale", Point{0, 25}, FONT_HERSHEY_PLAIN, 1.25, Scalar {170, 10, 200}, 2);
    putText(roi_canny, "canny edge detection", Point{0, 25}, FONT_HERSHEY_PLAIN, 1.25, Scalar {170, 10, 200}, 2);
    
    imshow(composite_window, composite_image);

}


int main(int argc, char** argv)
{
    // argv[1] == filename to video
    if (argc != 3)
    {
        cout << "expects exactly two argument: path to video file and mode [s|c]" << endl;
        return -1;
    }
    
    VideoCapture cap;
    cap.open(argv[1]);
    if (!cap.isOpened())
    {
        cout << "could not open video file: " << argv[1] << endl;
    }
    

    char mode = argv[2][0];
    
    switch (mode)
    {
    case 's':
        namedWindow(raw_window, WINDOW_AUTOSIZE);
        namedWindow(grayscale_window, WINDOW_AUTOSIZE);
        namedWindow(canny_window, WINDOW_AUTOSIZE);
    break;
    case 'c':
        namedWindow(composite_window, WINDOW_AUTOSIZE);
    break;
    }
    
    Mat frame;
    Mat grayscale;
    Mat canny;
    while (cap.read(frame))
    {
        cvtColor(frame, grayscale, COLOR_BGR2GRAY); // TODO: dynamically determine COLOR_...2GRAY?
        Canny(grayscale, canny, 10, 100); // TODO: good threshold values?
        
        Images frame_data;
        frame_data.raw = frame;
        frame_data.grayscale = grayscale;
        frame_data.canny = canny;

        switch (mode)
        {
        case 's': show_separate(frame_data); break;
        case 'c': show_composite(frame_data); break;
        }
        
        waitKey(20);        
    }
    
    switch (mode)
    {
    case 's':
        destroyWindow(raw_window);
        destroyWindow(grayscale_window);
        destroyWindow(canny_window);
        break;
    case 'c':
        destroyWindow(composite_window);
        break;
    }
}
