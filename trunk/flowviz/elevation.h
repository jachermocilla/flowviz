#ifndef __ELEVATION_H_
#define __ELEVATION_H_

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
}*ElevationMetaData;

Elevation Elevation_new(float value);
ElevationMetaData ElevationMetaData_new();
Layer Elevation_load(char *fname, int x, int y, int width, int height);
void Elevation_view();

#endif
