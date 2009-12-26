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

#define		STEP_SIZE	  4							/* Width And Height Of Each Quad */
#define		HEIGHT_RATIO  0.35f				/* Ratio That The Y Is Scaled According To The X And Z */


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

int simulation_time=0;
/*struct _point drop_points[]={{241,234,0},{243,232,0},{230,236,0}};*/
struct _point drop_points[100];

void dp(Grid grid, int w,int h)
{
  int i,j,k=0;
  int x=(grid->width/2)-(w/2);
  int y=(grid->length/2)-(h/2);

  for (i=y;i<(y+h);i++)
  {
    for (j=x;j<(x+w);j++)
    {
      drop_points[k].x=j;
      drop_points[k].y=i;
      k++;
    }
  }
}

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
  return (int)(elevation*(256.0/(data_set_param.max_elev-data_set_param.min_elev)));
}


int Height(Grid grid, int X, int Y)	
{
	int x = X % (int)(grid->width);						
	int y = Y % (int)(grid->length);

	if(!grid) return 0;						
	return NormalizeElevation((grid->cells[y][x]->elevation));
}

void SetVertexColor(Grid grid, int x, int y)	/* Sets The Color Value For A Particular Index, Depending On The Height Index */
{
	if(!grid)
		return;								/* Make Sure Our Height Data Is Valid */
	else
	{
		float fColor = (-0.15f + ((Height(grid, x, y )+127) / 256.0f));
		/* Assign This Blue Shade To The Current Vertex */
    if (Height(grid, x, y )<2){
      //fColor = (-0.15f + ((Height(grid, x, y )+127) / 256.0f));
		  glColor3f(0,0 ,fColor );
    }
    else if (Height(grid, x, y )>=2 && Height(grid, x, y )<5)
    {
		  glColor3f(fColor, fColor, fColor  );
    }
    else
    {
		  glColor3f(0, 1.0-fColor ,0 );
    }

	}
}

void GetPointFromDir(int dir, Point p){
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
}


void RenderFlowMap(int n, Grid grid, int maxsteps){
	int cx, cy,i,j,t,flowdir;
	struct _point p;
  FlowMap flow_map;
  
  flow_map=grid->flow_map;

  dp(grid,10,10);

  glBegin(GL_LINES);
    for (i=0;i<n;i++){
      p.x=drop_points[i].x;
		  p.y=drop_points[i].y;
	    for (t=0;t<maxsteps;t++){
        glColor3f(1.0f,1.0f,0.0f);
        glVertex3i(p.x,NormalizeElevation(grid->cells[p.y][p.x]->elevation),p.y);
        flowdir = flow_map->direction[p.y][p.x];
        GetPointFromDir(flowdir, &p);
      }
    }
  glEnd();
}


void RenderHeightMap(Grid grid)		/* This Renders The Height Map As Quads */
{
	int X = 0, Y = 0;									/* Create Some Variables To Walk The Array With. */
	int x, y, z;										/* Create Some Variables For Readability */
  Point p;


	if(!grid) return;							/* Make Sure Our Height Data Is Valid */

	if(bRender)											/* What We Want To Render */
		glBegin( GL_QUADS );							/* Render Polygons */
	else
		glBegin( GL_LINES );							/* Render Lines Instead */

	for ( X = 0; X < grid->width; X += STEP_SIZE )
		for ( Y = 0; Y < grid->length; Y += STEP_SIZE )
		{
			/* Get The (X, Y, Z) Value For The Bottom Left Vertex */
			x = X;							
			y = Height(grid, X, Y );
			z = Y;

			/* Set The Color Value Of The Current Vertex */
			SetVertexColor(grid, x, z);

			glVertex3i(x, y, z);						/* Send This Vertex To OpenGL To Be Rendered (Integer Points Are Faster) */

			/* Get The (X, Y, Z) Value For The Top Left Vertex */
			x = X;										
			y = Height(grid, X, Y + STEP_SIZE );
			z = Y + STEP_SIZE ;

			/* Set The Color Value Of The Current Vertex */
			SetVertexColor(grid, x, z);

			glVertex3i(x, y, z);						/* Send This Vertex To OpenGL To Be Rendered */

			/* Get The (X, Y, Z) Value For The Top Right Vertex */
			x = X + STEP_SIZE;
			y = Height(grid, X + STEP_SIZE, Y + STEP_SIZE );
			z = Y + STEP_SIZE ;

			/* Set The Color Value Of The Current Vertex */
			SetVertexColor(grid, x, z);
			
			glVertex3i(x, y, z);						/* Send This Vertex To OpenGL To Be Rendered */

			/* Get The (X, Y, Z) Value For The Bottom Right Vertex */
			x = X + STEP_SIZE;
			y = Height(grid, X + STEP_SIZE, Y ); 
			z = Y;

			/* Set The Color Value Of The Current Vertex */
			SetVertexColor(grid, x, z);

			glVertex3i(x, y, z);						/* Send This Vertex To OpenGL To Be Rendered */
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


int drawGLScene(GLvoid)									/* Here's Where We Do All The Drawing */
{
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
	glTranslatef(-(grid->width)/2.0,0,-(grid->width)/2.0);

	RenderHeightMap(grid);						/* Render The Height Map */
	RenderFlowMap(100,grid,200);						/* Render The Height Map */
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


int view3dGrid(Grid grid)
{
	createGLWindow("FlowViz",640,480,24,False); /* Create our window*/
	run();												/* Start Event-Loop */
	killGLWindow();										/* shutdown window */
	return 0;
}


