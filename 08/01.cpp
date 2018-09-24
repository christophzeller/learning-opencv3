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



int main(int argc, char** argv)
{
    // argv[1] == filename to video
    if (argc != 2)
    {
        cout << "expects exactly one argument: path to video file" << endl;
        return -1;
    }
    
    VideoCapture cap;
    cap.open(argv[1]);
    if (!cap.isOpened())
    {
        cout << "could not open video file: " << argv[1] << endl;
    }
    
    const char* canny_window = "Canny";
    const char* raw_window = "Raw";
    const char* grayscale_window = "Grayscale";
    
    namedWindow(raw_window, WINDOW_AUTOSIZE);
    namedWindow(grayscale_window, WINDOW_AUTOSIZE);
    namedWindow(canny_window, WINDOW_AUTOSIZE);
    
    Mat frame;
    Mat grayscale;
    Mat canny;
    while (cap.read(frame))
    {
        cvtColor(frame, grayscale, COLOR_BGR2GRAY); // TODO: dynamically determine COLOR_...2GRAY?
        Canny(grayscale, canny, 10, 100); // TODO: good threshold values?

        imshow(raw_window, frame);
        imshow(grayscale_window, grayscale);
        imshow(canny_window, canny);
        
        waitKey(20);        
    }
    
    destroyWindow(raw_window);
    destroyWindow(grayscale_window);
    destroyWindow(canny_window);
}
