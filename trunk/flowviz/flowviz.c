#include <stdio.h>
#include <stdlib.h>
#include "flowviz.h"

int main(int argc,char *argv[])
{
	if (argc < 2){
		printf("Usage: flowviz <DEM>\n");
		exit(1);
	}
  //grid=load_gdal(argv[1],1400,1000,512,512);
  grid=load_gdal(argv[1],1024,512,2048,2048);
	//grid=load_raw(argv[1]);
	/*grid=generate_random_grid(10,10);*/
	/*print_grid(grid);*/
	d8(grid);
	/*print_flow_map(grid);*/
	view3dGrid(grid);
	return 0;
}
