
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int wjy_array[] = { 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0,
1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0 };
//---�������õ���������
uchar lut[] = { 200, 206, 220, 204, 0, 207, 0, 204, 0, 207, 221, 51, 1, 207, 221, 51,
0, 0, 221, 204, 0, 0, 0, 204, 1, 207, 221, 51, 1, 207, 221, 51 };


/*
ͼ��߽�Ϊ4
Ŀ��߽�Ϊ1(����ʹ��255)		--->> ��˼����Ϊ���ı߽磬����߽��ǲ��ϱ仯��
����Ϊ2
Ŀ��Ϊ0
ȷ��Ŀ��߽�3					--->> ��˼�Ǳ߽�1������ˮ�����ȷ�������ձ߽�
*/
void FindRidge(InputArray& _src, Mat& mask, vector<Point> Edge_Point, uchar thred = 5);
vector<Point> FindEdge(Mat& src,Mat& mask);//ֱ������Ĥȥ����
//vector<Point> FindEdge(InputArray& _src, Mat& mask);//����ԭͼȥ�������˷��������޲�ͼ��
int FillBlock(InputArray& _src, vector<Point>& Edge_Point, Mat& mask, int level, Point center);
void FloorEdge(InputArray& _src, vector<Point>& Edge_Point, Mat& mask, int level);
bool CheckData(Mat& mask, Point center);
int main(int argc,char**argv)
{
	Mat inputImage = imread("222.png", 0);
	//---Ϊд�������׼��
	threshold(inputImage, inputImage, 0, 255, THRESH_BINARY | THRESH_OTSU);

	//---���ú���
	vector<Point> Edge_Point;
	Mat mask;
	FindRidge(inputImage, mask, Edge_Point);
	return 0;
}
//----Ѱ��ɽ���ߺ���
//----@_src	��	��ֵͼ
//----@_mask:	��Ҫ�󣬸Ľ�֮��ֱ�����ڲ����в���
//----@thred:	����任֮�����ֵ����ɽ����-->>(0-10),�޲�ͼ��(����10)
void FindRidge(InputArray& _src, Mat& mask, vector<Point> Edge_Point, uchar thred)
{
	Mat src = _src.getMat();// , mask = _mask.getMat();

	//---������Ĥ���߽�Ϊ4������Ϊ2��Ŀ��Ϊ0
	mask = src.clone();
	bitwise_not(mask, mask);
	distanceTransform(mask, mask, DIST_L2, DIST_MASK_3, 5);
	normalize(mask, mask, 0, 255, NORM_MINMAX);
	mask.convertTo(mask, CV_8UC1);
	threshold(mask, mask, thred, 255, THRESH_BINARY_INV);
	Mat temp = mask.clone();

	//---�������滹û������ɵ�mask��������
	bitwise_not(mask, mask);
	mask = mask - 253;//������ʼ��Ϊ2
	Mat rows = Mat::ones(Size(src.cols, 1), CV_8UC1), cols = Mat::zeros(Size(1, src.rows), CV_8UC1);
	rows.setTo(4); cols.setTo(4);//�߽��ʼ��Ϊ4
	Mat src_rows_beg = mask.row(0),				src_cols_beg = mask.col(0);
	Mat src_rows_end = mask.row(src.rows - 1),	src_cols_end = mask.col(src.cols - 1);
	rows.copyTo(src_rows_beg); rows.copyTo(src_rows_end);
	cols.copyTo(src_cols_beg); cols.copyTo(src_cols_end);

	//---����Ŀ��߽磬������Ĥ�߽��ʼ��Ϊ1
	Edge_Point = FindEdge(temp, mask);
	//---findcontours���һ���©��,�����޲�mask
	bool TF = true;
	while (TF)
	{
		TF = false;
		for (size_t i = 0; i < mask.rows - 1; i++)
		{
			uchar* msk_up = mask.ptr(i - 1);
			uchar* msk = mask.ptr(i);
			uchar* msk_dw = mask.ptr(i + 1);
			for (size_t j = 0; j < mask.cols - 1; j++)
			{
				uchar _temp_data = msk[j];
				msk[j] = msk[j] == 2 && (msk_up[j] == 255 || msk_up[j] == 0)
					&& (msk_dw[j] == 255 || msk_dw[j] == 0)
					&& (msk[j - 1] == 255 || msk[j - 1] == 0)
					&& (msk[j + 1] == 255 || msk[j + 1] == 0) ? 0 : msk[j];
				msk[j] = msk[j] == 0 && (msk_up[j] == 2 || msk_dw[j] == 2 || msk[j - 1] == 2 || msk[j + 1] == 2) ? 2 : msk[j];
				TF = _temp_data != msk[j] ? true : TF;//�ж��Ƿ��д���ı߽�
			}
		}
	}

	//---����任֮���һ����0-255��uchar��ʽ
	distanceTransform(src, src, DIST_L2, DIST_MASK_3, 5);
	normalize(src, src, 0, 255, NORM_MINMAX);
	src.convertTo(src, CV_8UC1);

	//ֱ��ͼ���㣬���ٺ�����ˮ���ٶ�
	const int histSize = 255;
	float range[] = { 0, 255 };
	const float* histRange = { range };
	Mat hist;
	calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false);

	//��ˮ0-255
	hist = hist.reshape(1,1);//��ת������
	normalize(hist, hist, 0, 1000, NORM_MINMAX);
	hist.convertTo(hist, CV_32FC1);
	//float* ptr = hist.ptr(0);
	for (size_t level = 0; level <= 255; level++)
	{
		if (!hist.at<float>(0,level)) continue;//ֱ��ͼ���٣����Ϊ0˵��û��������ؾͲ���ѭ����// cout << "i = " << i << " " << int(ptr[i]) << endl;
		FloorEdge(src, Edge_Point, mask, level);
	}
}

/*
//----Ѱ��ͼ��ı߽�
//----@_src	:��ֵͼ������Ϊ0
//----@		:���ر߽������
vector<Point> FindEdge(InputArray& _src, Mat& mask)
{
	Mat src1 = _src.getMat(),src = src1.clone();
	//---�ȸ�ʴ��Ȼ����ȥ�ұ߽磬�߽���ǰ���߽磬�������ʴ�Ǿ��Ǳ����߽�
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(src, src, MORPH_ERODE, kernel);
	vector<Point> wjy_Point;//�洢�߽����е����ص�
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE, Point(-1, -1));
	for (size_t i = 0; i < contours.size(); i++)
	{
		for (size_t j = 0; j < contours[i].size(); j++)
		{
			wjy_Point.push_back(contours[i][j]);//�洢
			mask.at<uchar>(contours[i][j]) = 255;//��Ĥ�߽縳ֵ
		}
	}
	return wjy_Point;
}
*/

//----Ѱ��ͼ��ı߽�
//----�������д�ĺܲԭͼ����չ��ԭͼ��mask�Ǳ߽�ȴ������Ĥͼ��֮����иĽ���
//----@src	:
//----@		:���ر߽������
vector<Point> FindEdge(Mat& src,Mat& mask)
{
	//Mat src = mask.clone();
	//---�ȸ�ʴ��Ȼ����ȥ�ұ߽磬�߽���ǰ���߽磬�������ʴ�Ǿ��Ǳ����߽�
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	//morphologyEx(src, src, MORPH_ERODE, kernel);
	vector<Point> wjy_Point;//�洢�߽����е����ص�
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE, Point(-1, -1));
	for (size_t i = 0; i < contours.size(); i++)
	{
		for (size_t j = 0; j < contours[i].size(); j++)
		{
			wjy_Point.push_back(contours[i][j]);//�洢
			mask.at<uchar>(contours[i][j]) = 255;//��Ĥ�߽縳ֵ
		}
	}
	return wjy_Point;
}

//----��ˮ�жϱ߽纯��
//----@_src			������ͼ
//----@Edge_Point	: �߽��
//----@_mask		: ��Ĥͼ
//----@level		: ˮλֵ
void FloorEdge(InputArray& _src, vector<Point>& Edge_Point, Mat& mask,int level)
{
	Mat src = _src.getMat();
	for (int i = 0; i < Edge_Point.size(); i++)
	{
		//---���������ĸ�λ��
		vector<Point> temp_vector;
		temp_vector.push_back(Point(Edge_Point[i].x, Edge_Point[i].y - 1));
		temp_vector.push_back(Point(Edge_Point[i].x, Edge_Point[i].y + 1));
		temp_vector.push_back(Point(Edge_Point[i].x - 1, Edge_Point[i].y));
		temp_vector.push_back(Point(Edge_Point[i].x + 1, Edge_Point[i].y));
		uchar* msk = mask.ptr(Edge_Point[i].y);
		uchar* img = src.ptr(Edge_Point[i].y);
		if (img[Edge_Point[i].x] > level)	continue;//��ȫˮλ�����ж�
		bool Flag = true;//�ж��Ƿ��а���
		uchar count_num = 0;//�������������ĺܽ����߶Σ�����Ч�����ã����Բ�Ҫ�������
		for (size_t j = 0; j < temp_vector.size(); j++)//�����ǵ�ǰ���ص��ڻ����ˮλ
		{
			uchar* pre_data = mask.ptr(temp_vector[j].y);
			if (pre_data[temp_vector[j].x] == 2 || pre_data[temp_vector[j].x] == 4)//�������߽߱�
			{
				pre_data[temp_vector[j].x] = 2;
				continue;
			}
			else if (pre_data[temp_vector[j].x] == 128 || pre_data[temp_vector[j].x] == 255)
			{
				continue;//ȷ���߽�				
				count_num++;
			}
			else
			{
				if (src.at<uchar>(temp_vector[j]) >= level)//�Ա����ش���ˮλ
				{			
					count_num++;
					pre_data[temp_vector[j].x] = 255;//mask�߽����
					Edge_Point.push_back(temp_vector[j]);	
					//Edge_Point.insert(Edge_Point.begin()+i+1,temp_vector[j]);//ɾ����ǰ����
				}
				else//С��ˮλ�����а���
				{
					int temp = FillBlock(src, Edge_Point, mask, level, Edge_Point[i]);//����һ�������
					Flag = false;
				}
			}
		}
		if (false)//û�г��ְ��ء���Ϊ�а��صĻ�����ǰλ�ø�ֵΪ����2�������ڲ��ֱ��ȥ����
		{
			msk[Edge_Point[i].x] = 128;//���ڷ�ˮ֮��ĵ�϶�Ϊ�߽�㣬������ѷ�ڶ�ס����ȷ��Ϊ128
			Edge_Point.erase(Edge_Point.begin() + i);//ɾ����ǰ����
			i--;//��겻�ƶ���ֱ�Ӵ�����һ��
			continue;
		}
		else if (count_num == 4)
		{
			Edge_Point.push_back(Edge_Point[i]);
			Edge_Point.erase(Edge_Point.begin() + i);//ɾ����ǰ����
			i--;//��겻�ƶ���ֱ�Ӵ�����һ��
			continue;
		}
		else if (msk[Edge_Point[i].x] == 255)//û�а����ҵ�ǰֵΪδȷ���߽硢��Ϊȷ���߽�3����Ҫ�ж�
		{
			msk[Edge_Point[i].x] = CheckData(mask, Edge_Point[i]) == true ? 128 : 2;//���
			Edge_Point.erase(Edge_Point.begin() + i);//ɾ����ǰ����
			i--;//��겻�ƶ���ֱ�Ӵ�����һ��
		}
	}
}

//---��䰼��
//---@src		��input distance image/float/one signal
//---@mask		��mask image/ucahr/one signal
//---@center	��nowdays operator centers/ucahr
int FillBlock(InputArray& _src, vector<Point>& Edge_Point, Mat& mask, int level, Point center)
{
	Mat src = _src.getMat();
	mask.at<uchar>(center) = 2;//�а��ۣ���һ��������ˮ����ȥ
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
				//---��ˮλ�͡��Ҳ�Ϊ������Ϊδ������������
				if (img[i][fill_point[count].x + j] < level && !(j == 0 && i == 1) && msk[i][fill_point[count].x + j] == 0)
				{
					fill_point.push_back(Point(fill_point[count].x + j, fill_point[count].y + i - 1));
					msk[i][fill_point[count].x + j] = 2;//���Ϊ����
				}
				else if (img[i][fill_point[count].x + j] >= level && msk[i][fill_point[count].x + j] == 0)
				{
					Edge_Point.push_back(Point(fill_point[count].x + j, fill_point[count].y + i - 1));
					msk[i][fill_point[count].x + j] = 255;//���Ϊ�߽�
				}
			}
		}
		//msk[1][fill_point[count].x] = 2;//��ǰ�������Ϊ1
		count_mount = fill_point.size() - 1;//ѭ���ĺ��ģ�
		fill_point.erase(fill_point.begin());//ִ��һ��ɾ��һ������ռ���ڴ�
	}
	return 0;
}

//---�����
//---����trueΪ����ֵ��falseΪɾ��ֵ
bool CheckData(Mat& mask, Point center)
{
	uchar* msk_up = mask.ptr(center.y - 1);
	uchar* msk = mask.ptr(center.y);
	uchar* msk_dw = mask.ptr(center.y + 1);
	int num[8];
	//---�ѱ߽�255��ʵ�ʱ߽�3��Ŀ��0�������ο��������е����ˣ���ϸ��һ�¾ͺ���
	int sum = (num[0] = msk_up[center.x - 1] == 255 || msk_up[center.x - 1] == 128 || msk_up[center.x - 1] == 0 ? 1 : 0)
			+ (num[1] = msk_up[center.x] == 255 || msk_up[center.x] == 128 || msk_up[center.x] == 0 ? 1 : 0) * 2
			+ (num[2] = msk_up[center.x + 1] == 255 || msk_up[center.x + 1] == 128 || msk_up[center.x + 1] == 0 ? 1 : 0) * 4
			+ (num[3] = msk[center.x - 1] == 255 || msk[center.x - 1] == 128 || msk[center.x - 1] == 0 ? 1 : 0) * 8
			+ (num[4] = msk[center.x + 1] == 255 || msk[center.x + 1] == 128 || msk[center.x + 1] == 0 ? 1 : 0) * 16
			+ (num[5] = msk_dw[center.x - 1] == 255 || msk_dw[center.x - 1] == 128 || msk_dw[center.x - 1] == 0 ? 1 : 0) * 32
			+ (num[6] = msk_dw[center.x] == 255 || msk_dw[center.x] == 128 || msk_dw[center.x] == 0 ? 1 : 0) * 64
			+ (num[7] = msk_dw[center.x + 1] == 255 || msk_dw[center.x + 1] == 128 || msk_dw[center.x + 1] == 0 ? 1 : 0) * 128;
	return ((lut[uchar(sum / 8)] >> sum % 8) & 1) != 1 ? true : false;
}



