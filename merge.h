#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<math.h>
const int FRAMERATE = 1665;

using namespace cv;
using namespace std;


void  merge_test(string& file, Point2f& p1, Point2f& p2, Mat &dst);
