#pragma once

#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>



using namespace std;
using namespace cv;


class cube_detector
{
public:
	Mat cube_Find(Mat img);
	Mat getMat(Mat frame);
	int check_color(Mat src);
private:
	Mat frame;
};



