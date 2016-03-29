#include<iostream>
#include<tchar.h>
#include<math.h>

#include"abc.h"

//vec0とvec1の内積の計算
double dot(double vec0[], double vec1[])
{
	return (vec0[X] * vec1[X] + vec0[Y] * vec1[Y] + vec0[Z] * vec1[Z]);
}

//vec0とvec1の外積
void cross(double vec0[], double vec1[], double vec2[])
{
	vec2[X] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
	vec2[Y] = vec0[Z] * vec1[X] - vec0[X] * vec1[Z];
	vec2[Z] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];
}

//ベクトルの正規化
void normVec(double vec[])
{
	double norm;
	norm = sqrt(vec[X] * vec[X] + vec[Y] * vec[Y] + vec[Z] * vec[Z]);
	vec[X] /= norm;
	vec[Y] /= norm;
	vec[Z] /= norm;
}

//単位法線ベクトルの計算
void normal(double p0[], double p1[], double p2[], double normal[])
{
	unsigned int i;
	double v0[3], v1[3];

	//基本となる２つのベクトルの生成
	for (i = 0; i < 3; i++){
		v0[i] = p2[i] - p1[i];
		v1[i] = p0[i] - p1[i];
	}

	//生成したベクトルの外積を計算
	cross(v0, v1, normal);

	//外積によって得られた法線ベクトルを正規化
	normVec(normal);
}