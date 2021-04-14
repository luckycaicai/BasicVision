#ifndef PARTICLE_H
#define PARTICLE_H
//�궨��һЩ���õı���
#define NF_Max 650   //�������������
#define NG_Max 100   //�������
#define NW_Max 500   //΢ȼ������
#define AL_Max 100
#include "Coordinate.h"
#include <math.h>
#include <iostream>
using namespace std;
class Particle
{
	friend ostream& operator<<(ostream &output, const Particle &right);

public:
	Particle(float x, float y, float z);
	void setP();
	float getP()const;

	//void setPBest();  //pBest��������setP()�о�����ˡ�
	Coordinate getPBest()const;

	///////////////////////���ǵ�һ�ַ����������ú㶨��ѧϰ���ӡ�����ʵ���Ͽɱ��ѧϰ����c1,c2����ʹ��Ⱥ������������˴���������ά�ȵ��ٶ����÷���ͬһ�������С�
	void setV(Coordinate gBest, float w); //wΪ�������ӡ�
	float getVx()const;
	float getVy()const;
	float getVz()const;

	void setCoordinate();
	float getX()const;
	float getY()const;
	float getZ()const;
	
	void outputFile(char Dir[])const;
	float L;	//LΪ�����ܺͣ�Լ������������ʹ��
private:
	Coordinate c;
	float p;	//pΪ�ɱ���Ӧ��
	
	Coordinate pBest;
	////////////////////��ά�Ļ���Ҫ�������ٶ�
	float Vx;
	float Vy;
	float Vz;
	static float k;//�ͷ�ϵ��
	static float Xmax, Xmin;
	static float Ymax, Ymin;
	static float Zmax, Zmin;
	static float Vxmax, Vymax, Vzmax; //������������������ٶȽ������Ƶģ�������ֻ����һ���ķ�Χ�ڡ�
	static float Vxmin, Vymin, Vzmin;
	static float c1, c2, c3; //c1,c2,c3��ѧϰ���ӡ�
						 //����Ҫ�����еĶ�����бȽ�֮����ܵõ�Ⱥ�����ţ����������ǲ�
						 //static Coordinate gBest; //�����Ⱥ������.����Ⱥ�干��һ�ݾ��У����Խ������ó�static������ע��ǧ��Ҫ��Ϊstatic�����ڳ�ʼ����Ͳ����޸ĵģ�static const����������

};

#endif