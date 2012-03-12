/*
 * Nehe Lesson 34 Code (ported to Linux//GLX by Patrick Schubert 2003
 * with help from the lesson 1 basecode for Linux/GLX by Mihael Vrbanec)
 */

#include "nehe.h"
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>

#include "flowviz.h"
#include "layer.h"
#include "elevation.h"
#include "deight.h"
#include "project.h"
#include "rainfall.h"

#define		STEP_SIZE	  2							/* Width And Height Of Each Quad */
#define		HEIGHT_RATIO  0.35f				/* Ratio That The Y Is Scaled According To The X And Z */
#define		MAXSTEPS	  10		

BOOL		bRender = True;							/* Polygon Flag Set To TRUE By Default */


float scaleValue = 0.3f;						/* Scale Value For The Terrain 0.3*/

float rot=0.0;
float rotx=0.0;
float rotxx=0.0;
float roty=0.0;
float rotyy=0.0;
float rotz=0.0;
float rotzz=0.0;

GLdouble eyex=212,eyey=60,eyez=194;
GLdouble centerx=186, centery=55, centerz=171;
GLdouble upx=0,upy=1,upz=0;

void gluLookAt(GLdouble eyex,GLdouble eyey,GLdouble eyez,
			   GLdouble centerx, GLdouble centery, GLdouble centerz,
			   GLdouble upx,GLdouble upy, GLdouble upz)
{
	GLdouble m[16];
	GLdouble x[3], y[3],z[3];
	GLdouble mag;
	z[0] = eyex - centerx;
	z[1] = eyey - centery;
	z[2] = eyez - centerz;

	if((mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2])))
		z[0] /= mag,z[1] /= mag,z[2] /= mag;

	y[0] = upx;
	y[1] = upy;
	y[2] = upz;
	x[0] = y[1] * z[2] - y[2] * z[1];
	x[1] = -y[0] *z[2] + y[2] * z[0];
	x[2] = y[0] * z[1] - y[1] * z[0];

	y[0] = z[1] * x[2] - z[2] * x[1];
	y[1] = -z[0] * x[2] + z[2] * x[0];
	y[2] = z[0] * x[1] - z[1] * x[0];

	if((mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2])))
		x[0] /= mag,x[1] /= mag,x[2] /= mag;

	if((mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2])))
		y[0] /= mag,y[1] /= mag,y[2] /= mag;

	m[0] = x[0];m[4] = x[1];m[8] = x[2];m[12] = 0.0;
	m[1] = y[0];m[5] = y[1];m[9] = y[2];m[13] = 0.0;
	m[2] = z[0];m[6] = z[1];m[10] = z[2];m[14] = 0.0;
	m[3] = m[7] = m[11] = 0.0;
	m[15] = 1.0;

	glMultMatrixd(m);
	glTranslated(-eyex,-eyey, -eyez);
}

int NormalizeElevation(float elevation)
{
  Layer gElevation=Project_getLayer(globalProject,"elevation");
  ElevationMetaData metaData=(ElevationMetaData)gElevation->meta_data;;
  return (int)(elevation*(256.0/(metaData->max_elev-metaData->min_elev)));
}


void GetPointFromDir(int dir, Point p)
{
printf("dir: %d ",dir);
	switch (dir){
		case I_NW: p->x-=1;p->y-=1;break;
		case I_N: p->y-=1; break;
		case I_NE: p->x+=1; p->y-=1;break;
		case I_W: p->x-=1; break;
		case I_CENTER: break;
		case I_E: p->x+=1; break;
		case I_SW: p->x-=1;p->y+=1; break;
		case I_S: p->y+=1; break;
		case I_SE: p->x+=1;p->y+=1; break;
	}

  if(p->x < 0)
    p->x=0;
  if(p->y < 0)
    p->y=0;

}

int Height(Layer elevation, int X, int Y)	
{
  Elevation elev;
	int x = X % (int)(elevation->width);						
	int y = Y % (int)(elevation->length);

	if(!elevation) return 0;						
  elev = elevation->data[y+(x*elevation->width)];
	return NormalizeElevation(elev->value);
}

void SetVertexColor(Layer elevation, int x, int y)	/* Sets The Color Value For A Particular Index, Depending On The Height Index */
{
	if(!elevation)
		return;								/* Make Sure Our Height Data Is Valid */
	else
	{
		float fColor = (-0.15f + ((Height(elevation, x, y )+127) / 256.0f));
		/* Assign This Blue Shade To The Current Vertex */
    if (Height(elevation, x, y )<2){
      //fColor = (-0.15f + ((Height(grid, x, y )+127) / 256.0f));
		  glColor3f(0,0 ,fColor );
    }
    else if (Height(elevation, x, y )>=2 && Height(elevation, x, y )<5)
    {
		  glColor3f(fColor, fColor, fColor  );
    }
    else
    {
		  glColor3f(0, 1.0-fColor ,0 );
    }

	}
}

void RenderFlowMap(){
  int i,j,t;
  Point pt;
  Rainfall water_level;
  Layer deight = Project_getLayer(globalProject,"deight");
  Layer rainfall = Project_getLayer(globalProject,"rainfall");
  Layer elevation = Project_getLayer(globalProject,"elevation");
  DEightFlowDir flow_dir;
  glBegin(GL_LINES);
  for (i=0;i<elevation->length;i++)
  {
    for (j=0;j<elevation->width;j++)
    {
      water_level=(Rainfall)rainfall->data[i+(j*elevation->width)];
      if(water_level->amount  > 9 ){
		 pt=water_level->p;
	    for (t=0;t<MAXSTEPS;t++){
        glColor3f(1.0f,1.0f,0.0f);
	     glVertex3i(pt->x,Height(elevation,pt->x,pt->y),pt->y);
        flow_dir = (DEightFlowDir)deight->data[pt->y+(pt->x*deight->width)];
        GetPointFromDir(flow_dir->value,pt);
   	 }
		}
	}
	printf("\n");
  }
  glEnd();
}

void RenderCatchment()
{
  int i,j;
  WaterLevel water_level;
  Layer catchment = Project_getLayer(globalProject,"catchment");
  Layer elevation = Project_getLayer(globalProject,"elevation");
  glBegin(GL_POINTS);
  for (i=0;i<catchment->length;i++)
  {
    for (j=0;j<catchment->width;j++)
    {
      water_level=(WaterLevel)catchment->data[i+(j*catchment->width)];
      if (water_level->level  > 10 )
      {
     //   printf(" (%d,%d) ", j,i);
        glColor3f(1,1 ,0 );
        glVertex3i(j, Height(elevation,j,i), i);      
      }
    }
	//printf("\n");
  }
  glEnd();
}

void RenderHeightMap(Layer elevation)		/* This Renders The Height Map As Quads */
{
	int X = 0, Y = 0;									/* Create Some Variables To Walk The Array With. */
	int x, y, z;										/* Create Some Variables For Readability */
  Point p;


	if(!elevation) return;							/* Make Sure Our Height Data Is Valid */

	if(bRender)											/* What We Want To Render */
		glBegin( GL_QUADS );							/* Render Polygons */

	else
		glBegin( GL_LINES );							/* Render Lines Instead */

	for ( X = 0; X < elevation->width; X += STEP_SIZE )
		for ( Y = 0; Y < elevation->length; Y += STEP_SIZE )
		{
			/* Get The (X, Y, Z) Value For The Bottom Left Vertex */
			x = X;							
			y = Height(elevation, X, Y );
			z = Y;

			/* Set The Color Value Of The Current Vertex */
			SetVertexColor(elevation, x, z);

			glVertex3i(x, y, z);						/* Send This Vertex To OpenGL To Be Rendered (Integer Points Are Faster) */

			/* Get The (X, Y, Z) Value For The Top Left Vertex */
			x = X;										
			y = Height(elevation, X, Y + STEP_SIZE );
			z = Y + STEP_SIZE ;

			/* Set The Color Value Of The Current Vertex */
			SetVertexColor(elevation, x, z);

			glVertex3i(x, y, z);						/* Send This Vertex To OpenGL To Be Rendered */

			/* Get The (X, Y, Z) Value For The Top Right Vertex */
			x = X + STEP_SIZE;
			y = Height(elevation, X + STEP_SIZE, Y + STEP_SIZE );
			z = Y + STEP_SIZE ;

			/* Set The Color Value Of The Current Vertex */
			SetVertexColor(elevation, x, z);
			
			glVertex3i(x, y, z);						/* Send This Vertex To OpenGL To Be Rendered */

			/* Get The (X, Y, Z) Value For The Bottom Right Vertex */
			x = X + STEP_SIZE;
			y = Height(elevation, X + STEP_SIZE, Y ); 
			z = Y;

			/* Set The Color Value Of The Current Vertex */
			SetVertexColor(elevation, x, z);

			glVertex3i(x, y, z);						/* Send This Vertex To OpenGL To Be Rendered */
      //printf("%d %d %d\n",x,y,z);
		}
	glEnd();
  /*
  p=find_peak(grid);
  glBegin(GL_POINTS);
    glColor3f(1.0f,1.0f,0.0f);
    glVertex3i(p->x,p->y,p->z);
  glEnd();
  */
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);					/*Reset The Color */
}


int drawGLScene(GLVoid)									/* Here's Where We Do All The Drawing */
{
  Layer gElevation = Project_getLayer(globalProject,"elevation");
	glClearColor(0.5,0.5,0.5,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/* Clear The Screen And The Depth Buffer */
	glLoadIdentity();									/* Reset The Matrix */

	/* 			 Position	      View		Up Vector */
	gluLookAt(eyex, eyey, eyez,  0,0,0,  0, 1, 0);	/* This Determines Where The Camera's Position And View Is */
	glRotatef(rot,rotx,roty,rotz);

	glBegin( GL_LINES);
  glColor3f(255,0,0);
      glVertex3f(0,0,0);
      glVertex3f(100,0,0);

      glColor3f(0,255,0);
      glVertex3f(0,0,0);
      glVertex3f(0,100,0);

      glColor3f(0,0,255);
      glVertex3f(0,0,0);
      glVertex3f(0,0,100);

    glEnd( );
	glScalef(scaleValue, scaleValue * HEIGHT_RATIO, scaleValue);
	glTranslatef(-(gElevation->width)/2.0,0,-(gElevation->width)/2.0);

	RenderHeightMap(gElevation);						/* Render The Height Map */
	RenderCatchment();
	return True;										/* Keep Going */
}

void update()
{
	if(isKeyDown(WK_UP))						/* Is the UP ARROW key Being Pressed? */
	{
		scaleValue += 0.01f;					/* Increase the scale value to zoom in 0.001*/
		resetKey(WK_UP);
	}

	if(isKeyDown(WK_DOWN))						/* Is the DOWN ARROW key Being Pressed? */
	{
		scaleValue -= 0.01f;					/* Decrease the scale value to zoom out */
		resetKey(WK_DOWN);
	}

	if(isKeyDown(WK_SPACE))						/* Is SPACE kes being pressed? */
	{
		bRender = !bRender;						/* toggle wireframe/fill mode*/
		resetKey(WK_SPACE);
	}
	
  if(isKeyDown(WK_LEFT))						/* Is SPACE kes being pressed? */
	{
	  rotyy+=1;						/* toggle wireframe/fill mode*/
    rot=rotyy;
		roty=1;
		rotx=0;
		rotz=0;
		resetKey(WK_LEFT);
	}

	if(isKeyDown(WK_RIGHT))						/* Is SPACE kes being pressed? */
	{
		rotyy-=1;						/* toggle wireframe/fill mode*/
    rot=rotyy;
		roty=1;
		rotx=0;
		rotz=0;
		resetKey(WK_RIGHT);
	}

	if(isKeyDown(WK_A))						/* Is SPACE kes being pressed? */
	{
    eyey+=2;
		resetKey(WK_A);
	}

	if(isKeyDown(WK_S))						/* Is SPACE kes being pressed? */
	{
    eyey-=2;
		resetKey(WK_S);
	}


	if(isKeyDown(WK_Q))						/* Is SPACE kes being pressed? */
	{

    eyex+=2;
		resetKey(WK_Q);
	}

	if(isKeyDown(WK_W))						/* Is SPACE kes being pressed? */
	{
    eyex-=2;
		resetKey(WK_W);
	}
	
  if(isKeyDown(WK_Z))						/* Is SPACE kes being pressed? */
	{
    eyez+=2;
		resetKey(WK_Z);
	}
	
  if(isKeyDown(WK_X))						/* Is SPACE kes being pressed? */
	{
    eyez-=2;
		resetKey(WK_X);
	}
}


void Elevation_view()
{
	createGLWindow("FlowViz",640,480,24,False); /* Create our window*/
	run();												/* Start Event-Loop */
	killGLWindow();										/* shutdown window */
}


