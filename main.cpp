#if 1
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
    //Point2f p1(575.75, 161.97), p2(518.52, 470.33); // 默认值 
    //Point2f p1(575.75, 161.97), p2(518.52, 210.33);
    // Point2f p1(575.75, 161.97), p2(518.52, 210.33);//生成直线
      Point2f p1(575.75, 161.97), p2(425.52, 288.33);//生成直线
    int line_w = (int)(sqrt(abs(p1.x - p2.x) * abs(p1.x - p2.x) + abs(p1.y - p2.y) * abs(p1.y - p2.y)) + 0.5);
    Mat dst(FRAMERATE, line_w, CV_8UC3);

    merge_test(file, p1, p2,dst);

    imwrite("merge.png", dst);
    endTimes = high_resolution_clock::now();
    cout << "dot merge process times: " << (double)duration_cast<microseconds>(endTimes - beginTimes).count() * 1e-6 << endl;

	return 0;
}

#else 


#include <opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc_c.h>
	using namespace cv;
	using namespace std;

	struct userdata {
		Mat im;
		vector<Point2f> points;
	};


	void mouseHandler(int event, int x, int y, int flags, void* data_ptr)
	{
		if (event == EVENT_LBUTTONDOWN)
		{
			userdata* data = ((userdata*)data_ptr);
			circle(data->im, Point(x, y), 3, Scalar(0, 255, 255), 5, CV_AA);
			imshow("Image", data->im);
			if (data->points.size() < 4)
			{
				data->points.push_back(Point2f(x, y));
			}
		}

	}



	int main(int argc, char** argv)
	{

		// Read in the image.
		Mat im_src = imread("../data//first-image.jpg");
		Size size = im_src.size();

		// Create a vector of points.
		vector<Point2f> pts_src;
		pts_src.push_back(Point2f(0, 0));
		pts_src.push_back(Point2f(size.width - 1, 0));
		pts_src.push_back(Point2f(size.width - 1, size.height - 1));
		pts_src.push_back(Point2f(0, size.height - 1));



		// Destination image
		Mat im_dst = imread("../data//times-square.jpg");


		// Set data for mouse handler
		Mat im_temp = im_dst.clone();
		userdata data;
		data.im = im_temp;


		//show the image
		imshow("Image", im_temp);

		cout << "Click on four corners of a billboard and then press ENTER" << endl;
		//set the callback function for any mouse event
		setMouseCallback("Image", mouseHandler, &data);
		waitKey(0);

		// Calculate Homography between source and destination points
		Mat h = findHomography(pts_src, data.points);

		// Warp source image
		warpPerspective(im_src, im_temp, h, im_temp.size());

		// Extract four points from mouse data
		Point pts_dst[4];
		for (int i = 0; i < 4; i++)
		{
			pts_dst[i] = data.points[i];
		}

		// Black out polygonal area in destination image.
		fillConvexPoly(im_dst, pts_dst, 4, Scalar(0), CV_AA);

		// Add warped source image to destination image.
		im_dst = im_dst + im_temp;

		// Display image.
		imshow("Image", im_dst);
		waitKey(0);

		return 0;
	}
















  

#endif // 0

    
