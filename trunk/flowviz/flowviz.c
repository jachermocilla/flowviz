#include <stdio.h>
#include <stdlib.h>
#include "flowviz.h"
#include "layer.h"
#include "project.h"
#include "elevation.h"
#include "deight.h"
#include "rainfall.h"

int main(int argc, char *argv[])
{
    int x, y, w, l;
    Layer elevation, deight, rainfall, catchment;
    float lon, lat;
    ElevationMetaData meta_data;

    if (argc < 2){
		  printf("Usage: flowviz <DEM> <longitude> <latitude>\n");
		  exit(1);
	  }

    lat = 13.2552f;
    lon = 123.686f;

    if (argv[2] != NULL && argv[3] != NULL){
      lon = atof(argv[2]);
      lat = atof(argv[3]);
    }

    globalProject = Project_new("Flood Simulation");
    

    meta_data = Elevation_open(argv[1]);
    lon_lat_to_x_y(meta_data, &lon, &lat, &x, &y, &w, &l);
    
    elevation = Elevation_load(meta_data,x,y,w,l);
    //Elevation_dump_data(elevation);
    printf("--------------\n");

    deight = DEight_load(elevation);
    //DEight_dump_data(deight);

    printf("--------------\n");
    rainfall=Rainfall_load(w,l,1,1);
    //Rainfall_dump_data(rainfall);

    
    Project_add(globalProject, elevation);
    Project_add(globalProject, deight);
    Project_add(globalProject, rainfall);
    Rainfall_flow(globalProject,1);    
    printf("--------------\n");
    catchment = Project_getLayer(globalProject,"catchment");
    //Catchment_dump_data(catchment);
    
    Elevation_view();
    return 0;
}

