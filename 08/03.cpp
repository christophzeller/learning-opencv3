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
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using std::cout; using std::endl; using std::string; using std::vector;

class HistogramViewer
{
public:
    HistogramViewer(string);
    virtual ~HistogramViewer();
    
    void show();
    static void on_mouse_event(int event, int x, int y, int flags, void *userdata);
    
    typedef vector<vector<int>> histogram_bins;
    
private:
    Mat image;
    Mat display_image;
    
    Rect mouse_drag_region;
    string filename;
    string image_window_name = "Image";
    
    histogram_bins calculate_bins(const Mat&);
    void show_histogram();
    void on_mouse_pressed(int event, int x, int y, int flags, void *userdata);
    void on_mouse_released(int event, int x, int y, int flags, void *userdata);
};

HistogramViewer::HistogramViewer(string filename) :
 filename(filename)
{
    namedWindow(image_window_name);
    setMouseCallback(image_window_name, HistogramViewer::on_mouse_event, this);
}

HistogramViewer::~HistogramViewer()
{
    destroyWindow(image_window_name);
}

void HistogramViewer::show()
{
    image = imread(filename);
    display_image = image.clone();
    //color_histogram_region;
    
    imshow(image_window_name, display_image);
    
    waitKey(0);
}

void HistogramViewer::show_histogram()
{
    Mat color_histogram_region (image, mouse_drag_region);
    rectangle(display_image, mouse_drag_region, Scalar(175, 25, 220));
    imshow(image_window_name, display_image);
    
    auto bins = calculate_bins(color_histogram_region);
    
    int i = 0;
    
    for (const auto& color_bin : bins)
    {
        cout << "channel " << i << endl;
        int j = 0;
        for (const auto& value : color_bin)
        {
            cout << j*32 << "-" << (j+1)*32-1 << ": " << value << endl;
            ++j;
        }
        ++i;
    }
}

HistogramViewer::histogram_bins HistogramViewer::calculate_bins(const Mat& region)
{
    // TODO: get rid of magic numbers for channels, bin width & amount
    HistogramViewer::histogram_bins bins;
    bins.push_back(vector<int> (8, 0));
    bins.push_back(vector<int> (8, 0));
    bins.push_back(vector<int> (8, 0));
    
    for (MatConstIterator_<Vec3b> it = region.begin<Vec3b>(); it != region.end<Vec3b>(); ++it)
    {
        //cout << *it << endl;
        Vec3b v = *it;
        auto b = v[0], g = v[1], r = v[2];
        
        auto b_bin = b / 32;
        auto g_bin = g / 32;
        auto r_bin = r / 32;
        
        bins[0][b_bin]++;
        bins[1][g_bin]++;
        bins[2][r_bin]++;
    }
    
    return bins;
}

void HistogramViewer::on_mouse_pressed(int event, int x, int y, int flags, void *userdata)
{
    image.copyTo(display_image);
    //cout << "button down fired..." << endl;
    mouse_drag_region.x = x;
    mouse_drag_region.y = y;
}

void HistogramViewer::on_mouse_released(int event, int x, int y, int flags, void *userdata)
{    
    //cout << "button up fired..." << endl;
    // TODO: this only supports topLeft->bottomRight dragging for now. exceptions! yay!
    mouse_drag_region.height = y - mouse_drag_region.y;
    mouse_drag_region.width = x - mouse_drag_region.x;
    
    cout << mouse_drag_region << endl;
    
    show_histogram();
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
