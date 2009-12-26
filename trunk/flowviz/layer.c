#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "layer.h"

Layer Layer_new(char *name, void *meta_data, int width, int length)
{
  Layer layer;
  layer = (Layer)malloc(sizeof(struct _layer));
  strcpy(layer->name,name);
  layer->meta_data=meta_data;
  layer->width=width;
  layer->length=length;
  layer->data=(void *)malloc(width*length*sizeof(void *));
  return layer;
}

void Layer_delete(Layer *layer)
{
  free(*layer);
}
