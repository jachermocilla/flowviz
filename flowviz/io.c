#include <stdio.h>
#include <stdlib.h>
#include <gdal.h>
#include "flowviz.h"

Grid load_gdal(char *fname, int x, int y, int width, int height)
{
    GDALDatasetH  hDataset;
    //char *pszFilename = "/media/DATA/srtm_61_10.asc";
    GDALDriverH   hDriver;
    double        adfGeoTransform[6];
    GDALRasterBandH hBand;
    int             nBlockXSize, nBlockYSize;
    int             bGotMin, bGotMax;
    double          adfMinMax[2];
    float           *pafScanline;
    int             nXSize;
    int             nSize = width*height;
    int             i,j;
	  Grid grd;
  	Cell cel;
    float           tmp,val;

    /* Load all registered drivers */
    GDALAllRegister();

    /* Open the data set */
    hDataset = GDALOpen( fname, GA_ReadOnly );
    if( hDataset == NULL )
    {
        printf("Cannot open file.");
    }

    hDriver = GDALGetDatasetDriver( hDataset );

    printf( "Driver: %s/%s\n",
            GDALGetDriverShortName( hDriver ),
            GDALGetDriverLongName( hDriver ) );
    printf( "Size is %dx%dx%d\n",
            GDALGetRasterXSize( hDataset ), 
            GDALGetRasterYSize( hDataset ),
            GDALGetRasterCount( hDataset ) );
    if( GDALGetProjectionRef( hDataset ) != NULL )
        printf( "Projection is `%s'\n", GDALGetProjectionRef( hDataset ) );
    if( GDALGetGeoTransform( hDataset, adfGeoTransform ) == CE_None )
    {
        printf( "Origin = (%.6f,%.6f)\n",
                adfGeoTransform[0], adfGeoTransform[3] );
        printf( "Pixel Size = (%.6f,%.6f)\n",
                adfGeoTransform[1], adfGeoTransform[5] );
    }
    hBand = GDALGetRasterBand( hDataset, 1 );
    GDALGetBlockSize( hBand, &nBlockXSize, &nBlockYSize );
    printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
            nBlockXSize, nBlockYSize,
            GDALGetDataTypeName(GDALGetRasterDataType(hBand)),
            GDALGetColorInterpretationName(
            GDALGetRasterColorInterpretation(hBand)) 
    );
    
    adfMinMax[0] = GDALGetRasterMinimum( hBand, &bGotMin );
    adfMinMax[1] = GDALGetRasterMaximum( hBand, &bGotMax );
    if( ! (bGotMin && bGotMax) )
      GDALComputeRasterMinMax( hBand, TRUE, adfMinMax );
    printf( "Min=%.3f, Max=%.3f\n", adfMinMax[0], adfMinMax[1] );
    if( GDALGetOverviewCount(hBand) > 0 )
      printf( "Band has %d overviews.\n", GDALGetOverviewCount(hBand));
    if( GDALGetRasterColorTable( hBand ) != NULL )
      printf( "Band has a color table with %d entries.\n", 
                 GDALGetColorEntryCount(
                 GDALGetRasterColorTable( hBand ) ) 
      );
    
    
    /*
    nXSize = GDALGetRasterBandXSize( hBand );
    pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);
    GDALRasterIO( hBand, GF_Read, 0, 0, nXSize, 1, 
                    pafScanline, nXSize, 1, GDT_Float32, 
                     0, 0 );
    for (i=0;i<20;i++){
      printf("%f\n", pafScanline[i]);
    } */
    
    pafScanline = (float *) CPLMalloc(sizeof(float)*nSize);
    GDALRasterIO( hBand, GF_Read, x, y , width, height,
                    pafScanline, width, height, GDT_Float32,
                      0, 0);

    //Create the grid
	  grd = (Grid)malloc(sizeof(struct _grid));
	  grd->width=width;
	  grd->length=height;

    for (i=0; i<height; i++)
    {
      for (j=0; j<width; j++)
      {
        cel=(Cell)malloc(sizeof(struct _cell));
        val = pafScanline[j+i*width];
        if (val == -9999.00)
          val = 0;
        tmp =  val*(255.0/(adfMinMax[1]-adfMinMax[0]));
        cel->elevation = (double)ceil(tmp);
        grd->cells[i][j] = cel;
        //printf(" %f ",tmp);
        //printf(" %f ",pafScanline[i+j*width]);
      }
      //printf("\n");
    }
    return grd;
}



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
			/*
      cel->x = j;
			cel->y = i;
      */
			retval=fscanf(fp,"%d",&tmp);
			cel->elevation=(double)tmp;
			/*cel->capacity=1;*/	/*assumed unit capacity */
			grd->cells[i][j] = cel;
		}
	}
	fclose(fp);
	return grd;
}

Grid load_asc(char *fname){
	FILE *fp = fopen(fname,"r");
	Grid grd;
	Cell cel;
	int width,length,tmp,i,j,retval;
  char buff[256];
  float tmp2;


  retval=fscanf(fp,"%s",buff);
  retval=fscanf(fp,"%d",&width);
  retval=fscanf(fp,"%s",buff);
  retval=fscanf(fp,"%d",&length);

  retval=fscanf(fp,"%s",buff);
  retval=fscanf(fp,"%f",&tmp2);
  retval=fscanf(fp,"%s",buff);
  retval=fscanf(fp,"%f",&tmp2);
  retval=fscanf(fp,"%s",buff);
  retval=fscanf(fp,"%f",&tmp2);
  retval=fscanf(fp,"%s",buff);
  retval=fscanf(fp,"%d",&tmp);


	printf("%d %d\n",width,length);
	grd = (Grid)malloc(sizeof(struct _grid));
	grd->width=width;
	grd->length=length;

	for (i=0;i< length; i++){
		for (j=0; j < width;j++){
			cel=(Cell)malloc(sizeof(struct _cell));
      /*
			cel->x = j;
			cel->y = i;
      */
			retval=fscanf(fp,"%d",&tmp);
      if (tmp==-9999)
        tmp=0;
			cel->elevation=(double)tmp;
			/*cel->capacity=1;*/	/*assumed unit capacity */
			grd->cells[i][j] = cel;
		}
	}
	fclose(fp);
	return grd;
}




