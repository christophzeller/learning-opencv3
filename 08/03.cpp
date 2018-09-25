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
using std::cout; using std::endl; using std::string; using std::vector; using std::ostream;
using std::to_string;

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
    string graph_window_name = "Color Histogram";
    
    histogram_bins calculate_bins(const Mat&);
    void show_histogram();
    void draw_graph(const histogram_bins&);
    void on_mouse_pressed(int event, int x, int y, int flags, void *userdata);
    void on_mouse_released(int event, int x, int y, int flags, void *userdata);
    void debug_output(ostream&, const histogram_bins&);
};

HistogramViewer::HistogramViewer(string filename) :
 filename(filename)
{
    namedWindow(image_window_name);
    setMouseCallback(image_window_name, HistogramViewer::on_mouse_event, this);
    
    namedWindow(graph_window_name);
}

HistogramViewer::~HistogramViewer()
{
    destroyWindow(graph_window_name);
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
    
    //debug_output(cout, bins);
    draw_graph(bins);
}

void HistogramViewer::draw_graph(const histogram_bins& bins)
{
    // TODO: normalize heights
    
    // height: 25 + maximum in any bin
    // width: 20 * 8 bins
    // type: 3 channel byte
    Mat graph = Mat::zeros(500, 500, CV_8UC(3));
    vector<Scalar> bar_colors = { Scalar(0, 0, 200), Scalar(0, 200, 0), Scalar(200, 0, 0)};
    
    auto total_pixels = 0;
    for (auto pixel_count : bins[0])
    {
        total_pixels += pixel_count;
    }
    
    for (auto i = 0; i < 3; ++i)
    {
        const auto& current_channel = bins[i];
        const auto& current_bar_color = bar_colors[i];
        int offset_x = 0 + (5 * i);
        
        for (auto j = 0; j < 8; ++j)
        {
            const auto& bin_value = current_channel[j];
            offset_x += 50;
            //cout << "drawing @ " << offset_x << " in " << current_bar_color << endl;
            double bar_scale = (double)bin_value / (double)total_pixels;
            auto bar_height = 300 * bar_scale;
            cout << bar_height << endl;
            
            line(graph, Point(offset_x, 400 - bar_height), Point(offset_x, 400), current_bar_color, 3);
        }
        
    }
    
    for (auto i = 0; i < 8; ++i)
    {
        string bin_range;
        bin_range += to_string(i*32);
        bin_range += "-";
        bin_range += to_string((i+1)*32-1);
        putText(
            graph, 
            bin_range, 
            Point(25 + 50 * i, 450 + (i%2 * 25)), // Upper-left corner of text box
            FONT_HERSHEY_PLAIN, // Font (e.g., cv::FONT_HERSHEY_PLAIN)
            0.9, // size (a multiplier, not "points"!)
            Scalar(175, 25, 225), // Color, RGB form
            1);
    }
    
    imshow(graph_window_name, graph);
    
    // for each color
    //    set color
    //    calculate offset
    //    for each range
    //        calculate offset
    //        draw label
    //        draw bar depending on value in color
}

void HistogramViewer::debug_output(ostream& os, const histogram_bins& bins)
{
    int i = 0;
    for (const auto& color_bin : bins)
    {
        os << "channel " << i << endl;
        int j = 0;
        for (const auto& value : color_bin)
        {
            os << j*32 << "-" << (j+1)*32-1 << ": " << value << endl;
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
