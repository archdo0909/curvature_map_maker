// Project1.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>
#include <string.h>
#include <vector>
#include "abc.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996) 

#define judge 0.0001
#define e_judge 0.000000001

#define STL_FILE "C:\\Users\\LAB\\Documents\\dy\\STL Triangle\\stl\\PP-triangle_full_10kPa.stl"

//点
extern double point[MAX_NUM_POINTS][3];
extern unsigned int num_points;

//辺
extern unsigned int edge[MAX_NUM_EDGES][2];
extern unsigned int num_edges;

//三角形ポリゴンの頂点
extern unsigned int triangle[MAX_NUM_TRIANGLES][3];
extern unsigned int triangle_edge[MAX_NUM_TRIANGLES][3];
extern unsigned int num_triangles;

extern void normVec(double vec[]);
extern void normal(double p0[], double p1[], double p2[], double normal[]);
//STLファイルの読み込み
extern bool loadSTLFile(const char* STL_file);

unsigned int pair[MAX_NUM_EDGES][8][2];

typedef struct{
	int N[16];
	int T[16];
	int E[16];
	int index_t;
	int index_n;
	int pairs;
	double m_normal[3];
	double n_normal[3];
	double cosa[16];
	double cosb[16];
	double cota[16];
	double cotb[16];
	double K;
}neighbor;
static neighbor nei[MAX_NUM_POINTS];
bool close_flag = false;
bool open_flag = false;
bool close_flag_n = false;
bool open_flag_n = false;
double c[3];
int window_size_x = 1000;
int window_size_y = window_size_x;
GLfloat orange[] = { 255.0 / 256.0, 153.0 / 256.0, 0.0 / 256.0, 0.9 };
GLfloat blue[] = { 0.0 / 256.0, 65.0 / 256.0, 255.0 / 256.0, 0.4 };
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat blue2[] = { 102.0 / 256.0, 204.0 / 256.0, 255.0 / 256.0, 0.9 };
GLfloat blue_node[] = { 0.5, 0.5, 1.0, 1.0 };
bool up_flag = false;
bool down_flag = false;
int w_view;
int h_view;
int first_count = 1;
double wall_z = 0.5;
double wall_n = 5.0;
int num_count = 0;
int con_count = 0;
int tri_count = 0;
double damp_k = 1000.0;
double damp_k_normal = 20;   

double dv = 3.0;

double node_Radius = 0.08;
double View_from[3] = { 150.0, 50.0, 40.0 };  //above FEM
//double View_from[3] = {-150.0, -130.0, -1300.0};  //real data
//double View_from[3] = { 200.0, 70.0, -100.0 };
double View_to[3] = { 0.0, -20.0, 5.0 };  //FEM
//double View_to[3] = { -90.0, -70.0, -900.0 };
double View_from2[3] = { 0.0, 13.0, 0.01 };
double View_to2[3] = { 0.0, -10.0, 0.0 };
double View_from3[3] = { 0.0, 13.0, 0.01 };
double View_to3[3] = { 0.0, -10.0, 0.0 };
bool MouseFlagRight = false;
bool MouseFlagLeft = false;
bool MouseFlagMiddle = false;
bool View_point_flag = false;
GLUnurbsObj *theNurb;

void mult_matrix3x1(double *c, double *a, double *b){
	for (int i = 0; i < 3; i++){
		//for(int j=0;j<3;j++){
		c[i] = 0.0;
		for (int k = 0; k < 3; k++){
			c[i] += a[3 * i + k] * b[k];
		}
		//}
	}
}
void gaiseki_9_3(double *a, double *b){
	a[0] = 0.0;
	a[1] = -b[2];
	a[2] = b[1];
	a[3] = b[2];
	a[4] = 0.0;
	a[5] = -b[0];
	a[6] = -b[1];
	a[7] = b[0];
	a[8] = 0.0;
}
void View_control(bool vector_flag){
	double View_distance;
	double temp[5];
	temp[2] = View_from[2] - View_to[2];
	temp[1] = View_from[0] - View_to[0];
	temp[0] = pow(temp[1], 2.0) + pow(temp[2], 2.0);
	View_distance = pow(temp[0], 0.5);
	//	printf("%f\n", View_distance);
	temp[0] = View_from[2] - View_to[2];
	temp[3] = temp[0] / View_distance;
	temp[1] = View_from[0] - View_to[0];
	temp[4] = temp[1] / View_distance;
	temp[2] = atan2(temp[4], temp[3]);
	//temp[2] = acos(temp[1]);
	if (vector_flag) temp[2] = temp[2] + 0.01;
	else temp[2] = temp[2] - 0.01;
	temp[0] = View_distance * cos(temp[2]);
	temp[1] = View_distance * sin(temp[2]);
	View_from[2] = View_to[2] + temp[0];
	View_from[0] = View_to[0] + temp[1];
}
void View_control_up_down(bool vector_flag){
	double View_distance;
	double temp[5];
	temp[2] = View_from[1] - View_to[1];
	temp[1] = View_from[0] - View_to[0];
	temp[0] = pow(temp[1], 2.0) + pow(temp[2], 2.0);
	View_distance = pow(temp[0], 0.5);
	//	printf("%f\n", View_distance);
	temp[0] = View_from[1] - View_to[1];
	temp[3] = temp[0] / View_distance;
	temp[1] = View_from[0] - View_to[0];
	temp[4] = temp[1] / View_distance;
	temp[2] = atan2(temp[4], temp[3]);
	//temp[2] = acos(temp[1]);
	if (vector_flag) temp[2] = temp[2] + 0.01;
	else temp[2] = temp[2] - 0.01;
	temp[0] = View_distance * cos(temp[2]);
	temp[1] = View_distance * sin(temp[2]);
	View_from[1] = View_to[1] + temp[0];
	View_from[0] = View_to[0] + temp[1];
}
void View_control2(bool vector_flag){
	double View_distance;
	double temp[5];
	temp[2] = View_from2[2] - View_to2[2];
	temp[1] = View_from2[0] - View_to2[0];
	temp[0] = pow(temp[1], 2.0) + pow(temp[2], 2.0);
	View_distance = pow(temp[0], 0.5);
	temp[0] = View_from2[2] - View_to2[2];
	temp[3] = temp[0] / View_distance;
	temp[1] = View_from2[0] - View_to2[0];
	temp[4] = temp[1] / View_distance;
	temp[2] = atan2(temp[4], temp[3]);
	if (vector_flag) temp[2] = temp[2] + 0.01;
	else temp[2] = temp[2] - 0.01;
	temp[0] = View_distance * cos(temp[2]);
	temp[1] = View_distance * sin(temp[2]);
	View_from2[2] = View_to2[2] + temp[0];
	View_from2[0] = View_to2[0] + temp[1];
}
#if 1
void initiation(){
	
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	unsigned int s = 0;
	unsigned int h = 0;


	///Neighbor triangle
	while (i < num_points){
		while (j < num_triangles){
			while (k < 3){
				if (i == triangle[j][k]){
					nei[i].T[s] = j;
					s++;
					k++;
					//printf("i = %d, j = %d, s = %d\n", i, j, s);
					continue;
				}
				else{
					//printf("complete num = %d", i);
					k++;
					continue;
				}
			}
			j++;
			k = 0;
		}
		nei[i].index_t = s;
		j = 0;
		s = 0;
		i++;
		continue;
	}

	for (i = 0; i < num_points; i++){
		for (j = 0; j < nei[i].index_t; j++){
			//printf("i = %d, j = %d, T = %d\n", i, j, nei[i].T[j]);
		}
	}
	i = 0;
	s = 0;
	k = 0;
	j = 0;
	//Neighbor node

	while (i < num_points){
		while (s < num_edges){
			if (i == edge[s][0]){
				nei[i].N[k] = edge[s][1];
				k++;
				s++;
			}
			if (i == edge[s][1]){
				nei[i].N[k] = edge[s][0];
				k++;
				s++;
			}
			if (i != edge[s][0] && i != edge[s][1]){
				s++;
			}
		}
		nei[i].index_n = k;
		i++;
		k = 0;
		s = 0;
	}

	for (i = 0; i < num_points; i++){
		for (s = 0; s < nei[i].index_n; s++){
			//printf("i = %d, N = %d, index = %d\n", i, nei[i].N[s], nei[i].index_n);
		}
	}

	i = 0;
	j = 0;
	k = 0;

	//neighbor edge
	while (i < num_points){
		if (nei[i].index_t > 1){
			while (j < nei[i].index_n){
				while (k < num_edges){
					if ((edge[k][0] == i && edge[k][1] == nei[i].N[j]) || (edge[k][0] == nei[i].N[j] && edge[k][1] == i)){
						nei[i].E[j] = k;
						break;
					}
	//				if (edge[k][0] == nei[i].N[j] && edge[k][1] == i){
	//					nei[i].E[j] = k;
	//					break;
	//				}
					else{
						k++;
						continue;
					}
				}
				j++;
				k = 0;
				continue;
			}
		}
		else{
			i++;
			continue;
		}
		i++;
		j = 0;
		k = 0;
		continue;
	}

	

	i = 0;
	j = 0;
	s = 0;
	//unsigned int m = 0;
	unsigned int n = 0;
	//pairing
	while (i < num_points){
		if (nei[i].index_t > 1){
			while (j < nei[i].index_n){
				while (s < nei[i].index_t){
					//printf("i = %d,s = %d\n", i, s);
					if ((nei[i].N[j] == triangle[nei[i].T[s]][0] && i == triangle[nei[i].T[s]][1]) || (nei[i].N[j] == triangle[nei[i].T[s]][0] && i == triangle[nei[i].T[s]][2])
						|| (nei[i].N[j] == triangle[nei[i].T[s]][1] && i == triangle[nei[i].T[s]][2]) || (nei[i].N[j] == triangle[nei[i].T[s]][1] && i == triangle[nei[i].T[s]][0])
						|| (nei[i].N[j] == triangle[nei[i].T[s]][2] && i == triangle[nei[i].T[s]][0]) || (nei[i].N[j] == triangle[nei[i].T[s]][2] && i == triangle[nei[i].T[s]][1])){
						//printf("i = %d, j = %d, s = %d, T = %d\n", i, j, s, nei[i].T[s]);
						pair[i][j][n] = nei[i].T[s];
						n++;
						s++;
					}
					else{
						s++;
						continue;
					}
				}
				j++;
				n = 0;
				s = 0;
				continue;
			}
		}
		else{
			i++;
			continue;
		}
		//nei[i].pairs = m;
		i++;
		j = 0;
		s = 0;
		continue;
	}
	//printf("%d, %d, %d\n", triangle[197][0], triangle[197][1], triangle[197][2]);
	
	for (i = 0; i < 10; i++){
		for (j = 0; j < nei[i].index_n; j++){
			//printf("i = %d, j = %d, pair0 = %d, pair1 = %d\n", i, j, pair[i][j][0], pair[i][j][1]);
		}
	}


	n = 0;
	i = 0;
	k = 0;
	j = 0;
	double P0P1[3];
	double P0P2[3];
	double P3P2[3];
	double P3P1[3];
	
	while (i < num_points){
		if (nei[i].index_t > 1){
			while (j < nei[i].index_n){
				if (pair[i][j][1] != NULL){
					for (k = 0; k < 3; k++){
						if (i != triangle[pair[i][j][0]][k] && nei[i].N[j] != triangle[pair[i][j][0]][k]){

							/*P0P1[0] = point[triangle[pair[i][j][0]][k]][0] - point[nei[i].N[j]][0];
							P0P1[1] = point[triangle[pair[i][j][0]][k]][1] - point[nei[i].N[j]][1];
							P0P1[2] = point[triangle[pair[i][j][0]][k]][2] - point[nei[i].N[j]][2];
							P0P2[0] = point[triangle[pair[i][j][0]][k]][0] - point[i][0];
							P0P2[1] = point[triangle[pair[i][j][0]][k]][1] - point[i][1];
							P0P2[2] = point[triangle[pair[i][j][0]][k]][2] - point[i][2];*/
							P0P1[0] = point[nei[i].N[j]][0] - point[triangle[pair[i][j][0]][k]][0];
							P0P1[1] = point[nei[i].N[j]][1] - point[triangle[pair[i][j][0]][k]][1];
							P0P1[2] = point[nei[i].N[j]][2] - point[triangle[pair[i][j][0]][k]][2];
							P0P2[0] = point[i][0] - point[triangle[pair[i][j][0]][k]][0];
							P0P2[1] = point[i][1] - point[triangle[pair[i][j][0]][k]][1];
							P0P2[2] = point[i][2] - point[triangle[pair[i][j][0]][k]][2];
							nei[i].cosa[j] = ((P0P1[0] * P0P2[0]) + (P0P1[1] * P0P2[1]) + (P0P1[2] * P0P2[2])) / (sqrt(pow(P0P1[0], 2) + pow(P0P1[1], 2) + pow(P0P1[2], 2)) * sqrt(pow(P0P2[0], 2) + pow(P0P2[1], 2) + pow(P0P2[2], 2)));
							//printf("i = %d, j = %d, N = %d, pair0 = %d, pair1 = %d\n", i, j, nei[i].N[j], pair[i][j][0], pair[i][j][1]);						
						}
						if (i != triangle[pair[i][j][1]][k] && nei[i].N[j] != triangle[pair[i][j][1]][k]){
							//		/*P3P2[0] = point[triangle[pair[i][j][1]][k]][0] - point[i][0];
							//		P3P2[1] = point[triangle[pair[i][j][1]][k]][1] - point[i][1];
							//		P3P2[2] = point[triangle[pair[i][j][1]][k]][2] - point[i][2];
							//		P3P1[0] = point[triangle[pair[i][j][1]][k]][0] - point[nei[i].N[j]][0];
							//		P3P1[1] = point[triangle[pair[i][j][1]][k]][1] - point[nei[i].N[j]][1];
							//		P3P1[2] = point[triangle[pair[i][j][1]][k]][2] - point[nei[i].N[j]][2];*/
							P3P2[0] = point[i][0] - point[triangle[pair[i][j][1]][k]][0];
							P3P2[1] = point[i][1] - point[triangle[pair[i][j][1]][k]][1];
							P3P2[2] = point[i][2] - point[triangle[pair[i][j][1]][k]][2];
							P3P1[0] = point[nei[i].N[j]][0] - point[triangle[pair[i][j][1]][k]][0];
							P3P1[1] = point[nei[i].N[j]][1] - point[triangle[pair[i][j][1]][k]][1];
							P3P1[2] = point[nei[i].N[j]][2] - point[triangle[pair[i][j][1]][k]][2];
							nei[i].cosb[j] = ((P3P2[0] * P3P1[0]) + (P3P2[1] * P3P1[1]) + (P3P2[2] * P3P1[2])) / (sqrt(pow(P3P2[0], 2) + pow(P3P2[1], 2) + pow(P3P2[2], 2)) * sqrt(pow(P3P1[0], 2) + pow(P3P1[1], 2) + pow(P3P1[2], 2)));
						}
						//printf("i = %d, j = %d, pair0 = %d, pair1 = %d\n", i, j, pair[i][j][0], pair[i][j][1]);
					}
					j++;
					continue;
				}
				else{
					j++;
					continue;
				}
			}
			i++;
			j = 0;
			continue;
		}
		else{
			i++;
			continue;
		}
	}

#if 0
	for(i = 0; i < num_points; i++){
		if (nei[i].index_t > 1){
			for (j = 0; j < nei[i].index_n; j++){
				if (pair[i][j][1] != NULL){
					for (k = 0; k < 3; k++){
						if (i != triangle[pair[i][j][0]][k] && nei[i].N[j] != triangle[pair[i][j][0]][k]){
							P0P1[0] = point[nei[i].N[j]][0] - point[triangle[pair[i][j][0]][k]][0];
							P0P1[1] = point[nei[i].N[j]][1] - point[triangle[pair[i][j][0]][k]][1];
							P0P1[2] = point[nei[i].N[j]][2] - point[triangle[pair[i][j][0]][k]][2];
							P0P2[0] = point[i][0] - point[triangle[pair[i][j][0]][k]][0];
							P0P2[1] = point[i][1] - point[triangle[pair[i][j][0]][k]][1];
							P0P2[2] = point[i][2] - point[triangle[pair[i][j][0]][k]][2];
							nei[i].cosa[j] = ((P0P1[0] * P0P2[0]) + (P0P1[1] * P0P2[1]) + (P0P1[2] * P0P2[2])) / (sqrt(pow(P0P1[0], 2) + pow(P0P1[1], 2) + pow(P0P1[2], 2)) * sqrt(pow(P0P2[0], 2) + pow(P0P2[1], 2) + pow(P0P2[2], 2)));
						}
						if (i != triangle[pair[i][j][1]][k] && nei[i].N[j] != triangle[pair[i][j][1]][k]){
							P3P2[0] = point[i][0] - point[triangle[pair[i][j][1]][k]][0];
							P3P2[1] = point[i][1] - point[triangle[pair[i][j][1]][k]][1];
							P3P2[2] = point[i][2] - point[triangle[pair[i][j][1]][k]][2];
							P3P1[0] = point[nei[i].N[j]][0] - point[triangle[pair[i][j][1]][k]][0];
							P3P1[1] = point[nei[i].N[j]][1] - point[triangle[pair[i][j][1]][k]][1];
							P3P1[2] = point[nei[i].N[j]][2] - point[triangle[pair[i][j][1]][k]][2];
							nei[i].cosb[j] = ((P3P2[0] * P3P1[0]) + (P3P2[1] * P3P1[1]) + (P3P2[2] * P3P1[2])) / (sqrt(pow(P3P2[0], 2) + pow(P3P2[1], 2) + pow(P3P2[2], 2)) * sqrt(pow(P3P1[0], 2) + pow(P3P1[1], 2) + pow(P3P1[2], 2)));
						}
					}
				}
			}
		}
	}
#endif

	///////////////////////////////////////////////////////////////////////////cota, cotb
	i = 0; 
	j = 0;

	while (i < num_points){
		if (nei[i].index_t > 1){
			while (j < nei[i].index_n){
				if (pair[i][j][1] != NULL){
					nei[i].cota[j] = nei[i].cosa[j] / sqrt(1 - pow(nei[i].cosa[j], 2));
					nei[i].cotb[j] = nei[i].cosb[j] / sqrt(1 - pow(nei[i].cosb[j], 2));
					j++;
				}
				else{
					j++;
				}
			}
			i++;
			j = 0;
		}
		else{
			i++;
		}
	}

	for (i = 0; i < num_points; i++){
		if (nei[i].index_t > 1){
			for (j = 0; j < nei[i].index_n; j++){
				//printf("i = %d, j = %d, cota = %lf, cotb = %lf\n", i, j, nei[i].cota[j], nei[i].cotb[j]);
			}
		}
	}

	/////////////////////////////////////////////////////////////////////// normal
	double nrml_vec[3];
	i = 0;
	j = 0;
	while (i < num_points){
		if (nei[i].index_t > 1){
			while (j < nei[i].index_t){
				normal(point[triangle[nei[i].T[j]][0]], point[triangle[nei[i].T[j]][1]], point[triangle[nei[i].T[j]][2]], nrml_vec);
				nei[i].m_normal[0] += nrml_vec[0];
				nei[i].m_normal[1] += nrml_vec[1];
				nei[i].m_normal[2] += nrml_vec[2];
				j++;
			}
			i++;
		}
		else{
			i++;
		}
		j = 0;
	}

	////////////////////////////////////////////////////////////////////normal 正規化
	i = 0;
	while (i < num_points){
		if (nei[i].index_t > 1){
			nei[i].n_normal[0] = nei[i].m_normal[0] / sqrt(pow(nei[i].m_normal[0], 2) + pow(nei[i].m_normal[1], 2) + pow(nei[i].m_normal[2], 2));
			nei[i].n_normal[1] = nei[i].m_normal[1] / sqrt(pow(nei[i].m_normal[0], 2) + pow(nei[i].m_normal[1], 2) + pow(nei[i].m_normal[2], 2));
			nei[i].n_normal[2] = nei[i].m_normal[2] / sqrt(pow(nei[i].m_normal[0], 2) + pow(nei[i].m_normal[1], 2) + pow(nei[i].m_normal[2], 2));
			//printf("%d, %lf, %lf, %lf\n", i, nei[i].n_normal[0], nei[i].n_normal[1], nei[i].n_normal[2]);
			i++;
		}
		else{
			i++;
		}
	}

	///////////////////////////////////////////////////////////////////K
	i = 0;
	while (i < num_points){
		if (nei[i].index_t > 1){
			while (j < nei[i].index_n){
				if (pair[i][j][1] != NULL){
					nei[i].K += (0.25 / nei[i].index_n)* (nei[i].cota[j] + nei[i].cotb[j]) * ((point[i][0] - point[nei[i].N[j]][0]) * nei[i].n_normal[0] + (point[i][1] - point[nei[i].N[j]][1]) * nei[i].n_normal[1] + (point[i][2] - point[nei[i].N[j]][2]) * nei[i].n_normal[2]);
					//printf("i = %d, j = %d, K = %lf\n", i, j, nei[i].K);
					j++;
				}
				else{
					j++;
				}
			}
			i++;
			j = 0;
		}
		else{
			i++;
		}
	}
	
	for (i = 0; i < num_points; i++){
		if (nei[i].index_t > 1){
			for (j = 0; j < nei[i].index_n; j++){
				if (pair[i][j][1] != NULL){
					if (nei[i].index_t == 6){
						//nei[i].K = 0.25 * (nei[i].cota[0] + nei[i].cotb[0]) * ((point[i][0] - point[nei[i].N[0]][0]) * nei[i].n_normal[0] + (point[i][1] - point[nei[i].N[0]][1]) * nei[i].n_normal[1] + (point[i][2] - point[nei[i].N[0]][2]) * nei[i].n_normal[2]);
					}
				}
			}
		}
	}


	////////////////////////////////////////////////////////////////////checking
	for (i = 0; i < 10; i++){
		if (nei[i].index_t > 1){
			//printf("i = %d, %lf, %lf, %lf\n", i, nei[i].m_normal[0], nei[i].m_normal[1], nei[i].m_normal[2]);
		}
	}

	for (i = 0; i < 10; i++){
		for (j = 0; j < nei[i].index_n; j++){
			//printf("i = %d, j = %d, pair0 = %d, pair1 = %d, cosa = %lf, cosb = %lf\n", i, j, pair[i][j][0], pair[i][j][1], nei[i].cosa[j], nei[i].cosb[j]);
		}
	}
	
	for (i = 0; i < num_points; i++){
		//printf("i = %d, K = %lf\n", i, nei[i].K);
	}
	for (i = 0; i < num_points; i++){
		//printf("i = %d, %lf, %lf, %lf\n", i, point[i][0], point[i][1], point[i][2]);
	}
	///////////////////////////////////////////////////////////////////////////
	//TEXT out
#if 0
	FILE *fp;

	fopen_s(&fp, "curvature.txt", "w");
	for (i = 0; i < num_points; i++){
		fprintf(fp, "%lf\n", nei[i].K);
	}

	/*fopen_s(&fp, "coordinate.txt", "w");
	for (i = 0; i < num_points; i++){
		fprintf(fp, "%lf %lf %lf\n", point[i][0], point[i][1], point[i][2]);
	}*/
	fclose(fp);
#endif
}
#endif
void node_simulation(int view_con){
	if (first_count == 1){
		initiation();
		first_count--;
	}

	int i;
	int j;
	double max = 0.00375;
	double min = -0.00375;

	for (i = 0; i < num_points; i++){
		if (nei[i].K > max){
			nei[i].K = max;
		}
		if (nei[i].K < min){
			nei[i].K = min;
		}
	}
	//printf("max = %lf, min = %lf\n", max, min);


	glPushMatrix();
	//glCullFace(GL_FRONT);	
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, blue2);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glBegin(GL_TRIANGLES);
	/*for (i = 0; i < num_triangles; i++){
		glVertex3d(point[triangle[i][0]][0], point[triangle[i][0]][1], point[triangle[i][0]][2]);
		glVertex3d(point[triangle[i][1]][0], point[triangle[i][1]][1], point[triangle[i][1]][2]);
		glVertex3d(point[triangle[i][2]][0], point[triangle[i][2]][1], point[triangle[i][2]][2]);
	}*/
#if 1
	double nrml_vec[3];
	for (i = 0; i < num_triangles; i++){
		for (j = 0; j < 3; j++){
			//if (nei[i].K > -0.07){
			//glColor3d(0.0, 0.0, 1.0);
				//normal(point[triangle[nei[i].T[j]][0]], point[triangle[nei[i].T[j]][1]], point[triangle[nei[i].T[j]][2]], nrml_vec);
				//glNormal3d(nrml_vec[0], nrml_vec[1], nrml_vec[2]);
				if (nei[triangle[i][j]].K < (max - min) / 2){
					//printf("%lf, %lf\n", point[triangle[i][j]][0], point[triangle[i][j]][1]);
					//glColor3d(0.0, (nei[triangle[i][j]].K - min) / ((max + min) / 2 - min), 1.0 - (nei[triangle[i][j]].K - min) / ((max + min) / 2 - min));
					glColor3d(0.0, (nei[triangle[i][j]].K - min) / ((max + min) / 2 - min), ((max + min) / 2.0 - nei[triangle[i][j]].K) / ((max + min) / 2.0 - min));
					glVertex3d(point[triangle[i][j]][0], point[triangle[i][j]][1], point[triangle[i][j]][2]);

				}
				else{
					//glColor3d((nei[triangle[i][j]].K - min) / ((max + min) / 2 - min), 1.0 - (nei[triangle[i][j]].K - min) / ((max + min) / 2 - min), 0.0);
					glColor3d((nei[triangle[i][j]].K - (max + min) / 2.0) / ((max + min) / 2 - min), 0.0, (max - nei[triangle[i][j]].K) / ((max + min) / 2 - min));
					glVertex3d(point[triangle[i][j]][0], point[triangle[i][j]][1], point[triangle[i][j]][2]);
				}
		//	}
		}
	}

#endif

	
	glEnd();
	
	glPopMatrix();
}
void idle(void)
{
	glutPostRedisplay();
}
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (MouseFlagLeft){
		if (View_point_flag) View_control(false);
		else View_control2(false);
	}
	else if (MouseFlagRight){
		if (View_point_flag) View_control(true);
		else View_control2(true);
	}
	if (up_flag) View_control_up_down(true);
	if (down_flag) View_control_up_down(false);

	gluLookAt(View_from[0], View_from[1], View_from[2], View_to[0], View_to[1], View_to[2], 0.0, 1.0, 0.0);
	glViewport(0, 0, w_view * 2.0 / 3.0, h_view);
	glPushMatrix();
	node_simulation(1);
	glPopMatrix();

	glutSwapBuffers();
	//printf("%d\n", first_count);
}
void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		switch (state) {
		case GLUT_UP:
			if (MouseFlagLeft){
				MouseFlagLeft = false;
			}
			break;
		case GLUT_DOWN:
			MouseFlagLeft = true;
			if (x < window_size_x * 2 / 3) View_point_flag = true;
			else View_point_flag = false;
			break;
		default:
			break;
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		switch (state) {
		case GLUT_UP:
			if (MouseFlagRight) MouseFlagMiddle = false;
			break;
		case GLUT_DOWN:
			MouseFlagMiddle = true;
			break;
		default:
			break;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		switch (state) {
		case GLUT_UP:
			if (MouseFlagRight) MouseFlagRight = false;
			break;
		case GLUT_DOWN:
			MouseFlagRight = true;
			if (x < window_size_x * 2 / 3) View_point_flag = true;
			else View_point_flag = false;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
void resize(int w, int h)
{
	w_view = w;
	h_view = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(50.0, (double)w / (double)h * 2.0 / 3.0, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);

}
void keyboard(unsigned char key, int x, int y){
	switch (key){
	case 'r':
		close_flag = true;
		break;
	case 'e':
		close_flag = false;
		break;
	case 'w':
		open_flag = true;
		break;
	case 'q':
		open_flag = false;
		break;
	case 'y':
		close_flag_n = true;
		break;
	case 'u':
		close_flag_n = false;
		break;
	case 'i':
		open_flag_n = true;
		break;
	case 'o':
		open_flag_n = false;
		break;
	}
}
void init(){

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat direction[] = { 0.0, 1.0, 0.0 };
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//アルファの設定
	glEnable(GL_BLEND);//アルファのブレンド有効
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	glLightfv(GL_LIGHTING, GL_SPOT_DIRECTION, direction);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);

	loadSTLFile(STL_FILE);

}
int main(int argc, char *argv[])
{
	//get_info();

	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_size_x, window_size_y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutInitWindowPosition(0, 0);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	//node_simulation(1);
	glutMainLoop();


	//printf("%d\n", first_count);
	return 0;

}




