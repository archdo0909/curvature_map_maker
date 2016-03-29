#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <tchar.h>
#include "abc.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996) 

//点とその個数、一時的な利用
tmp_point *point_array[3 * MAX_NUM_TRIANGLES];
unsigned int num_tmp_points;

//点に接続する辺のリスト、一時的な利用
tmp_edge **connecting_edge;

//点
double point[MAX_NUM_POINTS][3];
unsigned int num_points;

//辺
unsigned int edge[MAX_NUM_EDGES][2];
unsigned int num_edges;

//三角形ポリゴンの頂点と周辺の辺
unsigned int triangle[MAX_NUM_TRIANGLES][3];
unsigned int triangle_edge[MAX_NUM_TRIANGLES][3];
unsigned int num_triangles;


//アスキー形式のSTLファイルの読み込み
//読み込まれた座標は一時的にpoint_arrayに収納される
bool readASCIISTLFile(const char* STL_file){

	double x, y, z;
	char line[100];
	static char dummy[100];
	FILE *in;
	tmp_point *tmp_pnt;

	//stl file open
	//printf("%s\n", STL_file);
	in = fopen(STL_file, "r");
	if (in == NULL)
		return false;
	//read coordinates from files
	printf("reading file\n");
	num_tmp_points = 0;
	num_triangles = 0;
	while (fgets(line, 100, in) != NULL){
		if (strstr(line, "vertex") == NULL)
			continue;

		//連続する３頂点を読み込み
		sscanf(line, "%s %lf %lf %lf", dummy, &x, &y, &z);
		tmp_pnt = (tmp_point *)malloc(sizeof(tmp_point));
		point_array[num_tmp_points] = tmp_pnt;
		tmp_pnt->coord[X] = x;
		tmp_pnt->coord[Y] = y;
		tmp_pnt->coord[Z] = z;
		tmp_pnt->index = num_tmp_points;
		num_tmp_points++;

		fgets(line, 100, in);
		sscanf(line, "%s %lf %lf %lf", dummy, &x, &y, &z);
		tmp_pnt = (tmp_point *)malloc(sizeof(tmp_point));
		point_array[num_tmp_points] = tmp_pnt;
		tmp_pnt->coord[X] = x;
		tmp_pnt->coord[Y] = y;
		tmp_pnt->coord[Z] = z;
		tmp_pnt->index = num_tmp_points;
		num_tmp_points++;

		fgets(line, 100, in);
		sscanf(line, "%s %lf %lf %lf", dummy, &x, &y, &z);
		tmp_pnt = (tmp_point *)malloc(sizeof(tmp_point));
		point_array[num_tmp_points] = tmp_pnt;
		tmp_pnt->coord[X] = x;
		tmp_pnt->coord[Y] = y;
		tmp_pnt->coord[Z] = z;
		tmp_pnt->index = num_tmp_points;
		num_tmp_points++;
		num_triangles++;
	}
	fclose(in);
	if (num_triangles > 0){
		printf("complete!\n");
		//printf("num_points = %d\n", num_tmp_points);
	}
	else
		printf("failure...\n");
	return(num_triangles > 0);

}
int compare(tmp_point *point0, tmp_point *point1)
//tmp_point *point0,*point1は比較対象の点
{
	//X座標値で比較
	if (point0->coord[X] < (point1->coord[X] - (EPS)))
		return -1;
	else if (point0->coord[X] > (point1->coord[X] + (EPS)))
		return 1;

	//Y座標値で比較
	else{
		if (point0->coord[Y] < (point1->coord[Y] - (EPS)))
			return -1;
		else if (point0->coord[Y] > (point1->coord[Y] + (EPS)))
			return 1;

		//Z座標値で比較
		else{
			if (point0->coord[Z] < (point1->coord[Z] - (EPS)))
				return -1;
			else if (point0->coord[Z] > (point1->coord[Z] + (EPS)))
				return 1;
			else
				return 0; //2点を同一とみなす
		}
	}
}
//点列のクイックソート
void quickSort(long l, long r, tmp_point *point[])
//int l,rはソート範囲の左側と右側のインデックス
//tmp_point *point[]はソート対象の点列
{
	int i, j;
	tmp_point *m_p, *swap;
	if (l<r){
		m_p = point[(l + r) / 2];
		i = l - 1;
		j = r + 1;
		while (true){
			while (compare(point[++i], m_p) == -1){};
			while (compare(point[--j], m_p) == 1){};
			if (i >= j)break;
			swap = point[i];
			point[i] = point[j];
			point[j] = swap;
		}
		quickSort(l, i - 1, point);
		quickSort(j + 1, r, point);
	}
}
bool loadSTLFile(const char* STL_file){

	unsigned int i, j, start, end;
	unsigned int tri_index, ver_index;
	double ref_pnt[3] = { LARGE, LARGE, LARGE };
	tmp_edge *ed, *new_ed, *next_ed;

	//printf("a\n");

	if (readASCIISTLFile(STL_file))
		printf("Triangles: %d ", num_triangles);
	else{
		printf("ファイルが開けません\n");
		return(false);
	}

	//得られた点群をソートし同じ座標の点を一つにまとめる
	quickSort(0, num_tmp_points - 1, point_array);

	//ソート済みの点列を先頭からスキャン
	num_points = 0;
	for (i = 0; i < 3; i++){
		//printf("i =%d, %lf, %lf, %lf\n",i, point_array[i]->coord[X], point_array[i]->coord[Y], point_array[i]->coord[Z]);
	}
	//register
	/*	for (i = 0; i < num_tmp_points; i++){
			point[num_points][X] = point_array[i]->coord[X];
			point[num_points][Y] = point_array[i]->coord[Y];
			point[num_points][Z] = point_array[i]->coord[Z];
			num_points++;
			printf("num_p = %d\n", num_points);
			printf("%f, %f, %f\n", point[num_points][X], point[num_points][Y], point[num_points][Z]);
		}*/
	//printf("tmp_points = %d\n", num_tmp_points);
	for (i = 0; i <= num_tmp_points; i++){
		//最後の点が見つかる、もしくは参照点と異なる点がみつかる
		if ((i == num_tmp_points)
			|| (fabs(ref_pnt[X] - (point_array[i]->coord[X])) > (EPS))
			|| (fabs(ref_pnt[Y] - (point_array[i]->coord[Y])) > (EPS))
			|| (fabs(ref_pnt[Z] - (point_array[i]->coord[Z])) > (EPS))){
			//printf("i = %d\n",i);

			//同一の点を同じ点として登録
			end = i;
			if (i > 0){
				point[num_points][X] = ref_pnt[X];
				point[num_points][Y] = ref_pnt[Y];
				point[num_points][Z] = ref_pnt[Z];
				for (j = start; j < end; j++){
					//その点を含むポリゴンが何番目のポリゴンか計算
					tri_index = point_array[j]->index / 3;
					//点がそのポリゴンにおけるいくつめの頂点は計算
					ver_index = point_array[j]->index % 3;
					triangle[tri_index][ver_index] = num_points;
				}
				num_points++;
				//printf("num_pointsss = %d\n", num_points);
			}

			//参照点の更新
			if (end < num_tmp_points){
				ref_pnt[X] = point_array[end]->coord[X];
				ref_pnt[Y] = point_array[end]->coord[Y];
				ref_pnt[Z] = point_array[end]->coord[Z];
				start = end;
			}
		}
	}
	
	for (i = 0; i < num_tmp_points; i++)
		free(point_array[i]);
	//辺の登録
	num_edges = 0;
	//各点に接続する辺の記録場所の初期化
	connecting_edge = (tmp_edge **)malloc(num_points * sizeof(tmp_edge));
	for (i = 0; i < num_points; i++)
		connecting_edge[i] = NULL;

	//各三角形の周辺の辺と登録済みの辺を比較
	//i番目のポリゴンデータのj番目の頂点
	printf("num_point1 = %d\n", num_points);
	for (i = 0; i < num_triangles; i++){
		for (j = 0; j < 3; j++){
			ed = connecting_edge[triangle[i][j]];

			while (ed != NULL){
				if (((ed->start == triangle[i][j])
					&& (ed->end == triangle[i][(j + 1) % 3]))
					|| ((ed->start == triangle[i][(j + 1) % 3])
					&& (ed->end == triangle[i][j])))
					goto try_next;
				ed = ed->next;
			}

			//未登録の辺を見つけて登録
			edge[num_edges][0] = triangle[i][j];
			edge[num_edges][1] = triangle[i][(j + 1) % 3];
			//printf("i = %d, j = %d, num = %d\n", i, j, num_edges);

			//辺を三角形に登録
			triangle_edge[i][j] = num_edges;
			num_edges++;

			//同時に点にも登録
			new_ed = (tmp_edge *)malloc(sizeof(tmp_edge));
			new_ed->start = triangle[i][j];
			new_ed->end = triangle[i][(j + 1) % 3];
			new_ed->next = connecting_edge[triangle[i][j]];
			connecting_edge[triangle[i][j]] = new_ed;
			new_ed = (tmp_edge *)malloc(sizeof(tmp_edge));
			new_ed->start = triangle[i][(j + 1) % 3];
			new_ed->end = triangle[i][j];
			new_ed->next = connecting_edge[triangle[i][(j + 1) % 3]];
			connecting_edge[triangle[i][(j + 1) % 3]] = new_ed;
		try_next:;
		}
	}
	for (i = 0; i < num_points; i++){
		ed = connecting_edge[i];
		while (ed != NULL){
			next_ed = ed->next;
			free(ed);
			ed = next_ed;
		}
	}
	free(connecting_edge);
	printf("Edges:%d ", num_edges);
	printf("Points:%d \n", num_points);

	//printf("%d, %d, %d\n", triangle[119][0], triangle[119][1], triangle[119][2]);
	//printf("%d, %d, %d\n", triangle_edge[119][0], triangle_edge[119][1], triangle_edge[119][2]);

	return(true);

}


