

Slope Slope_new(float slope, float aspect, float angle)
{
  Slope slope;

  slope = (Slope)malloc(sizeof(struct _slope));
  slope->slope = slope;
  slope->aspect = aspect;
  slope->angle = angle; 

  return slope;
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
