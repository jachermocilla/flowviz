#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_WIDTH 	1024
#define MAX_HEIGHT 	1024

#define MAX_ELEVATION   100
#define MAX_CAPACITY	100

#define NW 	0
#define N 	1
#define NE	2
#define W	3
#define CENTER	4
#define E	5
#define SW	6
#define S	7
#define SE	8


typedef struct _cell {
	int x, y;	
	double elevation;
	double capacity;
	int catchment;
}*cell;

typedef struct _grid {
	double width, height;
	cell cells[MAX_WIDTH][MAX_HEIGHT];
	int flowmap[MAX_WIDTH][MAX_HEIGHT];
}*grid;

typedef struct _flow {
	double t;
	struct _cell cell;
	double amount;
	struct _flow *next_flow;
}*flow;

typedef struct _point {
	int x,y,z;

}*point;

grid load_grid(char *fname){
	FILE *fp = fopen(fname,"r");
        int width;
        int height;
	grid grd;
        int i,j;
	cell cel;
	int tmp;
	FILE *raw = fopen("makiling.raw","wb");

	fscanf(fp,"%d %d", &width,&height);
	printf("%d %d\n",width,height);
	grd = (grid)malloc(sizeof(struct _grid));
	grd->width=width;
	grd->height=height;

	for (i=0;i< height; i++){
		for (j=0; j < width;j++){
			cel=(cell)malloc(sizeof(struct _cell));
			cel->x = j;
			cel->y = i;
			fscanf(fp,"%d",&tmp);
			fputc((unsigned char)tmp,raw);
			cel->elevation=(double)tmp;
			cel->capacity=1;	//assumed unit capacity
			grd->cells[i][j] = cel;
		}
	}
	fclose(fp);
	fclose(raw);
	return grd;
}


grid generate_random_grid(int width, int height){
	int i,j;
	cell cel;

	unsigned int iseed = (unsigned int)time(NULL);
        srand (iseed);

	grid grd = (grid)malloc(sizeof(struct _grid));
	grd->width=width;
	grd->height=height;

	for (i=0;i< height; i++){
		for (j=0; j < width;j++){
			cel=(cell)malloc(sizeof(struct _cell));
			cel->x = j;
			cel->y = i;
			cel->elevation = rand()%MAX_ELEVATION;
			cel->capacity = rand()%MAX_CAPACITY;
			cel->catchment = (int)(rand()%2);
			grd->cells[i][j] = cel;
		}
	}
	return grd;
}

void print_cell(cell cel){
	/*
	printf("--------------\n");
	printf("x, y: %d, %d\n",cel->x,cel->y);
	printf("elevation: %f\n",cel->elevation);
	printf("capacity: %f\n",cel->capacity);
	printf("catchment: %d\n",cel->catchment);
	printf("--------------\n");
	*/
	printf(" %2.f ",cel->elevation);

}


void print_grid(grid grd){
	int i,j;
	cell cel;	

	for (i=0;i<grd->height;i++){
		for (j=0;j<grd->width;j++){
			cel = grd->cells[i][j];
			print_cell(cel);			
		}
		printf("\n");
	}
}


void get_neighbors(int x, int y, grid g, double neighbors[9]){
	int i;
	
	for (i=0;i<9;i++)
		neighbors[i]=9999999;

	if (y >= 1 && y<(g->height-1)){
		if (x>=1 && x<(g->width-1)){
			neighbors[NW]=g->cells[y-1][x-1]->elevation;
			neighbors[N]=g->cells[y-1][x]->elevation;
			neighbors[NE]=g->cells[y-1][x+1]->elevation;
			
			neighbors[W]=g->cells[y][x-1]->elevation;
			neighbors[CENTER]=g->cells[y][x]->elevation;
			neighbors[E]=g->cells[y][x+1]->elevation;
			
			neighbors[SW]=g->cells[y+1][x-1]->elevation;
			neighbors[S]=g->cells[y+1][x]->elevation;
			neighbors[SE]=g->cells[y+1][x+1]->elevation;
		}	
	}
	//printf("Here %d %d\n",x,y);	

}

int get_min_index(double nbors[9]){
	double min=99999999;
	int min_index=-1;
	int i;

	for (i=0;i<9;i++){
		if (nbors[i] < min){
			min=nbors[i];
			min_index=i;
		}
	}
	return min_index;
}

void d8(grid g){
	double nbors[9];
	int i,j,k,x,y;

	for (y=0;y<(g->height);y++){
		for (x=0;x<(g->width);x++){
			get_neighbors(x,y,g,nbors);
			g->flowmap[y][x]=get_min_index(nbors);
		}
	}
	//printf("flow at (1,1): %d\n",g->flowmap[1][1]);
}

void print_flowmap(grid g){
	int i,j,k,x,y;
	for (y=0;y<(g->height);y++){
		for (x=0;x<(g->width);x++){
			printf(" %d ",g->flowmap[y][x]);
		}
		printf("\n");
	}
}

void print_trace(int data[MAX_HEIGHT][MAX_WIDTH], int w,int h){
	int i,j,k,x,y;
	for (y=0;y<h;y++){
		for (x=0;x<w;x++){
			printf(" %d ",data[y][x]);
		}
		printf("\n");
	}

}


void get_point_from_dir(int dir, point p){
	
	switch (dir){
		case NW: p->x-=1;p->y-=1;break;
		case N: p->y-=1; break;
		case NE: p->x+=1; p->y-=1;break;
		case W: p->x-=1; break;
		case CENTER: break;
		case E: p->x+=1; break;
		case SW: p->x-=1;p->y+=1; break;
		case S: p->y+=1; break;
		case SE: p->x+=1;p->y+=1; break;
	}
}

void trace_flow(struct _point points[], int n, grid g, int maxsteps){
	int cx, cy;
	int i,j,t;
	int display[MAX_HEIGHT][MAX_WIDTH];
	int flowdir;
	struct _point p;

	for (t=0;t<maxsteps;t++){
		printf("Press any key to continue...");
		getchar();
		for (i=0;i<n;i++){
			p.x=points[i].x;
			p.y=points[i].y;
			flowdir = g->flowmap[p.y][p.x];
			printf("----%d %d %d\n", p.x,p.y, flowdir);
			display[p.y][p.x]=(i+1);
			get_point_from_dir(flowdir, &p);
			printf("---->>>%d %d \n", p.x,p.y);
			points[i].x=p.x;
			points[i].y=p.y;
		}
		print_trace(display,g->width/50, g->height/50);
	}

}


int main(){
	grid g;
	struct _point p[]={ {5,5,0},{7,7,0} };
       // print_grid(g=load_grid("data/makiling.txt"));
       	g=load_grid("data/makiling.txt");
	//g=generate_random_grid(20,20);
	print_grid(g);
	d8(g);
	print_flowmap(g);
	trace_flow(p,2,g,6);
	//print_grid(generate_random_grid(10,10));
}


