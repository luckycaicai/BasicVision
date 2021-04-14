#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
int FillBlock(Mat src, Mat &mask, Point center);
vector<Point> Find_Max(InputArray _src, Mat&mask, int gap, int radius);
int main(int argc,char**argv)
{
	Mat inputImage = imread("123.png");
	Mat showImage = inputImage.clone();
	cvtColor(inputImage, inputImage, CV_BGR2GRAY);
	threshold(inputImage, inputImage, 1, 255, THRESH_BINARY_INV | THRESH_OTSU);
	//����һ��maskΪ���߽�1���м�255
	Mat mask = Mat::zeros(inputImage.size(), CV_8UC1);
	mask.setTo(255);
	Mat rows = Mat::ones(Size(inputImage.cols, 1), CV_8UC1), cols = Mat::zeros(Size(1, inputImage.rows), CV_8UC1);
	Mat src_rows_beg = mask.row(0);
	Mat src_rows_end = mask.row(inputImage.rows - 1);
	Mat src_cols_beg = mask.col(0);
	Mat src_cols_end = mask.col(inputImage.cols - 1);
	rows.copyTo(src_rows_beg); rows.copyTo(src_rows_end);
	cols.copyTo(src_cols_beg); cols.copyTo(src_cols_end);
	int temp = FillBlock(inputImage, mask, Point(1, 1));
	//ǰ���ͱ���ת��+����任
	bitwise_not(mask, mask);
	distanceTransform(mask, mask, DIST_L2, DIST_MASK_3, 5);
	Mat wjy = mask.clone();
	normalize(mask, mask, 0, 255, NORM_MINMAX);
	mask.convertTo(mask, CV_8UC1);
	
	Mat msk;
	vector<Point> showPoint = Find_Max(mask, msk, 20, 15);
	circle(showImage, Point(87,153), 25, Scalar(100, 0, 255), 1);
	for (size_t i = 0; i < showPoint.size(); i++)
	{
		circle(showImage, showPoint[i], uint(wjy.at<float>(showPoint[i])), Scalar(0, 0, 255), 1);
	}
	return 0;
}

//---Fill parts areas
//---@src		��input distance image/float/one signal
//---@mask		��mask image/ucahr/one signal
//---@center	��nowdays operator centers/ucahr
int FillBlock(Mat src, Mat &mask, Point center)
{
	uchar back = src.at<uchar>(center.y, center.x);//��ǰֵ����ֵ
	vector<Point> fill_point;//�洢�����ĵ�
	int count = 0, count_mount = 1;//count����ǰ�������ص㣬count_mount���ܼ���������
	fill_point.push_back(center);
	while (count < count_mount)
	{
		vector<uchar*> img;
		vector<uchar*> msk;
		for (int i = -1; i < 2; i++)
		{
			img.push_back(src.ptr<uchar>(fill_point[count].y + i));
			msk.push_back(mask.ptr<uchar>(fill_point[count].y + i));
		}
		for (size_t i = 0; i < 3; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				//---�͵�ǰ���ش�Сһ�����Ҳ�Ϊ������Ϊδ������������
				if (img[i][fill_point[count].x + j] == back && !(j == 0 && i == 1) && msk[i][fill_point[count].x + j] == 255)
				{
					fill_point.push_back(Point(fill_point[count].x + j, fill_point[count].y + i - 1));
					msk[i][fill_point[count].x + j] = 1;//����������ȸ�ֵΪ1��Ϊ�˱��ڲ���
				}
			}
		}
		msk[1][fill_point[count].x] = 1;//��ǰ�������Ϊ1
		count_mount = fill_point.size() - 1;//ѭ���ĺ��ģ�
		fill_point.erase(fill_point.begin());//ִ��һ��ɾ��һ������ռ���ڴ�
	}
	return 0;
}

//---������Ĥ
//---@_src		�����뵥ͨ������ͼ
//---@mask		: ����������ֵΪ->255���߽�->0������->1
void MaskImage(InputArray _src, Mat &mask)
{
	Mat src = _src.getMat(), mask_tmp = Mat::zeros(src.size(), CV_8UC1);
	mask_tmp.setTo(255);//ȫ��ֵ��ֵΪ1�������Ժ����
	Mat rows = Mat::zeros(Size(src.cols, 1), CV_8UC1), cols = Mat::zeros(Size(1, src.rows), CV_8UC1);
	Mat src_rows_beg = mask_tmp.row(0);
	Mat src_rows_end = mask_tmp.row(src.rows - 1);
	Mat src_cols_beg = mask_tmp.col(0);
	Mat src_cols_end = mask_tmp.col(src.cols - 1);
	rows.copyTo(src_rows_beg); rows.copyTo(src_rows_end);
	cols.copyTo(src_cols_beg); cols.copyTo(src_cols_end);
	for (size_t i = 1; i < src.rows - 1; i++)
	{
		uchar *img0 = src.ptr<uchar>(i - 1);
		uchar *img = src.ptr<uchar>(i);
		uchar *img1 = src.ptr<uchar>(i + 1);
		uchar *msk = mask_tmp.ptr<uchar>(i);
		for (size_t j = 1; j < src.cols - 1; j++)
		{
			bool flag = false;
			//msk[j] = img[j] == 0 ? 0 : msk[j];//���������mask���Ϊ0
			if (msk[j] != 255) continue;
			flag = (img[j] < img[j - 1] || img[j] < img[j + 1]
				|| img[j] < img0[j] || img[j] < img0[j - 1]
				|| img[j] < img0[j + 1] || img[j] < img1[j]
				|| img[j] < img1[j - 1] || img[j] < img1[j + 1])
				? true : false;//���Ŀ������С����Χ�κ�һ�����ؾ�˵�����һ���������ֵ
			int tmp = flag == true ? FillBlock(src, mask_tmp, Point(j, i)) : 0;//tmpû���壬����Ϊ�˵��ú���ĺ�������

		}
	}
	mask = mask_tmp.clone();
}
//---������Щ���õ����ֵ
//---@
//---@
//---@gap		:���,����ɽ֮������
//---@radius	:�뾶,����ɽ֮��İ뾶

vector<Point> Find_Max(InputArray _src, Mat&mask, int gap, int radius)
{
	Mat src = _src.getMat();

	typedef struct MyStruct
	{
		Point position;
		float data;
	}MyStruct;

	MaskImage(src, mask);//�ҿ������ֵ
						 //---�����ֵ�����Ϣ���
	vector<MyStruct> max_point;
	for (size_t i = 0; i < src.rows; i++)
	{
		uchar *img = src.ptr<uchar>(i);
		uchar *msk = mask.ptr<uchar>(i);
		for (size_t j = 0; j < src.cols; j++)
		{
			if (msk[j] != 255) continue;
			MyStruct my_struct;
			my_struct.data = img[j];
			my_struct.position = Point(j, i);
			max_point.push_back(my_struct);
		}
	}
	//---�������ֵ��Ϣ
	for (size_t i = 0; i < max_point.size(); i++)
	{
		for (size_t j = i; j < max_point.size(); j++)
		{
			MyStruct temp;
			if (max_point[i].data <= max_point[j].data)
			{
				if (max_point[j].data == 0) continue;
				temp = max_point[j];
				max_point[j] = max_point[i];
				max_point[i] = temp;
			}
		}
	}
	//---�����������ֵ
	//---255��	ʵ�����ֵ
	//---0		�߽�
	//---1		����
	//---2		����ռ���
	//---3		��ʱռ���
	for (size_t k = 0; k < max_point.size(); k++)//---
	{
		uchar back = src.at<uchar>(max_point[k].position.y, max_point[k].position.x);//��ǰֵ����ֵ
		vector<Point> fill_point;//�洢�����ĵ�
		int count = 0, count_mount = 1;//count����ǰ�������ص㣬count_mount���ܼ���������
		fill_point.push_back(max_point[k].position);

		while (count < count_mount &&  max_point[k].data != 1)//max_point[k].data == 1˵��������Ѿ��ϳ�
		{
			vector<uchar*> img;
			vector<uchar*> msk;
			for (int i = -1; i < 2; i++)
			{
				img.push_back(src.ptr<uchar>(fill_point[count].y + i));
				msk.push_back(mask.ptr<uchar>(fill_point[count].y + i));
			}
			for (int i = 0; i < 3; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					//---
					uchar x = pow((max_point[k].position.x - fill_point[count].x + j), 2); //(max_point[k].position.x - img[i][fill_point[count].x + j])*(max_point[k].position.x - img[i][fill_point[count].x + j]);
					uchar y = pow((max_point[k].position.y - (fill_point[count].y + i - 1)), 2); // (max_point[k].position.y - img[i][fill_point[count].y + j])*(max_point[k].position.y - img[i][fill_point[count].x + j]);
					uchar distance = sqrt(x + y);//�������
					if (img[i][fill_point[count].x + j] <= img[1][fill_point[count].x] - gap //��Ȳ���
						|| msk[i][fill_point[count].x + j] == 3
						|| msk[i][fill_point[count].x + j] == 0
						|| (j == 0 && i == 1)
						|| distance >= radius) continue;//��ȴ���gap���Ѿ���ǣ��߽磬����ֵ����ֱ���˳�
					if (img[i][fill_point[count].x + j] == 2) max_point[k].data = 1;
					msk[i][fill_point[count].x + j] = 3;
					fill_point.push_back(Point(fill_point[count].x + j, fill_point[count].y + i - 1));
					count_mount++;
				}
			}
			count++;
		}
		if (max_point[k].data == 1)//����̭�����ֵ������ռ��ĵض������ͷ�
		{
			for (size_t i = 0; i < fill_point.size(); i++)
			{
				mask.at<uchar>(fill_point[i]) = 1;
			}
		}
		else//ȷ�������ֵ����ռ����ر�����ȷ��
		{
			for (size_t i = 0; i < fill_point.size(); i++)
			{
				mask.at<uchar>(fill_point[i]) = 2;
			}
			max_point[k].data = 255;
			mask.at<uchar>(max_point[k].position) = 255;
		}
	}
	//---���
	vector<Point> print_wjy;
	for (size_t i = 0; i < mask.rows; i++)
	{
		uchar *msk = mask.ptr<uchar>(i);
		for (size_t j = 0; j < mask.cols; j++)
		{
			if (msk[j] == 255)
				print_wjy.push_back(Point(j, i));
		}

	}
	return print_wjy;
}