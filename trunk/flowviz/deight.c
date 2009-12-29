#include <stdio.h>
#include <stdlib.h>

#include "flowviz.h"
#include "deight.h"
#include "elevation.h"


void get_neighbors(int x, int y, Layer elevation, double neighbors[9])
{
  int i;
  Elevation elev;
	
	for (i=0;i<9;i++)
		neighbors[i]=9999999;

	if (y >= 1 && y<(elevation->length-1)){
		if (x>=1 && x<(elevation->width-1)){
      elev = (Elevation)elevation->data[(y-1)+((x-1)*elevation->width)];
			neighbors[I_NW]=elev->value;

      elev = (Elevation)elevation->data[(y-1)+(x*elevation->width)];
			neighbors[I_N]=elev->value;
			
      elev = (Elevation)elevation->data[(y-1)+((x+1)*elevation->width)];
      neighbors[I_NE]=elev->value;
			
      elev = (Elevation)elevation->data[y+((x-1)*elevation->width)];
			neighbors[I_W]=elev->value;
			
      elev = (Elevation)elevation->data[y+(x*elevation->width)];
      neighbors[I_CENTER]=elev->value;
			
      elev = (Elevation)elevation->data[y+((x+1)*elevation->width)];
      neighbors[I_E]=elev->value;
			
      elev = (Elevation)elevation->data[(y+1)+((x-1)*elevation->width)];
			neighbors[I_SW]=elev->value;
			
      elev = (Elevation)elevation->data[(y+1)+(x*elevation->width)];
      neighbors[I_S]=elev->value;
			
      elev = (Elevation)elevation->data[(y+1)+((x+1)*elevation->width)];
      neighbors[I_SE]=elev->value;
		}	
	}
	/*printf("Here %d %d\n",x,y);	*/

}

int get_min_index(double nbors[9])
{
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

Layer DEight_load(Layer elevation)
{
  double nbors[9];
	int x,y;
	FlowMap flow_map;
	
	flow_map=(FlowMap)malloc(sizeof(struct _flow_map));

	for (y=0;y<(layer->length);y++){
		for (x=0;x<(layer->width);x++){
			get_neighbors(x,y,g,nbors);
			flow_map->direction[y][x]=get_min_index(nbors);
		}
	}
	g->flow_map=flow_map;

}

