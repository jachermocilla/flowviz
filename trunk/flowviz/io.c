#include <stdio.h>
#include <stdlib.h>
#include "flowviz.h"

Grid load_raw(char *fname){
	FILE *fp = fopen(fname,"r");
	Grid grd;
	Cell cel;
	int width,length,tmp,i,j,retval;

	retval=fscanf(fp,"%d %d", &width,&length);
	/*printf("%d %d\n",width,height);*/
	grd = (Grid)malloc(sizeof(struct _grid));
	grd->width=width;
	grd->length=length;

	for (i=0;i< length; i++){
		for (j=0; j < width;j++){
			cel=(Cell)malloc(sizeof(struct _cell));
			cel->x = j;
			cel->y = i;
			retval=fscanf(fp,"%d",&tmp);
			cel->elevation=(double)tmp;
			cel->capacity=1;	/*assumed unit capacity */
			grd->cells[i][j] = cel;
		}
	}
	fclose(fp);
	return grd;
}

