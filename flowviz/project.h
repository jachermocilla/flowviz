#ifndef __PROJECT_H_
#define __PROJECT_H_

typedef struct _project
{
  char name[255];
  Layer layers;
}*Project;


Project Project_new(char *name);
void Project_delete(Project *project);

void Project_add(Project project, Layer layer);
Layer Project_getLayer(Project project, char *name);


#endif
