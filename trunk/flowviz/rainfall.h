#ifndef __RAINFALL_H_
#define __RAINFALL_H_

#include "layer.h"
#include "project.h"
#include "flowviz.h"

typedef struct _rainfall
{
  Point p;
  float probability;
  float amount;
}*Rainfall;

typedef struct _water_level
{
  Point p;
  float level;
}*WaterLevel;


Rainfall Rainfall_new(float probability, float amount, Point p);
WaterLevel WaterLevel_new(float level, Point p);
Layer Rainfall_load(int w, int l,float probability, float amount);
void Rainfall_flow(Project p, int maxsteps);
void Rainfall_dump_data(Layer rainfall);
void Catchment_dump_data(Layer catchment);


#endif
