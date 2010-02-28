#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "layer.h"
#include "project.h"


Project Project_new(char *name)
{
  Project project;
  project = (Project)malloc(sizeof(struct _project));
  strcpy(project->name, name);
  project->layers=NULL;
  return project;
}

void Project_delete(Project *project)
{
  free(*project);
}

void Project_add(Project project, Layer layer)
{
  layer->next = project->layers;
  project->layers = layer;
}


Layer Project_getLayer(Project project, char *name)
{
  Layer tmp;
  tmp = project->layers;
  while (tmp != NULL && strcmp(tmp->name,name)!=0)
  { 
    tmp=tmp->next;
  }
  return tmp;
}

