/*

Create a program that reads in and displays an image. When the user’s mouse
clicks on the image, read in the corresponding pixel values (blue, green, red) and
write those values as text to the screen at the mouse location.
a. For the program of Exercise 2, display the mouse coordinates of the individual
image when clicking anywhere within the three-image display.

*/

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using std::cout; using std::endl; using std::string;

void on_click(int event, int x, int y, int flags, void* image)
{
    if (event != EVENT_LBUTTONDOWN)
        return;
        
    Mat* p_image = static_cast<Mat*>(image);
    auto pixel = p_image->at<Vec3b>(y, x);
    String value;
    value << format(pixel, Formatter::FMT_C);
    //cout << value << endl;
    putText(*p_image, value, Point(x, y), FONT_HERSHEY_PLAIN, 1.0, Scalar(170, 15, 225), 2);
    cout << pixel << endl;
    
    imshow("8.2", *p_image);
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "expects exactly one argument: filename" << endl;
        return -1;
    }
    
    Mat image = imread(argv[1]);
    if (image.empty())
    {
        cout << "provided file is not an image" << endl;
        return -2;
    }
    
    namedWindow("8.2");
    setMouseCallback("8.2", on_click, &image);
    imshow("8.2", image);
    waitKey(0);
    destroyWindow("8.2");
    return 0;
}
