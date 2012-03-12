#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rainfall.h"
#include "deight.h"
#include "list.h"


int compare(WaterLevel level1, WaterLevel level2)
{
  //printf("Comparing %f, %f\n",level1->level,level2->level);
  if (level1->level == level2->level)
  {
    return 0;
  }else if (level1->level > level2->level)
  {
    return 1;
  }else
  {
    return -1;
  }    
}


List Rainfall_get_catchments(Layer catchment)
{
  List list;
  int i, j;
  WaterLevel water_level;

  list=List_new();

  for (i=1;i<catchment->length-1;i++)
  {
    for (j=1;j<catchment->length-1;j++)
    {
      water_level = (WaterLevel)catchment->data[i+(j*catchment->width)];
      if (water_level->level > 1)
      {
        List_insert_sorted(list,water_level,compare);
      }
    }
  }
  return list;
}

void Catchment_dump_data(Layer catchment)
{
  int i, j;
  WaterLevel water_level;
  printf("Catchment---------------------------------------\n");
  for (i=0;i<catchment->length;i++)
  {
    for (j=0;j<catchment->width;j++)
    {
      water_level = (WaterLevel)catchment->data[i+(j*catchment->width)];
      printf(" %3.0f ", water_level->level);
    }
    printf("\n");
  }
  printf("-----------------------------------------------\n");
}



void Rainfall_dump_data(Layer rainfall)
{
  int i, j;
  Rainfall rain;
  printf("Rainfall---------------------------------------\n");
  for (i=0;i<rainfall->length;i++)
  {
    for (j=0;j<rainfall->width;j++)
    {
      rain = (Rainfall)rainfall->data[i+(j*rainfall->width)];
      printf(" %1.0f ", rain->amount);
    }
    printf("\n");
  }
  printf("-----------------------------------------------\n");
}


void get_next(int dir, Point p)
{

	switch (dir){
		case I_NW: p->x-=1;p->y-=1;break;
		case I_N: p->y-=1; break;
		case I_NE: p->x+=1; p->y-=1;break;
		case I_W: p->x-=1; break;
		case I_CENTER: break;
		case I_E: p->x+=1; break;
		case I_SW: p->x-=1;p->y+=1; break;
		case I_S: p->y+=1; break;
		case I_SE: p->x+=1;p->y+=1; break;
	}
  
  if(p->x < 0)
    p->x=0;
  if(p->y < 0)
    p->y=0;
}

void Rainfall_flow(Project p,int maxsteps)
{
  Layer catchment, elevation, deight, rainfall, traceflow;
  int i,j,k,t=0;  
  WaterLevel water_level;
  Point point;
  Rainfall rain;
  DEightFlowDir flow_dir;
  float tmp;
  int area;
  int *shuffle;
  int mytmp, r;
  
  srand(time(NULL));

  elevation=Project_getLayer(p,"elevation");
  deight=Project_getLayer(p,"deight");
  rainfall=Project_getLayer(p,"rainfall");

  catchment = Layer_new("catchment",NULL,elevation->width, elevation->length);
  traceflow = Layer_new("traceflow",NULL,elevation->width, elevation->length);


  for (i=0;i<rainfall->length;i++)
  {
    for (j=0;j<rainfall->width;j++)
    {
      rain = (Rainfall)rainfall->data[i+(j*elevation->width)];
      water_level = WaterLevel_new(rain->amount,rain->p);
      catchment->data[i+(j*elevation->width)]=water_level;		
      traceflow->data[j+(i*elevation->width)]=water_level;
    }
  }

  /**
   *FIXME: This code performs a column-row order traversal
   * Might as well do a random choice of data points.
   *
  */

  area = (rainfall->length * rainfall->width);
  shuffle = (int *)malloc(area*sizeof(int));
  
  for (i=0;i<area;i++)
  {
    shuffle[i]=i;
  }

  for (i=0;i<area;i++)
  {
    r = i + (rand() % (area-i));
    mytmp = shuffle[i];
    shuffle[i] = shuffle[r];
    shuffle[r]=mytmp;  
  }

  for (i=2;i<rainfall->length-2;i++){
    for (j=2;j<rainfall->width-2;j++){			
	      k=i+(j*rainfall->width);
	      rain=(Rainfall)rainfall->data[k];
		   point=rain->p;
		 for (t=0;t<maxsteps;t++){
		     water_level = (WaterLevel)catchment->data[k];
		     tmp = water_level->level;
		     water_level->level=0;
	//		  printf(" water source: (%d,%d)", point->x, point->y);
		     flow_dir = (DEightFlowDir)deight->data[point->y+(point->x*deight->width)];
		     get_next(flow_dir->value,point);
	//		  printf(" nxt point: (%d,%d) ", point->x, point->y);
		     water_level = (WaterLevel)catchment->data[point->y+(point->x*deight->width)];
		     water_level->level+=tmp; //update catchment of next point
	 		  if(tmp>0){
				traceflow->data[k] = water_level;
				traceflow->data[point->y+(point->x*deight->width)] = water_level;
			  }
		 }
        //printf("(%d, %d, %f, %f)\n", point->x,point->y,flow_dir->value,water_level->level);
		 printf("\n");
    }
  }
  Project_add(p,traceflow);
  Project_add(p,catchment);
}

WaterLevel WaterLevel_new(float level, Point p)
{
  WaterLevel water_level=(WaterLevel)malloc(sizeof(struct _water_level));
  water_level->level=level;
  water_level->p=p;
  return water_level;
}

Rainfall Rainfall_new(float probability, float amount, Point p)
{
  Rainfall rain;
  rain = (Rainfall)malloc(sizeof(struct _rainfall));
  rain->probability=probability;
  rain->amount=amount;
  rain->p=p;
  return rain;
}

Layer Rainfall_load(int w, int l,int probability, float amount)
{
  Layer layer;
  Rainfall rain;
  int area;
  int i,j;
  int r;

  srand(time(NULL));

  area=w*l;
  layer = Layer_new("rainfall", NULL, w, l);
  for (i=0;i<l;i++)
  {
    for (j=0;j<w;j++)
    {
      Point p=(Point)malloc(sizeof(struct _point));
      p->x=j;
      p->y=i;
      r = rand() % 100;
      if (r >= (100-probability))
      {
        rain = Rainfall_new(1,1,p);
      }else
      {
        rain = Rainfall_new(1,0,p);
      }
      layer->data[i+(j*w)]=rain;
    }
  }
  return layer;
}
