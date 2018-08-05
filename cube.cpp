#include "cube.hpp"

//轮廓大小比较
static inline bool ContoursSortFun(vector<cv::Point> contour1, vector<cv::Point> contour2)
{
	return (contour1.size() > contour2.size());
}

Mat cube_detector::cube_Find(Mat img)
{
	Mat img_gray, img_binary, img_edge, img_hsv;
	//转为HSV空间
	cvtColor(img, img_hsv, CV_BGR2HSV);
	GaussianBlur(img_hsv, img_hsv, Size(5, 5), 0, 0);
	//直方图均衡化弱化背景（增强魔方图像）
	vector<Mat> hsvSplit;
	split(img_hsv, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, img_hsv);
	cvtColor(img_hsv, img_gray, CV_BGR2GRAY);
	medianBlur(img_gray, img_gray, 3);
	//***********排除棕色背景干扰*******************
	cv::adaptiveThreshold(img_gray, img_binary, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 7, -4);
	//形态学处理???????????????????????????????????????????????????????????????????
	Mat core = getStructuringElement(MORPH_RECT, Size(5, 5));
	morphologyEx(img_binary, img_binary, MORPH_CLOSE, core, Point(-1, -1), 3);
	morphologyEx(img_binary, img_binary, MORPH_OPEN, core, Point(-1, -1), 4);
	//轮廓检测
	vector<vector<Point>> contours2;
	vector<Vec4i>	hierarchy2;
	findContours(img_binary, contours2, hierarchy2, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
	//1.轮廓排序，拿到最大外接框
	std::sort(contours2.begin(), contours2.end(), ContoursSortFun);
	vector<Rect> boundRect(contours2.size());  //定义外接矩形集合
	vector<RotatedRect> box(contours2.size()); //定义最小外接矩形集合
	Point2f rect[4];
	int xRoi, yRoi, widthRoi, heightRoi;
	for (int i = 0; i <= 0; i++)
	{
		box[i] = minAreaRect(Mat(contours2[i]));  //计算每个轮廓最小外接矩形
		boundRect[i] = boundingRect(Mat(contours2[i]));
		box[i].points(rect);  //把最小外接矩形四个端点复制给rect数组
		xRoi = boundRect[i].x;
		yRoi = boundRect[i].y;
		widthRoi = boundRect[i].width;
		heightRoi = boundRect[i].height;
		rectangle(img, Point(boundRect[i].x, boundRect[i].y), Point(boundRect[i].x + boundRect[i].width, boundRect[i].y + boundRect[i].height), Scalar(0, 255, 0), 2, 8);
	}
	imshow("矩形框选魔方", img);
	Mat solo;
	//提取魔方
	solo = img(Rect(xRoi, yRoi, widthRoi, heightRoi));
	return solo;
}

int check(Mat imgThresholded)
{
	int false1 = 0;
	for (int y = 0; y < imgThresholded.rows; y++)
	{
		for (int x = 0; x < imgThresholded.cols; x++)
		{
			if (imgThresholded.at<uchar>(y, x) == 255)
			{
				false1 = 1; break;
			}
			if (false1 == 1)
				break;
		}
	}
	return false1;
}

//2.提取蓝色
int cube_detector::check_color(Mat src)
{
	Mat imgHSV;
	vector<Mat> hsvSplit;
	cvtColor(src, imgHSV, COLOR_BGR2HSV);
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat imgThresholded1, imgThresholded2, imgThresholded3, imgThresholded4, imgThresholded5, imgThresholded6;
	inRange(imgHSV, Scalar(90, 43, 46), Scalar(125, 255, 255), imgThresholded1);                                  /*判断九个块是否有 **蓝色**，如果有，令a=1           */
	morphologyEx(imgThresholded1, imgThresholded1, MORPH_OPEN, element);
	imshow("提取蓝色", imgThresholded1);
	int a = check(imgThresholded1);
	return a;
}





//1.去背景
Mat cube_detector::getMat(Mat frame)
{
	Mat img1, img2;                      //img2为结果图，img1为原图，img是接下来要经过二值化处理的图      
	//读入原图
	Mat img = cube_Find(frame);
	img.copyTo(img1);
	imshow("原图", img);
	cvtColor(img, img, CV_BGR2HSV);
	cvtColor(img, img, CV_BGR2GRAY);                               //对img进行一系列处理二值化
	GaussianBlur(img, img, Size(5, 5), 0, 0);
	threshold(img, img, 140, 255, THRESH_BINARY);
	medianBlur(img, img, 3);
	img1.copyTo(img2, img);                                        //核心掩膜处理
	imshow("去背景",img2);
	return img2;
}