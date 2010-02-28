#ifndef __ELEVATION_H_
#define __ELEVATION_H_

#include <gdal.h>
#include <cpl_conv.h>
#include "layer.h"


typedef struct _elevation
{
  float value;
}*Elevation;

typedef struct _elevation_meta_data
{
  float min_lat;
  float max_lat;
  float min_long;
  float max_long;
  float pixel_size;
  float block_size;
  float min_elev;
  float max_elev;
  GDALRasterBandH hBand;
  int x,y,w,l;
}*ElevationMetaData;

Elevation Elevation_new(float value);
ElevationMetaData ElevationMetaData_new();
ElevationMetaData Elevation_open(char * fname);
Layer Elevation_load(ElevationMetaData meta_data, int x, int y, int width, int height);
void Elevation_view();
void Elevation_dump_data(Layer elevation);

#endif
