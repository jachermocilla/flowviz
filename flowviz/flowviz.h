#ifndef __FLOWVIZ_H__
#define __FLOWVIZ_H__

/* Maximum dimensions of the raster */
#define MAX_WIDTH 	1000
#define MAX_LENGTH 	6001

/* Maximum elevation and capacity */
#define MAX_ELEVATION   100
#define MAX_CAPACITY	100

/* Constants for directions */

#define CENTER	0
#define E	1
#define SE	2
#define S	4
#define SW	8
#define	W	16
#define	NW	32
#define N	64
#define NE	128

/* Index constants */
#define I_NW 	0
#define I_N 	1
#define I_NE	2
#define I_W	3
#define I_CENTER	4
#define I_E	5
#define I_SW	6
#define I_S	7
#define I_SE	8


/* A cell */
typedef struct _cell {
	int x, y;	
	double elevation;
	double capacity;
	int catchment;
}*Cell;

/* A node in a flow map linked list */
typedef struct _flow_map{
	int direction[MAX_LENGTH][MAX_WIDTH];
	struct _flow_map *next;
}*FlowMap;

/* Grid */
typedef struct _grid {
	double width, length;
	Cell cells[MAX_LENGTH][MAX_WIDTH];
	FlowMap flow_map;
}*Grid;

typedef struct _point {
	int x,y,z;
}*Point;

Grid grid;


/*io.c*/
Grid load_raw(char *fname);

/*utils.c*/
Grid generate_random_grid(int width, int length);
void print_cell(Cell cel);
void print_grid(Grid grd);
void print_flow_map(Grid g);

/*d8.c*/
void d8(Grid g);


/*view3d.c*/
int view3d( int argc, char **argv );


#endif

