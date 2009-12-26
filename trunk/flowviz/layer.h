#ifndef __LAYER_H_
#define __LAYER_H_

typedef struct _layer
{
  char name[255];
  int width, length;
  void *meta_data;
  void **data;
  struct _layer *next;
}*Layer;

Layer Layer_new(char *name, void *meta_data ,int width, int length);
void Layer_delete(Layer *layer);


#endif
