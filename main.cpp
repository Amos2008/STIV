#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<math.h>
#include <chrono>
#include"merge.h"
#include<fstream>

using namespace cv;
using namespace std;
using namespace std::chrono;

  

int main()
{
    high_resolution_clock::time_point beginTimes, endTimes;
    beginTimes = high_resolution_clock::now();

    string file = "F:/video.MOV";
    Point2f p1(575.75,161.97), p2(518.52,270.33);
    int line_w = (int)(sqrt(abs(p1.x - p2.x) * abs(p1.x - p2.x) + abs(p1.y - p2.y) * abs(p1.y - p2.y)) + 0.5);
    Mat dst(FRAMERATE, line_w, CV_8UC3);

    merge_test(file, p1, p2,dst);

    imwrite("merge.png", dst);
    endTimes = high_resolution_clock::now();
    cout << "dot merge process times: " << (double)duration_cast<microseconds>(endTimes - beginTimes).count() * 1e-6 << endl;



    int a = 0;



    
    return 0;
}
