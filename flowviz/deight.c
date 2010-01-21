#include <stdio.h>
#include <stdlib.h>

#include "flowviz.h"
#include "deight.h"
#include "elevation.h"


void DEight_dump_data(Layer deight)
{
  int i, j;
  DEightFlowDir flow_dir;
  printf("D8");
  printf("-----------------------------------------------\n");
  for (i=0;i<deight->length;i++)
  {
    for (j=0;j<deight->width;j++)
    {
      flow_dir = (DEightFlowDir)deight->data[i+(j*deight->width)];
      printf(" %1.0f ", flow_dir->value);
    }
    printf("\n");
  }
  printf("-----------------------------------------------\n");
}


void DEight_get_neighbors(int x, int y, Layer elevation, double neighbors[9])
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

int DEight_get_min_index(double nbors[9])
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
  Layer layer;
  DEightFlowDir flow_dir;

  
	
  layer = Layer_new("deight", NULL, elevation->width, elevation->length);

	for (y=0;y<(elevation->length);y++){
		for (x=0;x<(elevation->width);x++){
			DEight_get_neighbors(x,y,elevation,nbors);
      flow_dir=(DEightFlowDir)malloc(sizeof(struct _deight_flow_dir));
      flow_dir->value = DEight_get_min_index(nbors);
      layer->data[y+(x*elevation->width)]=flow_dir;
		}
	}
  return layer;
}

