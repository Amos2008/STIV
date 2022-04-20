#include"merge.h"

//求长度
inline int line_width(Point2f& p1, Point2f& p2)
{
    float line_x = abs(p1.x - p2.x);
    float line_y = abs(p1.y - p2.y);
    return  (int)(sqrt(line_x * line_x + line_y * line_y) + 0.5);
}


struct LinePara
{
    float k;
    float b;
};

// 获取直线长度的参数，求所有点集 
inline void getLinePara(Point2f& p1, Point2f& p2, LinePara& LP)
{
    double m = 0;
    p1.y = -p1.y;
    p2.y = -p2.y;
    // 计算分子  
    m = p2.x - p1.x;

    if (0 == m)
    {
        LP.k = 10000.0;
        LP.b = p1.y - LP.k * p1.x;
    }
    else
    {
        LP.k = (p2.y - p1.y) / (p2.x - p1.x);
        LP.b = p1.y - LP.k * p1.x;
    }

}


void compress_image(Mat& frame, Mat& dst)
{
    for (int i = 0; i < frame.rows; i++)
    {
        for (int j = 0; j < frame.cols; j++)
        {
            frame.at<Vec3b>(i, j)[0] = frame.at<Vec3b>(i, j)[0];
            frame.at<Vec3b>(i, j)[1] = frame.at<Vec3b>(i, j)[1];
            frame.at<Vec3b>(i, j)[2] = frame.at<Vec3b>(i, j)[2];
        }
    }

}





void merge_test(string& file, Point2f& p1, Point2f& p2,Mat&dst)
{
    //越界
    if (p1.x < 0 || p1.y < 0 || p2.x < 0 || p2.y < 0)
    {
        cout << "选取点越界" << endl;
        return ;
    }


    VideoCapture capture(file);
    if (!capture.isOpened())
    {
        cout << "视频为空" << endl;
        return;
    }

    //直线长度
    int line_w = line_width(p1, p2);
    LinePara lp;
    getLinePara(p1, p2, lp);

    //存储图像的坐标值 
    float line_gap = (p2.x - p1.x) / line_w;
    vector<Point2f>dot(line_w);
    for (int i = 0; i < line_w; i++)
    {
        dot[i].x = p1.x + i * line_gap;
        dot[i].y = lp.k * dot[i].x + lp.b;
    }

    for (auto &var : dot)
    {
        var.y = -var.y;
    }



    //点的距离长度
    vector<float> v;
    for (int i=0;i<dot.size()-1;i++)
    {
        float aaa = pow(dot[i + 1].x - dot[i].x, 2) + pow(dot[i + 1].y - dot[i].y, 2);
        v.push_back(aaa);

    }
    //存储图像的像素值 
    vector<vector<Vec3b>> img_pix;


    Mat H = imread("H.bmp");
    while (true)
    {
        vector<Vec3b>img_line;
        Mat frame;
        capture >> frame;
        if (frame.empty()) {
            break;
        }

        warpPerspective(frame, frame, H, frame.size());

        //flip(frame, frame, 1);
        //imwrite("frame.png", frame);
        //越界
        int width = frame.rows;
        int height = frame.cols;
        if (p1.x >=width || p1.y >= height|| p2.x >= width || p2.y >=height)
        {
            cout << "选取点越界" << endl;
            break;
        }

        //每一帧图像取一行
        for (int i = 0; i < dot.size(); i++)
        {
            //插值取像素值
            Rect rect1((int)(dot[i].x - 1), (int)(dot[i].y - 1), 2, 2);
            Mat roi;
            frame(rect1).copyTo(roi);
            Vec3b pixel;           
            for (int m = 0; m < 3; m++)
            {
                float R1 = (dot[i].x - (int)dot[i].x) * (float)(roi.at<Vec3b>(0, 1)[m] - roi.at<Vec3b>(0, 0)[m]) + (float)roi.at<Vec3b>(0, 0)[m];
                float R2 = (dot[i].x - (int)dot[i].x) * (float)(roi.at<Vec3b>(1, 1)[m] - roi.at<Vec3b>(1, 0)[m]) + (float)roi.at<Vec3b>(1, 0)[m];
                pixel[m] = (uchar)((dot[i].y - (int)dot[i].y) * (R2 - R1) + R1);
            }

            img_line.push_back(pixel);

        }
        //所有行图像
        img_pix.push_back(img_line);
        img_line.clear();

    }

    //合成图像

    for (int i = 0; i < FRAMERATE; i++)
    {
        for (int j = 0; j < line_w; j++)
        {
            dst.at<Vec3b>(i, j)[0] = img_pix[i][j][0];
            dst.at<Vec3b>(i, j)[1] = img_pix[i][j][1];
            dst.at<Vec3b>(i, j)[2] = img_pix[i][j][2];
        }
    }


}