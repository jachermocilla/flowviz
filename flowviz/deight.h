#ifndef __DEIGHT_H_
#define __DEIGHT_H_

#include "layer.h"

/* Constants for directions */

#define CENTER	0
#define E	1
#define SE	2
#define S	4
#define SW	8
#define	W	16
#define	NW	32
#define N	64
#define NE	128

/* Index constants */
#define I_NW 	0
#define I_N 	1
#define I_NE	2
#define I_W	3
#define I_CENTER	4
#define I_E	5
#define I_SW	6
#define I_S	7
#define I_SE	8

typedef struct _deight_flow_dir
{
  float value;
}*DEightFlowDir;

typedef struct _deight_meta_data
{

}*DEightMetaData;


void get_neighbors(int x, int y, Layer layer, double neighbors[9]);
int get_min_index(double nbors[9]);
Layer DEight_load(Layer elevation);
#endif
