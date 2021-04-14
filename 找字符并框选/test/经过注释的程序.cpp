#include <opencv2/opencv.hpp>
#include <iostream>
#include "math.h"
using namespace cv;
using namespace std;

//---�洢Ŀ������Ĳ���
typedef struct MyStruct
{
	Rect my_rec;//Ŀ�����
	Mat my_img;//Ŀ��ROI����
}MyStruct;

int main(int argc, char*argv[])
{
	Mat inputImage = imread("1.png");
	Mat showImage = inputImage.clone();
	cvtColor(inputImage, inputImage, CV_BGR2GRAY);
	//---����ͼ��ֱ���ǻҶ�ͼ/��ֵͼ�����Բ���Ҫ�����ر�Ĵ���
	Mat morph, gray = inputImage.clone(), showGray;
	showGray.create(inputImage.size(), CV_8UC1);
	showGray.setTo(0);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(inputImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));
	Rect rec_adapt;
	//----�����ǳ���ĵ�һ�����ģ��ų������ҵ���������
	//----��������û��Ҫ������ô��������Ļ����һ��ȥ�жϣ���������̫�������װ���Ҫ��ϢŪ��
	for (size_t i = 0; i < contours.size(); i++)
	{
		//----���������������ռ�ܵı�������ֹ�нϴ�Ŀհ�������ż����
		//----���εĳ������ƣ�Ҳ���������Ӷ������������������
		int x = minAreaRect(contours[i]).boundingRect().x;
		int y = minAreaRect(contours[i]).boundingRect().y;
		int width = minAreaRect(contours[i]).boundingRect().width;
		int height = minAreaRect(contours[i]).boundingRect().height;
		Mat true_image;
		int true_pix_count;
		double true_pix_rate;
		//---�������Ŀ�걻ȥ�������ڴ˽����Ż�
		if (x < 0 || y < 0) true_pix_rate = 1;
		else
		{
			true_image = gray(Rect(x, y, width, height));
			true_pix_count = countNonZero(true_image);
			true_pix_rate = static_cast<double>(true_pix_count) / static_cast<double>(minAreaRect(contours[i]).boundingRect().area());
		}			 
		double angle = minAreaRect(contours[i]).angle;
		bool flag_angle = (angle == 9 ||  angle == 180 || angle == 0 ) ? true : false;//|| angle == 270
		if (minAreaRect(contours[i]).size.height >= 10 && minAreaRect(contours[i]).size.height <= 20 && minAreaRect(contours[i]).size.width >= 4 && minAreaRect(contours[i]).size.width <= 30  && flag_angle && true_pix_rate <= 0.8)//
		{
			drawContours(showGray, contours, static_cast<int>(i), Scalar(255, 255, 255), 1);
		}
	}
	//---��̬ѧ��������������������֣������ϸ�ڿ�����ͼƬ��
	//---���ϸ�ڲ��ֲ����Ĳ��ÿ����ڴ˽����Ż�---//
	Mat img1;
	Mat kernel_x = getStructuringElement(MORPH_RECT, Size(20,1));
	Mat kernel_y = getStructuringElement(MORPH_RECT, Size(1, 28));
	Mat kernel_x_l = getStructuringElement(MORPH_RECT, Size(20, 1));
	morphologyEx(showGray, showGray, MORPH_DILATE, kernel_x);
	morphologyEx(showGray, showGray, MORPH_DILATE, kernel_x);
	//morphologyEx(showGray, showGray, MORPH_DILATE, kernel_x);
	morphologyEx(showGray, img1, MORPH_OPEN, kernel_y);
	showGray = showGray - img1;
	morphologyEx(showGray, showGray, MORPH_CLOSE, kernel_x_l);
	//---��Ŀ��������о��ΰ���
	findContours(showGray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));
	vector<MyStruct> my_class;
	for (size_t i = 0; i < contours.size(); i++)
	{	
		if (boundingRect(contours[i]).width > 60)
		{
			int x = minAreaRect(contours[i]).boundingRect().x;
			int y = minAreaRect(contours[i]).boundingRect().y;
			int width = minAreaRect(contours[i]).boundingRect().width;
			int height = minAreaRect(contours[i]).boundingRect().height;
			MyStruct Struct_temp;
			Struct_temp.my_rec = boundingRect(contours[i]);
			Struct_temp.my_img = showImage(Rect(x, y, width, height)).clone();
			my_class.push_back(Struct_temp);
			rectangle(showImage, boundingRect(contours[i]), Scalar(0, 0, 255));
		}		
	}
	

	waitKey(0);
	return 0;

}