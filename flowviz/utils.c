#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "flowviz.h"

Grid generate_random_grid(int width, int length){
	int i,j;
	Cell cel;
	Grid grd;
	unsigned int iseed;

	iseed = (unsigned int)time(NULL);
        srand (iseed);

	grd = (Grid)malloc(sizeof(struct _grid));
	grd->width=width;
	grd->length=length;

	for (i=0;i< length; i++){
		for (j=0; j < width;j++){
			cel=(Cell)malloc(sizeof(struct _cell));
      /*
			cel->x = j;
			cel->y = i;
      */
			cel->elevation = rand()%MAX_ELEVATION;
			/*
      cel->capacity = rand()%MAX_CAPACITY;
			cel->catchment = (int)(rand()%2);
      */
			grd->cells[i][j] = cel;
		}
	}
	return grd;
}

void print_cell(Cell cel){
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


void print_grid(Grid grd){
	int i,j;
	Cell cel;	

	for (i=0;i<grd->length;i++){
		for (j=0;j<grd->width;j++){
			cel = grd->cells[i][j];
			print_cell(cel);			
		}
		printf("\n");
	}
}


void print_flow_map(Grid g){
	int x,y;
	FlowMap flow_map=g->flow_map;
	for (y=0;y<(g->length);y++){
		for (x=0;x<(g->width);x++){
			printf(" %d ",flow_map->direction[y][x]);
		}
		printf("\n");
	}
}


Point find_peak(Grid grid){
  int w,l,h;
  Point p=(Point)malloc(sizeof(struct _point));
  p->y=-9999;
  for (l=0;l<grid->length;l++){
    for (w=0;w<grid->width;w++){
        h=grid->cells[l][w]->elevation;
        if (h>p->y){
          p->x=w;
          p->z=l;
          p->y=h;          
        }
    }
  }
  return p;
}