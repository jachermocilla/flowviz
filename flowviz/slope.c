
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


  retval = Layer_new("zevenvergen", NULL, elevation->width, elevation->length);
  
  for (y=1;y<(elevation->length)-1;y++)
  {
		for (x=1;x<(elevation->width)-1;x++)
    {
      
    }
  }
  
  

  return retval;
}
