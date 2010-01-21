
#include "elevation.h"
#include "slope.h"


Slope Slope_new(float slope, float aspect, float angle)
{
  Slope retval;

  retval = (Slope)malloc(sizeof(struct _slope));
  retval->slope = slope;
  retval->aspect = aspect;
  retval->angle = angle; 

  return retval;
}




Layer zevenvergen_slope(Layer elevation)
{
  Layer retval;
  Slope slope;
  int x, y;
  Elevation elev;
  float east_west, north_south;
  Elevation north, south, east, west;
  


  retval = Layer_new("zevenvergen", NULL, elevation->width, elevation->length);
  
  slope = Slope_new(0, 0, 0);
  for (y=1;y<(elevation->length)-1;y++)
  {
		for (x=1;x<(elevation->width)-1;x++)
    {
      east = (Elevation)elevation->data[y+((x+1)*elevation->width)];
      west = (Elevation)elevation->data[y+((x-1)*elevation->width)];
      north = (Elevation)elevation->data[(y-1)+(x*elevation->width)];
      south = (Elevation)elevation->data[(y+1)+(x*elevation->width)];
      east_west = (-1*(east->value)+(west->value))/(2*90);
      north_south = ((north->value)-(south->value))/(2*90);
      slope->slope = -sqrt((east_west*east_west)+(north_south*north_south));
      slope->angle = 1.0/tan(slope->slope);
      slope->aspect = arctan(north_south/east_west);
    }
  }
  
  

  return retval;
}
