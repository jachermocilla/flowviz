#include <stdio.h>
#include <stdlib.h>

#include "flowviz.h"

void get_neighbors(int x, int y, Grid g, double neighbors[9]){
	int i;
	
	for (i=0;i<9;i++)
		neighbors[i]=9999999;

	if (y >= 1 && y<(g->length-1)){
		if (x>=1 && x<(g->width-1)){
			neighbors[I_NW]=g->cells[y-1][x-1]->elevation;
			neighbors[I_N]=g->cells[y-1][x]->elevation;
			neighbors[I_NE]=g->cells[y-1][x+1]->elevation;
			
			neighbors[I_W]=g->cells[y][x-1]->elevation;
			neighbors[I_CENTER]=g->cells[y][x]->elevation;
			neighbors[I_E]=g->cells[y][x+1]->elevation;
			
			neighbors[I_SW]=g->cells[y+1][x-1]->elevation;
			neighbors[I_S]=g->cells[y+1][x]->elevation;
			neighbors[I_SE]=g->cells[y+1][x+1]->elevation;
		}	
	}
	/*printf("Here %d %d\n",x,y);	*/
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

void d8(Grid g){
	double nbors[9];
	int x,y;
	FlowMap flow_map;
	
	flow_map=(FlowMap)malloc(sizeof(struct _flow_map));

	for (y=0;y<(g->length);y++){
		for (x=0;x<(g->width);x++){
			get_neighbors(x,y,g,nbors);
			flow_map->direction[y][x]=get_min_index(nbors);
		}
	}
	g->flow_map=flow_map;
}

