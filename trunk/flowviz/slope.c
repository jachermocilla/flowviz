
#include "elevation.h"
#include "slope.h"

void Slope_dump_data(Layer slope)
{
  int i, j;
  Slope slp;
  printf("-------Slope----------------------------\n");
  for (i=0;i<slope->length;i++)
  {
    for (j=0;j<slope->width;j++)
    {
      slp = (Slope)slope->data[i+(j*slope->width)];
      printf(" (%2.2f,%2.2f,%2.2f) ",slp->slope, slp->angle, slp->aspect);
    }
    printf("\n");
  }
  printf("---------------------------------");
}


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
  float slp, angle, aspect;

  retval = Layer_new("zevenvergen", NULL, elevation->width, elevation->length);

  //boundary
  for (y=0;y<(elevation->width);y++)
  {
      slope = Slope_new(0, 0, 0);      
      retval->data[y+(0*elevation->width)]=slope;
      retval->data[y+((elevation->width-1)*elevation->width)]=slope;  
  }
  
  for (x=0;x<(elevation->length);x++)
  {
      slope = Slope_new(0, 0, 0);      
      retval->data[0+(x*elevation->width)]=slope;
      retval->data[(elevation->length-1)+(x*elevation->width)]=slope;  
  }



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

      slp = -sqrt((east_west*east_west)+(north_south*north_south));
      angle = fabs(1.0/tan(slope->slope));
      aspect = atan(north_south/east_west);

      slope = Slope_new(slp, angle, aspect);

      retval->data[y+(x*elevation->width)]=slope;
    }
  }
  return retval;
}
