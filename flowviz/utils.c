#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "flowviz.h"
#include "elevation.h"

void x_y_to_longi_lati(DataSetParam tparam, int x, int y, float *longi, float *lati);
void lon_lat_to_x_y(ElevationMetaData tparam,float *lon, float *lat, int *x, int *y, int *w, int *l);

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
//    getchar();
	}
}


//FIXME
void find_catchment(Grid grid)
{
  int i,j,k=0;
  float longi, lati;
  Cell c;
  FlowMap flow_map=grid->flow_map;
  for (i=0;i<grid->length;i++)
  {
    for (j=0;j<grid->width;j++){
      if (flow_map->direction[i][j]==I_CENTER)
      {
        c=grid->cells[i][j];
        if (c->elevation > 3)
        {
          x_y_to_longi_lati(&data_set_param, j, i, &longi, &lati);
          printf("%f,%f\n",longi,lati);
          k++;
        }
      }
    }
  }
  printf("%d\n",k);
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

int minimum(int a, int b)
{
  if (a < b)
  {
    return a;
  }
  else
  {
    return b;
  }
}

void lon_lat_to_x_y(ElevationMetaData tparam, float *lon, float *lat, int *x, int *y, int *w, int *l)
{
  int xdim, ydim,dim;

  *x = (int)((*lon - tparam->min_long)/tparam->pixel_size);
  *y = (int)(tparam->block_size-((*lat - tparam->min_lat)/tparam->pixel_size));

  xdim = minimum(*x,((int)(tparam->block_size)-(*x)));
  ydim = minimum(*y,((int)(tparam->block_size)-(*y)));

  dim = minimum(xdim,ydim); 

//  printf("%d,%d,%d\n",*y,((int)(tparam->block_size)-(*y)),ydim);
  
  dim=512; 


  *x = *x-dim;
  *y = *y-dim;

//  printf("%d, %d\n",*x,*y);

  *w = dim*2;
  *l = dim*2;


}

void x_y_to_longi_lati(DataSetParam tparam, int x, int y, float *longi, float *lati)
{
  *longi = (x*tparam->pixel_size)+tparam->min_long;
  *lati = ((tparam->block_size - y)*tparam->pixel_size) + tparam->min_lat;
}
