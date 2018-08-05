/*
	1.取外接矩形:白色块过多或者集中出现会导致只取到一个面而取不到整个魔方，所以不能出现一个面上大片白色和蓝色同时出现
	2.背景分离时红色有缺失，但不影响结果
	3.不能把摄像头拍到外面去
	4.框选魔方成功率不能达到百分之百
*/

	
#include "cube.hpp"



int main()
{
	cube_detector cube;
	Mat frame = imread("E://picture/a.jpg");
	
	//camera
	Mat dstImage1;
	resize(frame, dstImage1, Size(frame.cols /5 , frame.rows / 5), 0, 0, INTER_LINEAR);
	imshow("缩小图", dstImage1);
	Mat result = cube.getMat(dstImage1);
	
	//return
	int key = cube.check_color(result);
	if (key==1)
		cout << "not complete";
	else        
		cout << "complete";
	waitKey(0);

	return 0;
}