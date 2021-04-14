#include<iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace cv;
using namespace std;

int Threshold_Value = 176;
const int Threshold_Max_value = 255;
const int Threshold_type_value = 3;

Mat input_image, threshold_image, output_image, Middle_image;

void Threshold_Image_Bar(int, void *);

int main(int argc, char**argv)
{
	input_image = imread("b.jpg");
	if (input_image.data == NULL) {
		return -1; cout << "can't open image.../";
	}
	//----------��Ԥ����
	imshow("Sourse Image", input_image);
	blur(input_image, Middle_image, Size(3, 3), Point(-1, -1), 4);
	imshow("Blur Image", Middle_image);
	cvtColor(Middle_image, Middle_image, COLOR_RGB2GRAY);
	imshow("Gray Image", Middle_image);
	//-----------���ñ�����ͼ�����ROI����
	const float init_pointx =  saturate_cast<float>(Middle_image.cols / 7);
	const float init_pointy =  saturate_cast<float>(Middle_image.rows / 7);
	Rect roi_rect = Rect(Point2f(2 * init_pointx, 2 * init_pointy), Point2f(6 * init_pointx, 6 * init_pointy));
	Mat  roi_Image = Middle_image(roi_rect);
	Middle_image = roi_Image;
	//----------����ʹ�ô󷨶�������Ӧ����ͼ�񣬳�����ȶ��Ժܸ�
	threshold(Middle_image, threshold_image, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);//���65���Ը��Ŀ�Ч��
	imshow("Threshold Image", threshold_image);
	//----------����ͨ����̬ѧ������ͼ����������
	Mat kernel_rect   = getStructuringElement(MORPH_ELLIPSE, Size(30, 30), Point(-1, -1));
	Mat kernel_circle = getStructuringElement(MORPH_ELLIPSE, Size(10, 10), Point(-1, -1));
	morphologyEx(threshold_image, threshold_image, MORPH_CLOSE, kernel_circle);
	Mat RedImage = threshold_image.clone();
	/*--------���ǵ�ʱ������Բ�Ĵ��룬���ﲻ��Ҫ�ˣ��������Բ��׼ȷ
	Mat otherImage;
	Canny(threshold_image, otherImage, 50, 200);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat showImage = Mat::zeros(RedImage.size(), CV_8UC1);
	findContours(RedImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(-1, -1));
	for (size_t i = 0; i < contours.size(); i++)
	{
	if (boundingRect(contours[i]).area() > 10000)//���������žͿ���
	{
	drawContours(showImage, contours, static_cast<int>(i), Scalar(255, 255, 255), 1);
	Point2f center;
	float radius;
	minEnclosingCircle(contours[i], center, radius);
	Mat result = Mat::zeros(RedImage.size(), CV_8UC3);
	circle(input_image, center, radius, Scalar(0, 0, 255), 2);
	}
	}
	*/
	morphologyEx(RedImage, threshold_image, MORPH_OPEN, kernel_rect);
	//---------ͼ��ɾ���Ա߸�������
	for (size_t i = 0; i < threshold_image.rows; i++)
	{
		for (size_t j = 0; j < threshold_image.cols; j++)
		{
			RedImage.at<uchar>(i, j) = saturate_cast<uchar>(RedImage.at<uchar>(i, j) - threshold_image.at<uchar>(i, j));
		}
	}
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat showImage = Mat::zeros(RedImage.size(), CV_8UC1);
	findContours(RedImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));
	for (size_t i = 0; i < contours.size(); i++)
	{
		//------���Բ�ҵ�λ��̫���ף�����ɽ����Ż�
		//------����ҵ�Բλ�ò���ȷ�����������
		if (minAreaRect(contours[i]).size.area() > 10000 && minAreaRect(contours[i]).size.height > 80 && minAreaRect(contours[i]).size.width > 80)//���������žͿ���
		{
			drawContours(showImage, contours, static_cast<int>(i), Scalar(255, 255, 255), 1);
		}
	}
	//-----------��������������е㼯����
	vector<Point> points;
	for (int i = 0; i < showImage.rows; i++)
	{
		for (int j = 0; j < showImage.cols; j++)
		{
			if (showImage.at<uchar>(i, j) == 255)
			{
				points.push_back(Point(j, i));
			}
		}
	}
	//----------�����Բ���������ֻ����һ��Բ��Ҳ���Ż����Ҷ��Բ
	Point2f center;
	float radius;
	if (points.data() == 0)
	{
		printf("Don't detecte point");
		return -1;
	}
	minEnclosingCircle(points, center, radius);
	center.x += 2 * init_pointx;
	center.y += 2 * init_pointy;
	Mat result = Mat::zeros(RedImage.size(), CV_8UC3);
	circle(input_image, center, radius, Scalar(0, 0, 255), 2);

	/*namedWindow("Threshold Image", 1);
	createTrackbar("��ֵ����", "Threshold Image", &Threshold_Value, 255, Threshold_Image_Bar);
	Threshold_Image_Bar(0, 0);*/
	imshow("result", input_image);
	waitKey(0);
	return 0;
}
//------���ǵ����õĻ��飬�Ѿ����Ժ��ˣ��������ȥ�ˣ��������Ҫ�����Ǿ���ʹ��
void Threshold_Image_Bar(int, void *)
{
	threshold(Middle_image, threshold_image, 65, 255,THRESH_BINARY_INV);//110,65
	imshow("Threshold Image", threshold_image);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(50, 50), Point(-1, -1));//�����������ν��
	Mat RedImage = threshold_image.clone();
	/*
	Mat otherImage;
	Canny(threshold_image, otherImage, 50, 200);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat showImage = Mat::zeros(RedImage.size(), CV_8UC1);
	findContours(RedImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(-1, -1));
	for (size_t i = 0; i < contours.size(); i++)
	{
		if (boundingRect(contours[i]).area() > 10000)//���������žͿ���
		{
			drawContours(showImage, contours, static_cast<int>(i), Scalar(255, 255, 255), 1);
			Point2f center;
			float radius;
			minEnclosingCircle(contours[i], center, radius);
			Mat result = Mat::zeros(RedImage.size(), CV_8UC3);
			circle(input_image, center, radius, Scalar(0, 0, 255), 2);
		}
	}
	*/
	morphologyEx(RedImage, threshold_image, MORPH_OPEN, kernel);
	for (size_t i = 0; i < threshold_image.rows; i++)
	{
		for (size_t j = 0; j < threshold_image.cols; j++)
		{
			RedImage.at<uchar>(i, j) = saturate_cast<uchar>(RedImage.at<uchar>(i, j) - threshold_image.at<uchar>(i, j));
		}
	}
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat showImage = Mat::zeros(RedImage.size(), CV_8UC1);
	findContours(RedImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));
	for (size_t i = 0; i < contours.size(); i++)
	{
		if (boundingRect(contours[i]).area() > 20000)//���������žͿ���
		{
			drawContours(showImage, contours, static_cast<int>(i), Scalar(255, 255, 255), 1);
		}
	}
	vector<Point> points;
	for (int i = 0; i < showImage.rows; i++)
	{
		for (int j = 0; j < showImage.cols; j++)
		{
			if (showImage.at<uchar>(i, j) == 255)
			{
				points.push_back(Point(j, i));
			}
		}
	}
	Point2f center;
	float radius;
	minEnclosingCircle(points, center, radius);
	Mat result = Mat::zeros(RedImage.size(), CV_8UC3);
	circle(input_image, center, radius, Scalar(0, 0, 255), 2);
}