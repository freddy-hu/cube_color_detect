/*
	1.ȡ��Ӿ���:��ɫ�������߼��г��ֻᵼ��ֻȡ��һ�����ȡ��������ħ�������Բ��ܳ���һ�����ϴ�Ƭ��ɫ����ɫͬʱ����
	2.��������ʱ��ɫ��ȱʧ������Ӱ����
	3.���ܰ�����ͷ�ĵ�����ȥ
	4.��ѡħ���ɹ��ʲ��ܴﵽ�ٷ�֮��
*/

	
#include "cube.hpp"



int main()
{
	cube_detector cube;
	Mat frame = imread("E://picture/a.jpg");
	
	//camera
	Mat dstImage1;
	resize(frame, dstImage1, Size(frame.cols /5 , frame.rows / 5), 0, 0, INTER_LINEAR);
	imshow("��Сͼ", dstImage1);
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