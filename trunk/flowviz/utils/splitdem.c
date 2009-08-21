#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1000
#define MAX_FILES 10

int main(int argc, char **argv)
{
  FILE *source,*targets[MAX_FILES];
  int ncols,nrows,nfiles;
  char buff[255],buff2[255];
  int i,j,k,tmp,data;
  float tmp2;

  if (argc < 2)
  {
    printf("Usage: splitdem <dem>");
    exit(1);
  }

  source=fopen(argv[1],"r");
  if (source == NULL)
  {
    printf("Cannot open %s\n.",argv[1]);
    exit(1);
  }

  fscanf(source,"%s",buff);
  fscanf(source,"%d",&ncols);
  fscanf(source,"%s",buff);
  fscanf(source,"%d",&nrows);

  fscanf(source,"%s",buff);
  fscanf(source,"%f",&tmp2);
  fscanf(source,"%s",buff);
  fscanf(source,"%f",&tmp2);
  fscanf(source,"%s",buff);
  fscanf(source,"%f",&tmp2);
  fscanf(source,"%s",buff);
  fscanf(source,"%d",&tmp);

  /*printf("%d\n",tmp); */


  nfiles=(nrows/SIZE)*(nrows/SIZE);  

  printf("%d %d %d\n",nrows,ncols,nfiles);

  for (i=0;i<nfiles;i++)
  {
    sprintf(buff, "%s.%d", argv[1], (i+1));
    targets[i]=fopen(buff,"w");
    fprintf(targets[i],"ncols          %d\n",SIZE);
    fprintf(targets[i],"nrows          %d\n",SIZE);
    fprintf(targets[i],"xllcorner      %d\n",0);
    fprintf(targets[i],"yllcorner      %d\n",0);
    fprintf(targets[i],"cellsize       %d\n",0);
    fprintf(targets[i],"NODATA_value   %d\n",-9999);

    for (j=0;j<SIZE;j++)
    {
      fscanf(source,"%d",&data);
      fprintf(targets[i],"%d ",data); 
    }
    fclose(targets[i]);
  }

  


}
