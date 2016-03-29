#define X 0
#define Y 1
#define Z 2

#define A 0
#define B 1
#define C 2
#define D 3
#define EPS 0.00001   //�\���ɔ����Ȓl
#define LARGE 1000000 //�\���ɑ傫�Ȓl

#define PI 3.141592
#define MAX_NUM_POINTS 100000
#define MAX_NUM_EDGES 300000
#define MAX_NUM_TRIANGLES 250000

//�ꎞ�I�ɗ��p����_�̍\����
typedef struct tmp_point{
	double coord[3];//���W
	unsigned int index;//�_�ɕt������C���f�b�N�X
}tmp_point;

//�ꎞ�I�ɗ��p����ӂ̍\����
typedef struct tmp_edge{
	unsigned int start;
	unsigned int end;
	tmp_edge *next;
} tmp_edge;
