#include <iostream>
#include <opencv2/core.hpp>

using namespace cv;
using std::cout; using std::endl; using std::cin;


/* 1
a) choose negative floating point number
b) abs(n), round(n), ceil(n), floor(n)
c) generate some random numbers
d) convert: Point2f->Point->Point2f
*/

void _1()
{
    float f;
    cout << "gib negative float plz: ";
    cin >> f;
    
    cout << "abs, round, ceil, floor:" << endl;
    cout << cv::abs(f) << endl << cvRound(f) << endl << cvCeil(f) << endl << cvFloor(f) << endl;  
    
    cout << "the randomness: " << endl;
    RNG rng;
    for (auto i = 0; i < 20; ++i)
    {
        cout << rng.uniform(0., 1.) << " " << rng.uniform(0, 25) << " " << rng.uniform(0.f, 1.f) << " " << rng.gaussian(0.25) << endl;
    }
    
    Point2f p { 1.25f, -2.5f };
    Point p_ { p };
    Point2f p__ { p_ };
    
    cout << "p2f: " << p << " p2f->p: " << p_ << " p->p2f: " << p__ << endl;
}

/*
a) create 3x3 matrix and 3 component vector with "shorthands"
b) multiply yes/no? why?
*/
void _2()
{
    cout << "matrix multiplication exercise 2" << endl;
    auto m = Matx33f::eye();
    auto v = Vec3f(1.25f, -0.66f, 0.4f);
    
    // multiplication mat*vec works because opencv vectors are a specialization of opencv matrices
    auto p = m * v; 
    
    // multiplication vec*mat does not work
    // auto p = v * m;
    cout << p << endl;
}

/*
a) create 3x3 matrix and 3 component vector from explicit template instantiations
b) m * v yes/no? why?
c) what happens when casting the vector to a 3x1 matrix?
*/
void _3()
{
    cout << "matrix multiplication exercise 3" << endl;
    auto m = Matx<float, 3, 3>::eye();
    auto v = Vec<float, 3> {1.25f, -0.66f, 0.4f};
    
    auto p = m * v;
    cout << p << endl;
    
    auto q = m * (Matx<float, 3, 1>)v; // q == p, but is a matrix and not a vector.
    cout << q << endl; 
}

int main(int argc, char** argv)
{  
    _1();
    _2();
    _3();
    
    return 0;
}

