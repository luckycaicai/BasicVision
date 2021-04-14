#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
using namespace cv;
using namespace std;

void MaskImage(InputArray src, Mat &mask);
int FillBlock(Mat src, Mat &mask, Point center);
vector<Point> Find_Max(InputArray _src, Mat&mask, int gap, int radius);
int main()
{
	
	Mat img = imread("123.png");
	Mat showImage = img.clone();
	cvtColor(img, img, COLOR_BGR2GRAY);
	threshold(img, img, 0, 255, THRESH_BINARY | THRESH_OTSU);
	distanceTransform(img, img, DIST_L2, DIST_MASK_3, 5);
	normalize(img, img, 0, 255, NORM_MINMAX);
	img.convertTo(img, CV_8UC1);
	Mat mask;
	vector<Point> showPoint = Find_Max(img, mask,10, 50);//20,10��������10��50��
	for (size_t i = 0; i < showPoint.size(); i++)
	{
		circle(showImage, showPoint[i], 1, Scalar(0, 0, 255), 2);
	}
	return 0;
}
//---Fill parts aeras
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
		msk[1][fill_point[count].x] = 1;//��ǰ�������Ϊ0
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
	Mat src = _src.getMat(),mask_tmp = Mat::zeros(src.size(), CV_8UC1);
	mask_tmp.setTo(255);//ȫ��ֵ��ֵΪ1�������Ժ����
	Mat rows = Mat::zeros(Size(src.cols, 1), CV_8UC1), cols = Mat::zeros(Size(1, src.rows), CV_8UC1);
	Mat src_rows_beg = mask_tmp.row(0);
	Mat src_rows_end = mask_tmp.row(src.rows - 1);
	Mat src_cols_beg = mask_tmp.col(0);
	Mat src_cols_end = mask_tmp.col(src.cols - 1);
	rows.copyTo(src_rows_beg); rows.copyTo(src_rows_end);
	cols.copyTo(src_cols_beg); cols.copyTo(src_cols_end);
	
	for (size_t i = 3; i < src.rows-3; i++)
	{
		vector<uchar*> img;
		uchar* msk = mask_tmp.ptr(i);
		uchar* img1 = src.ptr(i);
		for (int k = -3; k < 4; k++)
		{
			img.push_back(src.ptr<uchar>(k + i));
		}
		for (size_t j = 3; j < src.cols-3; j++)
		{
			bool flag = false;
			if (msk[j] != 255) continue;//��ǰֵΪ255�Ž��д���
			float sum[4] = { 0 };//�洢���ֵ���жԱȡ�������˵�����������2017.10.17����
			sum[0] = img[3][j];
			Point x0 = Point(0, 0);
			Point x1 = Point(0, 0);
			uchar n_count = 0;
			for (int m = 2; m < 5; m++)
			{
				for (int n = -1; n < 2; n++)
				{
					if (m == 3 && n == 0) continue;
					sum[1] = sum[1] < img[m][j + n] ? img[m][j + n] : sum[1];
					x0 = sum[0] == img[m][j + n] ? Point(m, n) : x0;
					n_count = sum[0] == img[m][j + n] ? n_count+1 : n_count;
					//flag = img[3][j + 0] < img[m][j + n] ? true : flag;//���Ŀ������С����Χ�κ�һ�����ؾ�˵�����һ���������ֵ
				}
			}
			for (int m = 1; m < 6; m++)
			{
				for (int n = -2; n < 3; n++)
				{
					if (2 <= m && m <= 4 && -1 <= n && n <= 1) continue;
					sum[2] = sum[2] < img[m][j + n] ? img[m][j + n] : sum[2];
					x1 = sum[0] == img[m][j + n] ? Point(m, n) : x1;
					n_count = sum[0] == img[m][j + n] ? n_count+1 : n_count;
					//flag = img[3][j + 0] < img[m][j + n] ? true : flag;//���Ŀ������С����Χ�κ�һ�����ؾ�˵�����һ���������ֵ
				}
			}
			for (int m = 0; m < 7; m++)
			{
				for (int n = -3; n < 4; n++)
				{
					sum[3] = sum[3] < img[m][j + n] && !(1 <= m && m <= 5 && -2 <=n && n <= 2) ? img[m][j + n] : sum[3];
					//flag = img[3][j+0] < img[m][j + n] ? true : flag;//���Ŀ������С����Χ�κ�һ�����ؾ�˵�����һ���������ֵ
				}
			}
			x1 = (x1.x == 0 && x1.y == 0) || n_count >= 3 ? x0 : x1;//�ж��Ƿ����5X5�����ֵ����Ŀ�����ͬ��
			int tmp = sum[0] >= sum[1] && sum[1] >= sum[2] && sum[2] >= sum[3] && (abs(x0.x - x1.x) <= 2 && abs(x0.y - x1.y) <= 2)
				? 0 : FillBlock(src, mask_tmp, Point(j, i));//tmpû���壬����Ϊ�˵��ú���ĺ�������
		}
	}
	mask = mask_tmp.clone();
}
//---������Щ���õ����ֵ
//---@
//---@
//---@gap		:���,����ɽ֮������
//---@radius	:�뾶,����ɽ֮��İ뾶

vector<Point> Find_Max(InputArray _src, Mat&mask,int gap,int radius)
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
					double x = pow((max_point[k].position.x - fill_point[count].x + j), 2); //(max_point[k].position.x - img[i][fill_point[count].x + j])*(max_point[k].position.x - img[i][fill_point[count].x + j]);
					double y = pow((max_point[k].position.y - (fill_point[count].y + i - 1)), 2); // (max_point[k].position.y - img[i][fill_point[count].y + j])*(max_point[k].position.y - img[i][fill_point[count].x + j]);
					int distance = sqrt(x + y);//�������
					if (img[i][fill_point[count].x + j] <= img[0][fill_point[count].x] - gap //��Ȳ���
						|| msk[i][fill_point[count].x + j] == 3
						//|| msk[i][fill_point[count].x + j] == 0 //�������ж����ֵ��ͱ߽����ײ
						|| (j == 0 && i == 1)
						|| distance >= radius) continue;//��ȴ���gap���Ѿ���ǣ��߽磬����ֵ����ֱ���˳�
					if (img[i][fill_point[count].x + j] == 2 || msk[i][fill_point[count].x + j] == 0) max_point[k].data = 1;
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

