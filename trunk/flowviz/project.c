#include <stdio.h>
#include "layer.h"
#include "project.h"


Project Project_new(char name)
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

