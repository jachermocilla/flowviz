#include <stdio.h>
#include <stdlib.h>
#include "flowviz.h"

int main(int argc,char *argv[])
{
  float min_lat=10,
        max_lat=15,
        min_long=120,
        max_long=125,
        pixel_size=0.000833f,
        block_size=6001.0;

  float latitude=13.2552f, 
        longitude=123.686;

  int xoffset, yoffset, xdim, ydim, x, y, maxx=6001, maxy=6001;
 

	if (argc < 2){
		printf("Usage: flowviz <DEM> <longitude> <latitude>\n");
		exit(1);
	}

  if (argv[2] != NULL && argv[3] != NULL){
    longitude = atof(argv[2]);
    latitude = atof(argv[3]);
  }

  x = (int)((longitude - min_long)/pixel_size);
  y = (int)(block_size-((latitude - min_lat)/pixel_size));

  xdim = minimum(x,(maxx-x));
  ydim = minimum(y,(maxy-y));
 
  xoffset = x-xdim;
  yoffset = y-ydim;

  printf("x=%d, y=%d\n",x,y);
  printf("maxx=%d, maxy=%d\n",maxx,maxy);  
  printf("xdim=%d, ydim=%d\n",xdim,ydim);

  //comment out the following to maximize the view
  /*int clip_size=256;
  xdim=ydim=clip_size;
  xoffset = x-clip_size;
  yoffset = y-clip_size;
  */


  //xoffset = (int)((longitude - min_long)/pixel_size)-(dimension*0.5f);
  //yoffset = (int)(block_size-((latitude - min_lat)/pixel_size))-(dimension*0.5f);
  //if (yoffset < 0){
  //  yoffset=0;
  //}  


  //grid=load_gdal(argv[1],1400,1000,512,512);
  //grid=load_gdal(argv[1],3400,1069,2048,2048);
  grid=load_gdal(argv[1],xoffset,yoffset,xdim*2,ydim*2);
	//grid=load_raw(argv[1]);
	/*grid=generate_random_grid(10,10);*/
	/*print_grid(grid);*/
	d8(grid);
	/*print_flow_map(grid);*/
	view3dGrid(grid);
	return 0;
}
