#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <tchar.h>
#include "abc.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996) 

//�_�Ƃ��̌��A�ꎞ�I�ȗ��p
tmp_point *point_array[3 * MAX_NUM_TRIANGLES];
unsigned int num_tmp_points;

//�_�ɐڑ�����ӂ̃��X�g�A�ꎞ�I�ȗ��p
tmp_edge **connecting_edge;

//�_
double point[MAX_NUM_POINTS][3];
unsigned int num_points;

//��
unsigned int edge[MAX_NUM_EDGES][2];
unsigned int num_edges;

//�O�p�`�|���S���̒��_�Ǝ��ӂ̕�
unsigned int triangle[MAX_NUM_TRIANGLES][3];
unsigned int triangle_edge[MAX_NUM_TRIANGLES][3];
unsigned int num_triangles;


//�A�X�L�[�`����STL�t�@�C���̓ǂݍ���
//�ǂݍ��܂ꂽ���W�͈ꎞ�I��point_array�Ɏ��[�����
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

		//�A������R���_��ǂݍ���
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
//tmp_point *point0,*point1�͔�r�Ώۂ̓_
{
	//X���W�l�Ŕ�r
	if (point0->coord[X] < (point1->coord[X] - (EPS)))
		return -1;
	else if (point0->coord[X] > (point1->coord[X] + (EPS)))
		return 1;

	//Y���W�l�Ŕ�r
	else{
		if (point0->coord[Y] < (point1->coord[Y] - (EPS)))
			return -1;
		else if (point0->coord[Y] > (point1->coord[Y] + (EPS)))
			return 1;

		//Z���W�l�Ŕ�r
		else{
			if (point0->coord[Z] < (point1->coord[Z] - (EPS)))
				return -1;
			else if (point0->coord[Z] > (point1->coord[Z] + (EPS)))
				return 1;
			else
				return 0; //2�_�𓯈�Ƃ݂Ȃ�
		}
	}
}
//�_��̃N�C�b�N�\�[�g
void quickSort(long l, long r, tmp_point *point[])
//int l,r�̓\�[�g�͈͂̍����ƉE���̃C���f�b�N�X
//tmp_point *point[]�̓\�[�g�Ώۂ̓_��
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
		printf("�t�@�C�����J���܂���\n");
		return(false);
	}

	//����ꂽ�_�Q���\�[�g���������W�̓_����ɂ܂Ƃ߂�
	quickSort(0, num_tmp_points - 1, point_array);

	//�\�[�g�ς݂̓_���擪����X�L����
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
		//�Ō�̓_��������A�������͎Q�Ɠ_�ƈقȂ�_���݂���
		if ((i == num_tmp_points)
			|| (fabs(ref_pnt[X] - (point_array[i]->coord[X])) > (EPS))
			|| (fabs(ref_pnt[Y] - (point_array[i]->coord[Y])) > (EPS))
			|| (fabs(ref_pnt[Z] - (point_array[i]->coord[Z])) > (EPS))){
			//printf("i = %d\n",i);

			//����̓_�𓯂��_�Ƃ��ēo�^
			end = i;
			if (i > 0){
				point[num_points][X] = ref_pnt[X];
				point[num_points][Y] = ref_pnt[Y];
				point[num_points][Z] = ref_pnt[Z];
				for (j = start; j < end; j++){
					//���̓_���܂ރ|���S�������Ԗڂ̃|���S�����v�Z
					tri_index = point_array[j]->index / 3;
					//�_�����̃|���S���ɂ����邢���߂̒��_�͌v�Z
					ver_index = point_array[j]->index % 3;
					triangle[tri_index][ver_index] = num_points;
				}
				num_points++;
				//printf("num_pointsss = %d\n", num_points);
			}

			//�Q�Ɠ_�̍X�V
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
	//�ӂ̓o�^
	num_edges = 0;
	//�e�_�ɐڑ�����ӂ̋L�^�ꏊ�̏�����
	connecting_edge = (tmp_edge **)malloc(num_points * sizeof(tmp_edge));
	for (i = 0; i < num_points; i++)
		connecting_edge[i] = NULL;

	//�e�O�p�`�̎��ӂ̕ӂƓo�^�ς݂̕ӂ��r
	//i�Ԗڂ̃|���S���f�[�^��j�Ԗڂ̒��_
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

			//���o�^�̕ӂ������ēo�^
			edge[num_edges][0] = triangle[i][j];
			edge[num_edges][1] = triangle[i][(j + 1) % 3];
			//printf("i = %d, j = %d, num = %d\n", i, j, num_edges);

			//�ӂ��O�p�`�ɓo�^
			triangle_edge[i][j] = num_edges;
			num_edges++;

			//�����ɓ_�ɂ��o�^
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


