#ifndef __LAYER_H_
#define __LAYER_H_

/**
 * Layer abstration
 * @author Joseph Anthony C. Hermocilla
*/

typedef struct _layer
{
  char name[255];       /* Name of the layer */
  int width, length;    /* Dimensions of this layer */
  void *meta_data;      /* Description information for this layer */
  void **data;          /* Generic pointer to data*/
  struct _layer *next;  /* Pointer to the next layer in multilayer projects*/
}*Layer;


Layer gElevation;

Layer Layer_new(char *name, void *meta_data ,int width, int length);
void Layer_delete(Layer *layer);


#endif
