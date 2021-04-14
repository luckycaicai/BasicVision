#define _CRT_SECURE_NO_WARNINGS
#include "Particle.h"
#include <stdio.h> //��ΪҪ�õ�sprintf������
#include <time.h>
#include <string>
#include <math.h>
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	Particle*p[40];
	float w;//ʵ������wmax=1.2f;�˴�����wmin=0.5;

	Particle*temp;
	float randX, randY, randZ;//��������������΢ȼ������
	srand((unsigned int)time(NULL));
	size_t i = 0;
	while(i<40)
	{
		//��һ�γ�ʼ�����������СֵԼ��
		randX = rand() % (NF_Max+1);//0-650
		randY = rand() % (NG_Max+1);//0-408
		randZ = rand() % (NW_Max+1);//0-6
		if (0.5*randX + 0.8*randY + 0.1 * randZ - AL_Max<0) continue;
		cout << "randX=" << randX << endl;
		cout << "randY=" << randY << endl;
		cout << "randZ=" << randZ << endl;
		p[i] = new Particle(randX, randY,randZ);
		cout << "The temp info is X:" << p[i]->getX() << " Y:" << p[i]->getY() << " Z:" << p[i]->getZ() << " P: " << p[i]->getP() << endl;
		i++;
	}
	//���ˣ������������Ⱥ�ĳ�ʼ����
	//////////////////////////////////////
	Coordinate gBest;  //ȫ�����Ž⡣
	int bestIndex = 0;
	float bestP; //��õ���Ӧ�ȡ�
	bestP = p[0]->getP();
	gBest = p[0]->getPBest();
	for (int i = 1; i<40; ++i)
	{
		if (p[i]->getP()<bestP)
		{
			bestP = p[i]->getP();
			gBest = p[i]->getPBest();
			bestIndex = i;
		}
	}

	/////////////////////////////////// 
	cout << "Now the initial gBest is X:" << gBest.x << " Y:" << gBest.y << " Z:" << gBest.z << endl;
	cout << "And the p[0] is X:" << p[0]->getX() << " Y:" << p[0]->getY() << " Z:" << p[0]->getZ() << endl;
	cout << "And the p[39] is X:" << p[39]->getX() << " Y:" << p[39]->getY() << " Z:" << p[39]->getZ() << endl;
	cout << "Now p[0].p=" << p[0]->getP() << endl;
	////////////////////////////////���ˣ��Ѿ�Ѱ�ҵ���ʼʱ����Ⱥ���š�
	char buf[20];
	for (int i = 0; i<40; ++i)
	{
		sprintf(buf, "coordinate%d.dat", i);
		ofstream out(buf, ios::out);
		out.close();
	}
	//////////////////////////��������Ϊ��ÿ����һ�ζ��ظ���ӡ�
	for (int k = 0; k<100; ++k)   //kΪ����������
	{
		w = 0.9f - (0.9f - 0.4f)*k / 99.0f;  //������Ӻ���Ҫ���Ȳ���̫��Ҳ����̫С��һ��ʼ�Լ��������õ�̫���˵��³����Լ�ͨ�����㷢�ֲ��ÿɱ�Ĺ������ӿ���ʹ�õ��Ľ���ľ�ȷ�ȸ�һ��������(�ﵽ1e-006)����������ú㶨�Ĺ������ӣ���ֻ�ܵõ�1e-005�ľ��ȡ�
											 //////////////////////////һ��ʼwmax=1.0,wmin=0.6,���Դﵽ1e-006�ľ��ȣ���Ϊ�ܸ��ˣ�����ʵ����wmax=0.9f,wmin=0.4f�����Ժ����׵شﵽ1e-11��ˮƽ�������Ѿ��ӽ�float�ľ��ȼ��ޡ�

											 // w=0.85f;

		for (int i = 0; i<40; ++i)
		{
			temp = p[i];
			temp->setV(gBest, w);
			temp->setCoordinate();
			temp->setP();
			sprintf(buf, "coordinate%d.dat", i);
			temp->outputFile(buf);
		}
		bestP = p[0]->getP();
		gBest = p[0]->getPBest();
		for (int i = 1; i<40; ++i)
		{
			temp = p[i];
			if (temp->getP()<bestP)
			{
				bestP = temp->getP();
				gBest = temp->getPBest();
				bestIndex = i;
			}

		}
		cout << "Now gBest is X:" << gBest.x << " Y:" << gBest.y << " Z:" << gBest.z << " and the minP=" << p[bestIndex]->getP() << " L: " << p[bestIndex]->L << endl;
		cout << "bestIndex=" << bestIndex << endl;

	}
	system("pause");
	return 0;
}
