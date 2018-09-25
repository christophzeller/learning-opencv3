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
    void add_graph_labels(Mat&, int);
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
    
    imshow(image_window_name, display_image);
    
    waitKey(0);
}

void HistogramViewer::show_histogram()
{
    Mat color_histogram_region (image, mouse_drag_region);
    rectangle(display_image, mouse_drag_region, Scalar(175, 25, 225));
    imshow(image_window_name, display_image);
    
    auto channel_data = calculate_bins(color_histogram_region);
    
    //debug_output(cout, bins);
    draw_graph(channel_data);
}

void HistogramViewer::draw_graph(const histogram_bins& channel_data)
{
    Mat graph = Mat::zeros(500, 500, CV_8UC(3));
    vector<Scalar> bar_colors = { 
                                    Scalar(200, 0, 0), 
                                    Scalar(0, 200, 0), 
                                    Scalar(0, 0, 200) 
                                };
    
    // find max value for y axis
    int max_count = 0;
    for (auto channel : channel_data)
    {
        for (auto pixel_count : channel)
        {
            if (pixel_count > max_count)
                max_count = pixel_count;
        }
    }
    
    // draw bars
    for (auto i = 0; i < 3; ++i)
    {
        const auto& current_channel = channel_data[i];
        const auto& current_bar_color = bar_colors[i];
        int offset_x = 0 + (5 * i);
        
        for (auto j = 0; j < 8; ++j)
        {
            const auto& bin_value = current_channel[j];
            double bar_scale = (double)bin_value / (double)max_count;
            auto bar_height = 300 * bar_scale;

            offset_x += 50;            
            line(graph, 
                Point(offset_x, 350 - bar_height), 
                Point(offset_x, 350), 
                current_bar_color, 
                3);
        }
    }
    
    add_graph_labels(graph, max_count);
    
    imshow(graph_window_name, graph);
}

void HistogramViewer::add_graph_labels(Mat& graph, int max_count)
{
    Scalar magenta(175, 25, 225);

    // x axis labels
    putText(
            graph, 
            to_string(max_count), 
            Point(0, 40), 
            FONT_HERSHEY_PLAIN, 
            0.9, 
            magenta);
            
    putText(
            graph, 
            to_string(max_count / 2), 
            Point(0, 190), 
            FONT_HERSHEY_PLAIN, 
            0.9, 
            magenta, 
            1);
            
    putText(
            graph, 
            "0", 
            Point(0, 350), 
            FONT_HERSHEY_PLAIN, 
            0.9, 
            magenta, 
            1);
            
    // bin ranges / x axis labels
    for (auto i = 0; i < 8; ++i)
    {
        string bin_range;
        bin_range += to_string(i*32);
        bin_range += "-";
        bin_range += to_string((i+1)*32-1);
        putText(
            graph, 
            bin_range, 
            Point(25 + 50 * i, 400 + (i%2 * 25)), 
            FONT_HERSHEY_PLAIN, 
            0.9, 
            magenta);
    }
    
    // axis descriptors
    putText(
            graph, 
            "RGB Intensity", 
            Point(200, 450), 
            FONT_HERSHEY_PLAIN, 
            1.0, 
            magenta);
    
    putText(
            graph, 
            "Pixel Count", 
            Point(10, 10), 
            FONT_HERSHEY_PLAIN, 
            1.0, 
            magenta);
}

void HistogramViewer::debug_output(ostream& os, const histogram_bins& channel_data)
{
    int i = 0;
    for (const auto& current_channel : channel_data)
    {
        os << "channel " << i << endl;
        int j = 0;
        for (const auto& value : current_channel)
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
    mouse_drag_region.x = x;
    mouse_drag_region.y = y;
}

void HistogramViewer::on_mouse_released(int event, int x, int y, int flags, void *userdata)
{    
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

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "expects exactly one argument: filename" << endl;
    }
    
    HistogramViewer v { argv[1] };
    v.show();

    return 0;
}
