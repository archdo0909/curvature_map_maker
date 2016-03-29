#define X 0
#define Y 1
#define Z 2

#define A 0
#define B 1
#define C 2
#define D 3
#define EPS 0.00001   //十分に微小な値
#define LARGE 1000000 //十分に大きな値

#define PI 3.141592
#define MAX_NUM_POINTS 100000
#define MAX_NUM_EDGES 300000
#define MAX_NUM_TRIANGLES 250000

//一時的に利用する点の構造体
typedef struct tmp_point{
	double coord[3];//座標
	unsigned int index;//点に付随するインデックス
}tmp_point;

//一時的に利用する辺の構造体
typedef struct tmp_edge{
	unsigned int start;
	unsigned int end;
	tmp_edge *next;
} tmp_edge;
