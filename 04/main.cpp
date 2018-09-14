#include <opencv2/core.hpp>
#include <iostream>

using std::cout; using std::cin; using std::endl;
using namespace cv;

/* 1. Create a 500 × 500 single channel uchar image with every pixel equal to zero.
a. Create an ASCII numeric typewriter where you can type numbers into your
computer and have the number show up in a 20-pixel-high by 10-pixel-wide
block. As you type, the numbers will display from left to right until you hit the
end of the image. Then just stop.
b. Allow for carriage return and backspace.
c. Allow for arrow keys to edit each number.
d. Create a key that will convert the resulting image into a color image, each
number taking on a different color.
*/

void _1()
{
    Mat image = Mat::zeros(500, 500, CV_8UC(1)); // CV_8U, _8UC1, _8UC(1)
}


/* 2. We want to create a function that makes it efficient to sum up rectangular
regions in an image by creating a statistics image where each “pixel” holds the
sum of the rectangle from that point to the image origin. These are called integral
images and by using just 4 points from the integral image, you can determine the
sum of any rectangle in the image.
a. Create a 100 × 200 single-channel uchar image with random numbers. Create
a 100 × 200 single-channel float “integral image” with all members equal to
zero.
b. Fill in each element of the integral image with the corresponding sum of the
rectangle from that pixel to the origin in the original uchar image.
c. How can you do part b) very efficiently in one pass using the integral numbers
you’ve already calculated in the integral image plus the new number
being added in the original image? Implement this efficient method.
d. Use the integral image to rapidly calculate the sum of pixels in any rectangle
in the original image.
e. How can you modify the integral image so that you can compute the sum of a
45-degree rotated rectangle in the original image very efficiently? Describe the
algorithm.
*/

Mat naive_integrals(const Mat& image)
{
    Mat integral_image;
    // for row, column: sum(
    return integral_image;
}

Mat fast_integrals(const Mat& image)
{
    Mat integral_image = Mat::zeros(image.rows, image.cols, CV_32FC1);
    
    for (auto row = 0; row < image.rows; ++row)
    {
        for (auto col = 0; col < image.cols; ++col)
        {
            // neighboring, already calculated integrals
            float left = 0.;
            float top = 0.;
            float top_left = 0.;
            float integral = image.at<unsigned char>(row, col);
            
            if (row > 0)
                top = integral_image.at<float>(row-1, col);
            if (col > 0)
                left = integral_image.at<float>(row, col-1);
            if ((col > 0) && (row > 0))
                top_left = integral_image.at<float>(row-1, col-1);
                
            integral += left + top - top_left;
            
            integral_image.at<float>(row, col) = integral;
        }
    }
    
    return integral_image;
}

float sub_integral(const Mat& integral_image, Rect r)
{
    auto top = r.y;
    auto bottom = r.y + r.height - 1;
    auto left = r.x;
    auto right = r.x + r.width - 1;
    
    float i_top = 0.f;
    float i_left = 0.f;
    float i_top_left = 0.f;
    
    // TODO: out of bounds checks for rect
    
    if (top > 0)
        i_top = integral_image.at<float>(top-1, right);
    if (left > 0)
        i_left = integral_image.at<float>(bottom, left-1);
    if ((top > 0) && (left > 0))
        i_top_left = integral_image.at<float>(top-1, left-1);
    
    float i = integral_image.at<float>(bottom, right);
    
    return i - i_top - i_left + i_top_left;
}

void _2(size_t rows, size_t cols, bool debug_output=true)
{
    Mat random_image;
    random_image.create(rows, cols, CV_8UC1);
    RNG rng;
    for (auto row = 0; row < random_image.rows; ++row)
    {
        for (auto col = 0; col < random_image.cols; ++col)
        {
            random_image.at<unsigned char>(row, col) = rng.uniform(0, 10);
        }
    }
    
//    random_image.forEach<unsigned char>([&](unsigned char* el, const int* position) { *el = rng.uniform(0, 10); }
//    );
    auto I = fast_integrals(random_image);
    if (debug_output)
        cout << random_image << endl << I << endl;
        
    auto si = sub_integral(I, { 0, 0, 3, 3 });
    cout << "0 0 3 3: " << si << endl;
    cout << "0 0 1 1: " << sub_integral(I, { 0, 0, 1, 1 }) << endl;
    cout << "1 1 1 1: " << sub_integral(I, { 1, 1, 1, 1 }) << endl;
    cout << "1 1 2 2: " << sub_integral(I, { 1, 1, 2, 2 }) << endl;
    cout << "0 1 3 2: " << sub_integral(I, { 0, 1, 3, 2 }) << endl;
}

int main(int argc, char** argv)
{
    _2(3, 3);
    //_2(100, 200, false);
    
    return 0;
}
