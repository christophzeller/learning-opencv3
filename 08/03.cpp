/*
3. Create a program that reads in and displays an image.
a. Allow the user to select a rectangular region in the image by drawing a rectangle
with the mouse button held down, and highlight the region when the
mouse button is released. Be careful to save an image copy in memory so that
your drawing into the image does not destroy the original values there. The
next mouse click should start the process all over again from the original
image.
b. In a separate window, use the drawing functions to draw a graph in blue,
green, and red that represents how many pixels of each value were found in
the selected box. This is the color histogram of that color region. The x-axis
should be eight bins that represent pixel values falling within the ranges 0–31,
32–63,..., 223–255. The y-axis should be counts of the number of pixels that
were found in that bin range. Do this for each color channel, BGR.
*/

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using std::cout; using std::endl; using std::string;

class HistogramViewer
{
public:
    HistogramViewer(string);
    virtual ~HistogramViewer();
    
    void show();
    static void on_mouse_event(int event, int x, int y, int flags, void *userdata);
    
private:
    Mat image;
    Mat display_image;
    Mat color_histogram_region;
    string filename;
    
    void calculate_bins();
    void show_histogram();
    void on_mouse_pressed(int event, int x, int y, int flags, void *userdata);
    void on_mouse_released(int event, int x, int y, int flags, void *userdata);
};

HistogramViewer::HistogramViewer(string filename) :
 filename(filename)
{
    namedWindow("Image");
    setMouseCallback("Image", HistogramViewer::on_mouse_event, this);
}

HistogramViewer::~HistogramViewer()
{
    destroyWindow("Image");
}

void HistogramViewer::show()
{
    image = imread(filename);
    display_image = image.clone();
    color_histogram_region;
    
    imshow("Image", image);
    
    waitKey(0);
}

void HistogramViewer::on_mouse_pressed(int event, int x, int y, int flags, void *userdata)
{
    cout << "button down fired..." << endl;
}

void HistogramViewer::on_mouse_released(int event, int x, int y, int flags, void *userdata)
{    
    cout << "button up fired..." << endl;
}

void HistogramViewer::on_mouse_event(int event, int x, int y, int flags, void *userdata)
{
    HistogramViewer* p = static_cast<HistogramViewer*>(userdata);
    switch (event)
    {
    case EVENT_LBUTTONDOWN:
        p->on_mouse_pressed(event, x, y, flags, userdata);
    break;
    case EVENT_LBUTTONUP:
        p->on_mouse_released(event, x, y, flags, userdata);
    break;
    }
}


/*void on_mouse_pressed(int event, int x, int y, int flags, void *userdata)
{
    cout << "button down fired..." << endl;
}

void on_mouse_released(int event, int x, int y, int flags, void *userdata)
{    
    cout << "button up fired..." << endl;
}

void on_mouse_event(int event, int x, int y, int flags, void *userdata)
{
    switch (event)
    {
    case EVENT_LBUTTONDOWN:
        on_mouse_pressed(event, x, y, flags, userdata);
    break;
    case EVENT_LBUTTONUP:
        on_mouse_released(event, x, y, flags, userdata);
    break;
    }
}

void no_class(char** argv)
{
    Mat image = imread(argv[1]);
    Mat display_image = image.clone();
    Mat color_histogram_region;
    
    namedWindow("Image");
    setMouseCallback("Image", on_mouse_event);
    
    imshow("Image", image);
    
    waitKey(0);
    destroyWindow("Image");
}*/

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "expects exactly one argument: filename" << endl;
    }
    
    //no_class(argv);
    HistogramViewer v { argv[1] };
    v.show();

    return 0;
}
