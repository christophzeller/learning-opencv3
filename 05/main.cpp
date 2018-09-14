#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using std::cout; using std::cin; using std::endl; using std::vector;
using namespace cv;

void show_image_in_window(const char* window_name, const Mat& image)
{
    namedWindow(window_name, WINDOW_AUTOSIZE);
    imshow(window_name, image);
    waitKey(0);
    destroyWindow(window_name);
}

/* 1. This exercise will accustom you to the idea of many functions taking matrix
types. Create a two-dimensional matrix with three channels of type byte with
data size 100 × 100. Set all the values to 0.
a. Draw a circle in the matrix using void cv::circle(InputOutputArray img,
cv::point center, intradius, cv::Scalar color, int thickness=1,
int line_type=8, int shift=0).
b. Display this image using methods described in Chapter 2.
*/

void _1()
{
    Mat image = Mat::zeros(100, 100, CV_8UC3); // does not like auto image = ...
    circle(image, Point {50, 50}, 15, Scalar {200, 20, 210}, 2);
    
    namedWindow("ch5_1b", WINDOW_AUTOSIZE);
    imshow("ch5_1b", image);
    waitKey(0);
    destroyWindow("ch5_1b");
}


/*
2. Create a two-dimensional matrix with three channels of type byte with data size
100 × 100, and set all the values to 0. Use the cv::Mat element access functions to
modify the pixels. Draw a green rectangle between (20, 5) and (40, 20).
*/
void _2()
{
    Mat image = Mat::zeros(100, 100, CV_8UC(3));
    for (auto i = 20; i < 40; ++i)
    {
        for (auto j = 5; j < 20; ++j)
        {
            image.at<Vec3b>(j, i)[0] = 15;
            image.at<Vec3b>(j, i)[1] = 225;
            image.at<Vec3b>(j, i)[2] = 40;
            //cout << i << " " << j << " " << x[0] << endl;
        }
    }
    
    namedWindow("ch5_2", WINDOW_AUTOSIZE);
    imshow("ch5_2", image);
    waitKey(0);
    destroyWindow("ch5_2");
}

/*
3. Create a three-channel RGB image of size 100 × 100. Clear it. Use pointer arithmetic
to draw a green square between (20, 5) and (40, 20).
*/

/*
4. Practice using region of interest (ROI). Create a 210 × 210 single-channel byte
image and zero it. Within the image, build a pyramid of increasing values using
ROI and cv::Mat::setTo(). That is: the outer border should be 0, the next inner
border should be 20, the next inner border should be 40, and so on until the final
innermost square is set to value 200; all borders should be 10 pixels wide. Display
the image.
// note to self: height map of a pyramid, i guess.
*/

void _4()
{
    const char* window_name = "ch5_4";
    Mat image = Mat::zeros(210, 210, CV_8UC1); 
    
    for (auto i = 0; i < 11; ++i)
    {
        Rect r (10*i, 10*i, 210-20*i, 210-20*i);
        Mat roi(image, r);
        roi.setTo(20*i);
    }
    
    namedWindow(window_name, WINDOW_AUTOSIZE);
    imshow(window_name, image);
    waitKey(0);
    destroyWindow(window_name);
}

/*
5. Use multiple headers for one image. Load an image that is at least 100 × 100.
Create two additional headers that are ROIs where width = 20 and the height =
30. Their origins should be at (5, 10) and (50, 60), respectively. Pass these new
image subheaders to cv::bitwise_not(). Display the loaded image, which
should have two inverted rectangles within the larger image.
*/

void _5(char *filename)
{
    Mat image = imread(filename, -1);
    if (image.empty())
        return;
        
    const char* window_name = "ch5_5";
    
    Mat roi1(image, Rect(5, 10, 20, 30));
    Mat roi2(image, Rect(50, 60, 20, 30));
    Mat roi3(image, Rect(360, 40, 20, 30));
        
    bitwise_not(roi1, roi1);
    bitwise_not(roi2, roi2);
    bitwise_not(roi3, roi3);
    
    namedWindow(window_name, WINDOW_AUTOSIZE);
    imshow(window_name, image);
    waitKey(0);
    destroyWindow(window_name);
    
}

/*
6. Create a mask using cv::compare(). Load a real image. Use cv::split() to split
the image into red, green, and blue images.
a. Find and display the green image.
b. Clone this green plane image twice (call these clone1 and mask).
c. Find the green plane’s minimum and maximum value.
d. Set clone1’s values to thresh = (unsigned char)((maximum - minimum)/
2.0).
e. Set mask to 0 and use cv::compare (green_image, clone1, mask,
cv::CMP_GE). Now mask will have a mask of where the value exceeds
thresh in the green image.
f. Finally, use cv::subtract (green_image,thresh/2, green_image,
mask) and display the results.
*/

void _6(char* filename)
{
    const char* window_name = "ch5_6";
    Mat image = imread(filename, -1);
    if (image.empty())
        return;
        
    vector<Mat> channels;
    split(image, channels);
    
    cout << channels.size() << " channels split" << endl;
    
    show_image_in_window("Raw", image);
    
    double min;
    double max;
    
    minMaxLoc(channels[1], &min, &max, nullptr, nullptr);
    
    auto threshold = (unsigned char)((max - min) / 2.0);
    
    cout << "threshold: " << (int)threshold << endl;
    
    Mat clone1 = channels[1].clone();
    clone1.setTo(threshold);
    
//    show_image_in_window("G-mask", clone1);
    Mat mask = channels[1].clone();
    mask.setTo(0);
    
    compare(channels[1], clone1, mask, CMP_GE);
    subtract(channels[1], threshold/2, channels[1], mask);

    show_image_in_window("mask", mask);
    show_image_in_window("G-channel masked", channels[1]);
    show_image_in_window("Result full", image);
}




int main(int argc, char** argv)
{
//    _1();
//    _2();
//    _4();
    if (argc < 2)
    {
        char* filename = "../assets/penguin.jpg";
        //_5(filename);
        _6(filename);
    }
    else
    {
        //_5(argv[1]);
        _6(argv[1]);
    }
    return 0;
}
