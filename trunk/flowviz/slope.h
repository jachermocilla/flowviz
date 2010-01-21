#ifndef __SLOPE_H_
#define __SLOPE_H_

typedef struct _slope
{
  float slope;
  float aspect;
  float angle;
}Slope;

Slope Slope_new(float slope, float aspect, float angle);
Layer zevenvergen_slope(Layer elevation);

#endif
