#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "flowviz.h"
#include "layer.h"
#include "project.h"
#include "elevation.h"
#include "deight.h"
#include "rainfall.h"
#include "list.h"

/*
void test_list()
{
  List list;
  list = List_new();
  Point p=(Point)malloc(sizeof(struct _point));
  WaterLevel l1,l2,l3,l4,l;
  ListIterator iter;

  l1=WaterLevel_new(4,p);
  l2=WaterLevel_new(3,p);
  l3=WaterLevel_new(2,p);
  l4=WaterLevel_new(1,p);

  List_insert_sorted(list,l1,compare);
  List_insert_sorted(list,l2,compare);
  List_insert_sorted(list,l3,compare);
  List_insert_sorted(list,l4,compare);

  for (iter = List_begin(list); iter != List_last(list); iter = List_next(iter))
  {
    l = (WaterLevel)List_elementAt(list,iter);
		printf("%2.0f\n",l->level);
  }
}
*/

int main(int argc, char *argv[])
{
    int x, y, w, l;
    Layer elevation, deight, rainfall, catchment, slope;
    float lon, lat;
    ElevationMetaData meta_data;

    if (argc < 2){
		  printf("Usage: flowviz <DEM> <latitude> <longitude> \n");
		  exit(1);
	  }

    lat = 13.2552f;
    lon = 123.686f;

    if (argv[2] != NULL && argv[3] != NULL){
      lat = atof(argv[2]);
      lon = atof(argv[3]);
    }

    globalProject = Project_new("Flood Simulation");
    

    meta_data = Elevation_open(argv[1]);
    lon_lat_to_x_y(meta_data, &lon, &lat, &x, &y, &w, &l);
    
    elevation = Elevation_load(meta_data,x,y,w,l);
    Elevation_dump_data(elevation);

    slope = zevenvergen_slope(elevation);
    Slope_dump_data(slope);

    deight = DEight_load(elevation);
    DEight_dump_data(deight);

    rainfall=Rainfall_load(w,l,1,1);
    Rainfall_dump_data(rainfall);
    
    Project_add(globalProject, elevation);
    Project_add(globalProject, deight);
    Project_add(globalProject, rainfall);

    Rainfall_flow(globalProject,10);    

    catchment = Project_getLayer(globalProject,"catchment");
    Catchment_dump_data(catchment);

    export_google_map(catchment, meta_data);
/*
    list=Rainfall_get_catchments(catchment);
    k=0;
    for (iter = List_begin(list); iter != List_last(list); iter = List_next(iter))
    {
      water_level = (WaterLevel)List_elementAt(list,iter);
      x_y_to_longi_lati(meta_data, meta_data->x+water_level->p->x, meta_data->y+water_level->p->y,&longi, &lati );
		  printf("%f, %f, %2.0f\n",lati, longi, water_level->level);
      k++;
      if (k > 10) break;
    }
*/    
    
    Elevation_view();
    return 0;

}

