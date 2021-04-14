
#include "Particle.h"
#include <fstream>
#include <iostream>
using namespace std;
float Particle::Xmax = NF_Max;   //�Լ�ͨ������֤����ͨ�������ٶȺ�λ������ȡ�������õ�Ч�������Ժ����׵شﵽ1e-006�ľ������ϣ��������ܴﵽ1e-009�ľ��ȡ�ԭ�����������Ⱥ���е����Ӷ��ܸ���ؼ��е���Ӧ�ȸߵ�λ�ã���ȻȺ�����Ž�͸��á�
float Particle::Xmin = 0;
float Particle::Ymax = NG_Max;
float Particle::Ymin = 0;
float Particle::Zmax = NW_Max;
float Particle::Zmin = 0;
float Particle::Vxmax = (Xmax - Xmin)/5;  //ͨ������Vmax=Xmax-Xmin;  
float Particle::Vxmin = (0 - Vxmax);
float Particle::Vymax = (Ymax - Ymin)/5;
float Particle::Vymin = (0 - Vymax);
float Particle::Vzmax = (Zmax - Zmin)/5;
float Particle::Vzmin = (0 - Vzmax);

float Particle::k = 2;//�ͷ�ϵ��,������������ֵ�Ӵ󴦷�

float Particle::c1 = 2.0f;
float Particle::c2 = 2.0f;
float Particle::c3 = 2.0f;

Particle::Particle(float x, float y, float z)
{
	c.x = x;
	c.y = y;
	c.z = z;
	//p=100.0f; //�ȸ���һ���ϴ����Ӧ��ֵ����������Ҫ�õ�����һ����С����Ӧֵ����
	L = 0.5*c.x + 0.8*c.y + 0.1 * c.z - AL_Max;
	p = 1.2*c.x + 0.1764*c.y + 0.11*c.z + 258.6 + k*(0 <= L ? 0 : L*L);
	Vx = (Xmax - Xmin) / 8.0f;  //////////////////////�����Ȳ��õ�һ�ֳ�ʼ��������������������һ����ͬ�ĳ�ʼ�ٶ�,ΪVmax/8.0f,��Vmax=Xmax-Xmin=30-0=30;ע�������ʼ�ٶ�ǧ����̫���Լ����Է������Ĵ�С�����ս���ľ���Ӱ��Ҳ�ܴ��м�������������Ȼ��Ҳ����̫С���Լ����Է�����(Xmax-Xmin)/8.0fʱ���Եõ��Ƚϸߵľ��ȡ�
	Vy = (Ymax - Ymin) / 8.0f;
	Vz = (Zmax - Zmin) / 8.0f;

	///��ʼʱ��pBest

	pBest.x = x;
	pBest.y = y;
	pBest.z = z;

}

void Particle::setP()   //������������Ӧ��ֵp�����ã�Ҳ����˾ֲ�����pBest�����ã����Բ���Ҫ�����һ��setPBest()���� ��
{
	L = 0.5*c.x + 0.8*c.y + 0.1 * c.z - AL_Max;
	float temp = 1.2*c.x + 0.1764*c.y + 0.11*c.z + 258.6 + k*(0 <= L ? 0 : L*L);
	if (temp<p)
	{
		p = temp;
		pBest = c;
	}
}

float Particle::getP()const
{
	return p;
}

Coordinate Particle::getPBest()const
{
	return pBest;
}

void Particle::setV(Coordinate gBest, float w)  //wΪ�������ӣ�
{
	float r1, r2, r3;
	r1 = rand() / (float)RAND_MAX;
	r2 = rand() / (float)RAND_MAX;
	r3 = rand() / (float)RAND_MAX;
	Vx = w*Vx + c1*r1*(pBest.x - c.x) + c2*r2*(gBest.x - c.x) + c3*r3*(gBest.x - c.x);
	if (Vx>Vxmax)
		Vx = Vxmax;
	else if (Vx<Vxmin)
		Vx = Vxmin;
	Vy = w*Vy + c1*r1*(pBest.y - c.y) + c2*r2*(gBest.y - c.y) + c3*r3*(gBest.y - c.y);
	if (Vy>Vymax)
		Vy = Vymax;
	else if (Vy<Vymin)
		Vy = Vymin;
	Vz = w*Vz + c1*r1*(pBest.z - c.z) + c2*r2*(gBest.z - c.z) + c3*r3*(gBest.z - c.z);
	if (Vz>Vzmax)
		Vz = Vzmax;
	else if (Vx<Vxmin)
		Vz = Vzmin;
}

float Particle::getVx()const
{
	return Vx;
}
float Particle::getVy()const
{
	return Vy;
}
float Particle::getVz()const
{
	return Vz;
}

//////////////////////�����Ƚ���setV()�Ĳ���Ȼ����ܽ����ⲽ����������ӵľͲ��ǵ�k+1�ε��ٶ��ˡ�
void Particle::setCoordinate()
{
	c.x = c.x + Vx;
	if (c.x>Xmax)
		c.x = Xmax;
	else if (c.x<Xmin)
		c.x = Xmin;
	c.y = c.y + Vy;
	if (c.y>Ymax)
		c.y = Ymax;
	else if (c.y<Ymin)
		c.y = Ymin;
	c.z = c.z + Vz;
	if (c.z>Zmax)
		c.z = Zmax;
	else if (c.z<Zmin)
		c.z = Zmin;
}

float Particle::getX()const
{
	return c.x;
}
float Particle::getY()const
{
	return c.y;
}
float Particle::getZ()const
{
	return c.z;
}

void Particle::outputFile(char Dir[])const
{
	ofstream out(Dir, ios::app);  //������Ӱɣ�

	out << this->getX() << " " << this->getY() << " " <<this->getZ() << " " << pBest.x << " " << pBest.y << " " <<pBest.z << endl;
	out.close();

}

ostream& operator<<(ostream &output, const Particle &right)
{
	output << "Now the current coordinates is X:" << right.getX() << " Y:" << right.getY() << " Z:" << right.getZ() << endl;
	output << "And the pBest is X:" << right.getPBest().x << " Y:" << right.getPBest().y << " Z:" << right.getPBest().z << endl;
	return output;
}
