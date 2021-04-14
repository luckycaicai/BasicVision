#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
using namespace cv;
using namespace std;


void center_axis(InputArray _src, Mat& dst);
int main()
{
	
	Mat img = imread("hourse.jpg");
	cvtColor(img, img, COLOR_BGR2GRAY);
	threshold(img, img, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	Mat showImage;
	center_axis(img, showImage);
	string str = "./";
	imwrite(str + "7.jpg", showImage);
	//imwrite("123,jpg", showImage);
	return 0;
}
//------@_src:����Ϊ��ֵ��ͼ�񣬱���Ϊ0
//------@dst :���ΪCV_8UC1
void center_axis(InputArray _src, Mat&  dst)
{
	//---�洢λ�ú�����
	typedef struct MyStruct
	{
		Point position;
		float num;
	}MyStruct;
	//----����������/�����������⣬��ʱ����ȥ�Ľ�
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
	Mat src = _src.getMat();
	//Mat dst = _dst.getMat();
	distanceTransform(src, src, DIST_L2, DIST_MASK_3, 5);
	normalize(src, src, 0, 255, NORM_MINMAX);//ֻҪ����Ϊ0���ɣ�û��Ҫ0-255
	Mat img_row = src.reshape(0, 1);//ת��Ϊһ�����ݣ����㴦��
	vector<MyStruct> my_vector;//Ŀ�����ݡ���������
	for (size_t j = 0; j < img_row.cols; j++)//Mat�ŵ�vector���ڲ���
	{
		if (img_row.at<float>(0, j) == 0) continue;
		MyStruct my_struct;
		my_struct.num = saturate_cast<float>(img_row.at<float>(0, j));
		my_struct.position = Point(saturate_cast<int>(j % src.cols), saturate_cast<int>(j / src.cols));
		my_vector.push_back(my_struct);
	}
	//----����Ŀ�����ݣ�����λ�ú���ֵ
	for (size_t i = 0; i < my_vector.size(); i++)
	{
		if (my_vector[i].num == 0) continue;//����������
		for (size_t j = i; j < my_vector.size(); j++)
		{
			MyStruct temp;
			if (my_vector[i].num >= my_vector[j].num)
			{
				if (my_vector[j].num == 0) continue;//����������
				temp = my_vector[j];
				my_vector[j] = my_vector[i];
				my_vector[i] = temp;
			}
		}
	}
	//���ݲ�����������ݣ����ﶼΪĿ�����ݣ�����Ҫ���б����ж���
	for (size_t i = 0; i < my_vector.size(); i++)
	{
		//----�߽����ݲ��������ڸĽ�Ҳ�ܼ򵥣��������Ϳ�����
		if (my_vector[i].position.y == 1
			|| my_vector[i].position.x == 1
			|| my_vector[i].position.y == src.rows - 1
			|| my_vector[i].position.x == src.cols - 1
			|| src.at<float>(my_vector[i].position.y, my_vector[i].position.x) == 0) continue;
		else
		{
			char num[] = { 1,1,1,1,1,1,1,1 };
			//---����Ƚ�low������ȥ�����Ʊ任�ˣ��õ����Լ��Ľ���
			num[0] = src.at<float>(my_vector[i].position.y - 1, my_vector[i].position.x - 1)
	> 0 ? 1 : 0;
			num[1] = src.at<float>(my_vector[i].position.y - 1, my_vector[i].position.x)
		> 0 ? 1 : 0;
			num[2] = src.at<float>(my_vector[i].position.y - 1, my_vector[i].position.x + 1)
	> 0 ? 1 : 0;
			num[3] = src.at<float>(my_vector[i].position.y, my_vector[i].position.x - 1)
	> 0 ? 1 : 0;
			num[4] = src.at<float>(my_vector[i].position.y, my_vector[i].position.x + 1)
						> 0 ? 1 : 0;
			num[5] = src.at<float>(my_vector[i].position.y + 1, my_vector[i].position.x - 1)
						> 0 ? 1 : 0;
			num[6] = src.at<float>(my_vector[i].position.y + 1, my_vector[i].position.x)
						> 0 ? 1 : 0;
			num[7] = src.at<float>(my_vector[i].position.y + 1, my_vector[i].position.x + 1)
						> 0 ? 1 : 0;
			int sum = num[0] + num[1] * 2 + num[2] * 4 + num[3] * 8
				+ num[4] * 16 + num[5] * 32 + num[6] * 64 + num[7] * 128;
			src.at<float>(my_vector[i].position.y, my_vector[i].position.x) = ((lut[uchar(sum / 8)] >> sum % 8) & 1) != 1 ? 255 : 0;
		}
	}
	dst = src.clone();
	dst.convertTo(dst, CV_8UC1);
}

