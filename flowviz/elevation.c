#include <stdio.h>
#include <gdal.h>

#include "layer.h"
#include "elevation.h"

Elevation Elevation_new(float value)
{
  Elevation elevation = (Elevation)malloc(sizeof(struct _elevation));
  elevation->value=value;
  return elevation;
}

ElevationMetaData ElevationMetaData_new()
{
  ElevationMetaData meta_data;
  meta_data = (ElevationMetaData)malloc(sizeof(struct _elevation_meta_data));
  return meta_data;
}

Layer Elevation_load(char *fname, int x, int y, int width, int height)
{
    GDALDatasetH  hDataset;
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
    float           tmp,val;

    Elevation cel;
    ElevationMetaData meta_data;
    Layer layer;


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
    
    
    printf("*********");
    meta_data = ElevationMetaData_new();
    meta_data->min_lat=0;
    meta_data->max_lat=0;
    meta_data->min_long=0;
    meta_data->max_long=0;
    meta_data->pixel_size=0;
    meta_data->block_size=0;
    meta_data->min_elev=0;
    meta_data->max_elev=0;

    

    layer = Layer_new("Elevation", meta_data, width, height);

    pafScanline = (float *) CPLMalloc(sizeof(float)*nSize);
    GDALRasterIO( hBand, GF_Read, x, y , width, height,
                    pafScanline, width, height, GDT_Float32,
                      0, 0);

    for (i=0; i<height; i++)
    {
      for (j=0; j<width; j++)
      {
        val = pafScanline[j+i*width];
        if (val == -9999.00)
          val = 0;
        cel = Elevation_new(val);
        layer->data[i+(j*width)]=cel;
      }
    }
    
    return layer;


}
