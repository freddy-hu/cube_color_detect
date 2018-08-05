#include "cube.hpp"

//������С�Ƚ�
static inline bool ContoursSortFun(vector<cv::Point> contour1, vector<cv::Point> contour2)
{
	return (contour1.size() > contour2.size());
}

Mat cube_detector::cube_Find(Mat img)
{
	Mat img_gray, img_binary, img_edge, img_hsv;
	//תΪHSV�ռ�
	cvtColor(img, img_hsv, CV_BGR2HSV);
	GaussianBlur(img_hsv, img_hsv, Size(5, 5), 0, 0);
	//ֱ��ͼ���⻯������������ǿħ��ͼ��
	vector<Mat> hsvSplit;
	split(img_hsv, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, img_hsv);
	cvtColor(img_hsv, img_gray, CV_BGR2GRAY);
	medianBlur(img_gray, img_gray, 3);
	//***********�ų���ɫ��������*******************
	cv::adaptiveThreshold(img_gray, img_binary, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 7, -4);
	//��̬ѧ����???????????????????????????????????????????????????????????????????
	Mat core = getStructuringElement(MORPH_RECT, Size(5, 5));
	morphologyEx(img_binary, img_binary, MORPH_CLOSE, core, Point(-1, -1), 3);
	morphologyEx(img_binary, img_binary, MORPH_OPEN, core, Point(-1, -1), 4);
	//�������
	vector<vector<Point>> contours2;
	vector<Vec4i>	hierarchy2;
	findContours(img_binary, contours2, hierarchy2, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
	//1.���������õ������ӿ�
	std::sort(contours2.begin(), contours2.end(), ContoursSortFun);
	vector<Rect> boundRect(contours2.size());  //������Ӿ��μ���
	vector<RotatedRect> box(contours2.size()); //������С��Ӿ��μ���
	Point2f rect[4];
	int xRoi, yRoi, widthRoi, heightRoi;
	for (int i = 0; i <= 0; i++)
	{
		box[i] = minAreaRect(Mat(contours2[i]));  //����ÿ��������С��Ӿ���
		boundRect[i] = boundingRect(Mat(contours2[i]));
		box[i].points(rect);  //����С��Ӿ����ĸ��˵㸴�Ƹ�rect����
		xRoi = boundRect[i].x;
		yRoi = boundRect[i].y;
		widthRoi = boundRect[i].width;
		heightRoi = boundRect[i].height;
		rectangle(img, Point(boundRect[i].x, boundRect[i].y), Point(boundRect[i].x + boundRect[i].width, boundRect[i].y + boundRect[i].height), Scalar(0, 255, 0), 2, 8);
	}
	imshow("���ο�ѡħ��", img);
	Mat solo;
	//��ȡħ��
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

//2.��ȡ��ɫ
int cube_detector::check_color(Mat src)
{
	Mat imgHSV;
	vector<Mat> hsvSplit;
	cvtColor(src, imgHSV, COLOR_BGR2HSV);
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat imgThresholded1, imgThresholded2, imgThresholded3, imgThresholded4, imgThresholded5, imgThresholded6;
	inRange(imgHSV, Scalar(90, 43, 46), Scalar(125, 255, 255), imgThresholded1);                                  /*�жϾŸ����Ƿ��� **��ɫ**������У���a=1           */
	morphologyEx(imgThresholded1, imgThresholded1, MORPH_OPEN, element);
	imshow("��ȡ��ɫ", imgThresholded1);
	int a = check(imgThresholded1);
	return a;
}





//1.ȥ����
Mat cube_detector::getMat(Mat frame)
{
	Mat img1, img2;                      //img2Ϊ���ͼ��img1Ϊԭͼ��img�ǽ�����Ҫ������ֵ�������ͼ      
	//����ԭͼ
	Mat img = cube_Find(frame);
	img.copyTo(img1);
	imshow("ԭͼ", img);
	cvtColor(img, img, CV_BGR2HSV);
	cvtColor(img, img, CV_BGR2GRAY);                               //��img����һϵ�д����ֵ��
	GaussianBlur(img, img, Size(5, 5), 0, 0);
	threshold(img, img, 140, 255, THRESH_BINARY);
	medianBlur(img, img, 3);
	img1.copyTo(img2, img);                                        //������Ĥ����
	imshow("ȥ����",img2);
	return img2;
}