#include<iostream>
#include<tchar.h>
#include<math.h>

#include"abc.h"

//vec0��vec1�̓��ς̌v�Z
double dot(double vec0[], double vec1[])
{
	return (vec0[X] * vec1[X] + vec0[Y] * vec1[Y] + vec0[Z] * vec1[Z]);
}

//vec0��vec1�̊O��
void cross(double vec0[], double vec1[], double vec2[])
{
	vec2[X] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
	vec2[Y] = vec0[Z] * vec1[X] - vec0[X] * vec1[Z];
	vec2[Z] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];
}

//�x�N�g���̐��K��
void normVec(double vec[])
{
	double norm;
	norm = sqrt(vec[X] * vec[X] + vec[Y] * vec[Y] + vec[Z] * vec[Z]);
	vec[X] /= norm;
	vec[Y] /= norm;
	vec[Z] /= norm;
}

//�P�ʖ@���x�N�g���̌v�Z
void normal(double p0[], double p1[], double p2[], double normal[])
{
	unsigned int i;
	double v0[3], v1[3];

	//��{�ƂȂ�Q�̃x�N�g���̐���
	for (i = 0; i < 3; i++){
		v0[i] = p2[i] - p1[i];
		v1[i] = p0[i] - p1[i];
	}

	//���������x�N�g���̊O�ς��v�Z
	cross(v0, v1, normal);

	//�O�ςɂ���ē���ꂽ�@���x�N�g���𐳋K��
	normVec(normal);
}