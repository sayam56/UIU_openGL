#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<fstream>
#include<iostream>
#include <windows.h>
#include <glut.h>
using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include<mmsystem.h>
#include "bmp.h"
#include "RGBPixMap.h"
#define pi (2*acos(0.0))
#define 	GLFW_KEY_ENTER 257
RGBpixmap pix[6];


double cameraHeight;
double cameraAngle;
int bullet=0;
int drawgrid;
int drawaxes;
double angle;
double angle1;
double angle_c_z=0;
double angle_c_x=0, angle_t_x=0, angle_t_z=0;
double limit=0.0;
double carx,cary,carz;
double obsX=600,obsY=920;
double obsXX=-450,obsYY=920-50;
double tx= -750;
double ty=920;
int i;
double carRot=0;
int carState=0;
double cx=-5;
double cy=-5;
bool moveRight=false;
bool moveLeft=false;
bool moveUp=false;
bool moveDown=false;
int keypressed=0;
double carCount;
double rotAni=45;
int checkY;
int checkX;

double rr=1,g=1,b=1;

int casX=620, casY=620;
int casXX=620, casYY=310;

bool collision = false;


//int width=800;
//int height=800;

int gameState=0;

struct point
{
	double x,y,z;
};

struct point pos,l,u,r;
double X=0,Z=0;
int Y=-1;
struct point arra[100];
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
		    //x axis green
		    glColor3f(0.0f,1.0f,0.0f);
			glVertex3f( 1000,0,0);
			glVertex3f(-1000,0,0);

			//y axis red
            glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(0,-1000,0);
			glVertex3f(0, 1000,0);


			//z axis blue
            glColor3f(0.0f,0.0f,1.0f);
			glVertex3f(0,0, 1000);
			glVertex3f(0,0,-1000);
		}glEnd();
	}
}


void drawGrid()
{
	int i;

    glColor3f(0.6, 0.6, 0.6);	//grey
    glBegin(GL_POLYGON);{
        for(i=-50;i<=50;i++){

            if(i==0)
                continue;	//SKIP the MAIN axes

            //lines parallel to Y-axis
            glVertex3f(i*15, -90, 0);
            glVertex3f(i*15,  90, 0);

            //lines parallel to X-axis
            glVertex3f(-90, i*10, 0);
            glVertex3f( 90, i*10, 0);
        }
    }glEnd();
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}

void draw_circle_line(double radius,int segments)
{
  int i;
    struct point points[100];
   // glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments/4;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    glBegin(GL_LINES);
    {
        for(i=0;i<segments;i++)
        {

            glVertex3f(points[i].x,points[i].y,0);
            //glVertex3f(points[i+1].x,points[i+1].y,0);

        }
    }
        glEnd();
}
void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];

    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    glBegin(GL_POLYGON);
    {
        for(i=0;i<segments;i++)
        {

            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);

        }
    }
        glEnd();
}

void drawCorner(double radius, int segments)
{
    int i;
    struct point points[100];

    //generate points
    for(i=0;i<=segments/4;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    glBegin(GL_POLYGON);
    {
        for(i=0;i<segments;i++)
        {

            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);

        }
    }
        glEnd();

}
void draw_trackdiv(double radius,int segments)
{

    int i;
    struct point points[100];

    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    glBegin(GL_QUADS);
    {
        for(i=0;i<segments;i++)
        {


            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);

        }
    }
        glEnd();

}

void draw_cylinder(double radius,double height,int segments)
{
    int i;
     struct point points[2][100];
     for(i=0;i<=segments;i++)
    {
        points[0][i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[0][i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points[0][i].z=0;
        points[1][i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[1][i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points[1][i].z=height;
    }

    for (i=0;i<segments;i++)
    {
        glBegin(GL_QUADS);{
			    //upper hemisphere
			    glColor3f((i)%2,.5,(i)%2);
				glVertex3f(points[0][i].x,points[0][i].y,points[0][i].z);
				glVertex3f(points[0][i+1].x,points[0][i+1].y,points[0][i+1].z);
				glVertex3f(points[1][i+1].x,points[1][i+1].y,points[1][i+1].z);
				glVertex3f(points[1][i].x,points[1][i].y,points[1][i].z);

			}glEnd();


    }
}
void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<int(segments-limit);i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,0,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}



void draw_cube()
{
    glPushMatrix();
    glPushMatrix();
    glBegin(GL_QUADS);
    //bottom surface
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glVertex3f(1,1,0);
    glVertex3f(0,1,0);

    //leftsurface
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);
    glVertex3f(1,0,1);
    glVertex3f(1,0,0);

    //front surface
    glColor3f(0,0,1);
    glVertex3f(1,0,0);
    glVertex3f(1,0,1);
    glVertex3f(1,1,1);
    glVertex3f(1,1,0);

    //right surface
    glColor3f(1,1,0);
    glVertex3f(1,1,0);
    glVertex3f(0,1,0);
    glVertex3f(0,1,1);
    glVertex3f(1,1,1);

    //back surface
    glColor3f(0,1,1);
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);
    glVertex3f(0,1,1);
    glVertex3f(0,0,1);

    //top surface
    glColor3f(1,1,1);
    glVertex3f(0,0,1);
    glVertex3f(1,0,1);
    glVertex3f(1,1,1);
    glVertex3f(0,1,1);

    glEnd();
    glPopMatrix();
}



void draw_cube1()
{
    //glPushMatrix();
    glPushMatrix();
    glBegin(GL_QUADS);
    //bottom surface
    glPushMatrix();
    //glTranslatef(5,1,0);
    glColor3f(1,0,0);
    glVertex3f(2.5,2.5,0);
    glVertex3f(2.5,-2.5,0);
    glVertex3f(-2.5,-2.5,0);
    glVertex3f(-2.5,2.5,0);
    glPopMatrix();

    //leftsurface
    glColor3f(0,1,0);
    glVertex3f(-2.5,2.5,0);
    glVertex3f(-2.5,-2.5,0);
    glVertex3f(-1,-1,-5);
    glVertex3f(-1,1,-5);

    //front surface
    glColor3f(0,0,1);
    glVertex3f(-2.5,-2.5,0);
    glVertex3f(2.5,-2.5,0);
    glVertex3f(1,-1,-5);
    glVertex3f(-1,-1,-5);
//
//    //right surface
    glColor3f(1,1,0);
    glVertex3f(2.5,2.5,0);
    glVertex3f(2.5,-2.5,0);
    glVertex3f(1,-1,-5);
    glVertex3f(1,1,-5);
//
//    //back surface
    glColor3f(0,1,1);
    glVertex3f(-2.5,2.5,0);
    glVertex3f(2.5,2.5,0);
    glVertex3f(1,1,-5);
    glVertex3f(-1,1,-5);
//
    //top surface
    glColor3f(1,1,1);
    glVertex3f(1,1,-5);
    glVertex3f(1,-1,-5);
    glVertex3f(-1,-1,-5);
    glVertex3f(-1,1,-5);

    glEnd();
    glPopMatrix();
    glPopMatrix();
}


void draw_cube2()
{
    glPushMatrix();
    glPushMatrix();
    glBegin(GL_LINES);
    //bottom surface
    //glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glVertex3f(1,1,0);
    glVertex3f(0,1,0);

    //leftsurface
    //glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);
    glVertex3f(1,0,1);
    glVertex3f(1,0,0);

    //front surface
    //glColor3f(0,0,1);
    glVertex3f(1,0,0);
    glVertex3f(1,0,1);
    glVertex3f(1,1,1);
    glVertex3f(1,1,0);

    //right surface
    //glColor3f(1,1,0);
    glVertex3f(1,1,0);
    glVertex3f(0,1,0);
    glVertex3f(0,1,1);
    glVertex3f(1,1,1);

    //back surface
    //glColor3f(0,1,1);
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);
    glVertex3f(0,1,1);
    glVertex3f(0,0,1);

    //top surface
    //glColor3f(1,1,1);
    glVertex3f(0,0,1);
    glVertex3f(1,0,1);
    glVertex3f(1,1,1);
    glVertex3f(0,1,1);

    glEnd();
    glPopMatrix();
}
void castle()
{
    glPushMatrix();
    //roof
    glPushMatrix();
    glTranslatef(0,0,-1.5);
    glScalef(.5,1,.2);
    draw_cube1();
    glPopMatrix();


    //cones

    glPushMatrix();
    glRotatef(180,0,1,0);
    glTranslatef(0-1,0-2,.5+1);
    drawCone(.5,1.2,50);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180,0,1,0);
    glTranslatef(-2+3,0-2,.5+1);
    drawCone(.5,1.2,50);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180,0,1,0);
    glTranslatef(-2+3,4-2,.5+1);
    drawCone(.5,1.2,50);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180,0,1,0);
    glTranslatef(0-1,4-2,.5+1);
    drawCone(.5,1.2,50);
    glPopMatrix();


    //pillers
    glPushMatrix();
    glTranslatef(0-1,0-2,-.5-1);
    draw_cylinder(.5,2.5,50);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2-1,0-2,-.5-1);
    draw_cylinder(.5,2.5,50);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2-1,4-2,-.5-1);
    draw_cylinder(.5,2.5,50);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0-1,4-2,-.5-1);
    draw_cylinder(.5,2.5,50);
    glPopMatrix();


    //main body
    glPushMatrix();
    glTranslatef(-1,-2,-1.5);
    glScalef(2,4,2.5);
    draw_cube();
    glPopMatrix();


    glPopMatrix();

}


void drawSphere(double radius,double slices,double stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	glPushMatrix();

	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i-.2/(double)stacks,.9);
        // glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}

	glPopMatrix();
}


void drawSphere1(double radius,double slices,double stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	glPushMatrix();

	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)rr,(double)i-.2/(double)stacks,b);
        // glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_LINES);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}

	glPopMatrix();
}

void draw_unitRoad()
{
    glPushMatrix();
    //glRotatef(angle,0,0,1);
    glTranslatef(0,0,-3);
    int x=750;
    int y=5;
    for(i=0;i<48;i++)
    {
        glPushMatrix();
        glTranslatef(x-15,0,-1);
        glScalef(2,.8,1); // x=15
        glColor3f(1,1,1);
        drawSquare(5);
        glPopMatrix();
        x=x-31;
        //printf("%d \n",x);

    }

    glScalef(150,10,1); //x=750 y=40 z=1
    glColor3f(0,0,0);
    drawSquare(5);

    glPopMatrix();
}

void draw_longRoad()
{
        //one unit road
    double tx=1200;

    glPushMatrix();

    for(int j=0;j<5;j++)
    {
        glPushMatrix();
        glTranslatef(tx,0,0);
        draw_unitRoad();
        glPopMatrix();
        tx=tx-150;
      // printf("i=%d and tx= %lf \n",i,tx);
    }

    glPopMatrix();
}

void draw_corner()
{
        glPushMatrix();

    glTranslatef(0,0,-25);

    glColor3f(0.6,1,0.6);
    glTranslatef(0,0,-.5);
    drawCorner(75,50);

     glColor3f(1,1,1);
     //glTranslatef(0,0,-1);
    draw_circle_line(104,50);

    glColor3f(0,0,0);
    glTranslatef(0,0,0);
    drawCorner(140,50);
    glPopMatrix();
}
void drawCar()
{

/*
*/



    glPushMatrix();

    glPushMatrix();
    //glRotatef(angle,0,0,1);

    //glTranslatef(-0,-0,-20);



    if(carState==1)//lower left rotation
    {


       glTranslatef(tx,ty,-25);
        glRotatef(-45,0,0,1);


    }
    if(carState==2) //lower left to right
    {
        glTranslatef(tx,ty,-25);
        if(moveRight==true)
        {
            if(carCount<=0) {moveRight=false; keypressed=0;}
            if(rotAni<=0) rotAni=45;
            glRotatef(rotAni,0,0,1);;
        }
        if(moveLeft==true)
        {
            if(carCount<=0) {moveLeft=false; keypressed=0;}
            if(rotAni<=0) rotAni=45;
            glRotatef(-rotAni,0,0,1);
        }
        if(moveLeft==false && moveRight==false)
        {
            glRotatef(0,0,0,1);
        }
        glRotatef(-90,0,0,1);
    }
    if (carState==3)//bottom right rotation
    {
        glTranslatef(tx,ty,-25);
        glRotatef(-135,0,0,1);
    }
    if (carState==4)//right to up
    {
        glTranslatef(tx,ty,-25);
        if(moveRight==true)
        {
            if(carCount<=0) {moveRight=false; keypressed=0;}
            if(rotAni<=0) rotAni=45;
            glRotatef(rotAni,0,0,1);;
        }
        if(moveLeft==true)
        {
            if(carCount<=0) {moveLeft=false; keypressed=0;}
            if(rotAni<=0) rotAni=45;
            glRotatef(-rotAni,0,0,1);
        }
        if(moveLeft==false && moveRight==false)
        {
            glRotatef(0,0,0,1);
        }
        glRotatef(-180,0,0,1);
    }
    if (carState==5)//upper right rotation
    {
        glTranslatef(tx,ty,-25);
        glRotatef(-225,0,0,1);
    }
    if (carState==6)//upper right to left
    {
        glTranslatef(tx,ty,-25);
        if(moveRight==true)
        {
            if(carCount<=0) {moveRight=false; keypressed=0;}
            if(rotAni<=0) rotAni=45;
            glRotatef(rotAni,0,0,1);;
        }
        if(moveLeft==true)
        {
            if(carCount<=0) {moveLeft=false; keypressed=0;}
            if(rotAni<=0) rotAni=45;
            glRotatef(-rotAni,0,0,1);
        }
        if(moveLeft==false && moveRight==false)
        {
            glRotatef(0,0,0,1);
        }
        glRotatef(-270,0,0,1);
    }


    if (carState==7)//upper left rotation
    {
        glTranslatef(tx,ty,-25);
        glRotatef(-315,0,0,1);
    }


    if (carState==0) // default left to down
    {
        glTranslatef(tx,ty,-25);
        if(moveRight==true)
        {
            if(carCount<=0) {moveRight=false; keypressed=0;}
            if(rotAni<=0) rotAni=45;
            glRotatef(rotAni,0,0,1);;
        }
        if(moveLeft==true)
        {
            if(carCount<=0) {moveLeft=false; keypressed=0;}
            if(rotAni<=0) rotAni=45;
            glRotatef(-rotAni,0,0,1);
        }
        if(moveLeft==false && moveRight==false)
        {
            glRotatef(0,0,0,1);
        }
        glRotatef(-360,0,0,1);

    }


    glRotatef(-90,1,0,0);
    glScalef(.8,.8,.7);
    glPushMatrix();

    //.................................................main body rectangle


    //left side
    glBegin(GL_POLYGON);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(-40.0f,0.0f,15.0f);
        glVertex3f(40.0f,0.0f,15.0f);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(40.0f,-15.0f,15.0f);
        glVertex3f(-40.0f,-15.0f,15.0f);
    glEnd();



    //back tail light side
    glBegin(GL_POLYGON);
        glColor3f(1,0,0);
        glVertex3f(40.0f,0.0f,-15.0f);
        glVertex3f(40.0f,0.0f,15.0f);
        glVertex3f(40.0f,-15.0f,15.0f);
        glVertex3f(40.0f,-15.0f,-15.0f);
    glEnd();

    //front headlight side
    glBegin(GL_POLYGON);
        glColor3f(1,0,0);
        glVertex3f(-40.0f,0.0f,-15.0f);
        glVertex3f(-40.0f,0.0f,15.0f);
        glVertex3f(-40.0f,-15.0f,15.0f);
        glVertex3f(-40.0f,-15.0f,-15.0f);
    glEnd();


    //right side
    glBegin(GL_POLYGON);
        glColor3f(1,0,0);
        glVertex3f(-40.0f,0.0f,-15.0f);
        glVertex3f(40.0f,0.0f,-15.0f);
        glColor3f(1, 1.0, 1.0);
        glVertex3f(40.0f,-15.0f,-15.0f);
        glVertex3f(-40.0f,-15.0f,-15.0f);
    glEnd();


    //front and back trunk side
    glBegin(GL_POLYGON);
        glColor3f(0,0,1);
        glVertex3f(-40.0f,0.0f,15.0f);
        glVertex3f(-40.0f,0.0f,-15.0f);
        glVertex3f(40.0f,0.0f,-15.0f);
        glVertex3f(40.0f,0.0f,15.0f);
    glEnd();


    //bottom
    glBegin(GL_POLYGON);
        glColor3f(0,0,1);
        glVertex3f(-40.0f,-15.0f,15.0f);
        glVertex3f(-40.0f,-15.0f,-15.0f);
        glVertex3f(40.0f,-15.0f,-15.0f);
        glVertex3f(40.0f,-15.0f,15.0f);
    glEnd();


    //..............................main body rectangle ends here


    //................................window rectangle
    //left window side
    glBegin(GL_POLYGON);
       glColor3f(1,1,0);
        glVertex3f(-20.0f,0.0f,15.0f);
        glVertex3f(-10.0f,10.0f,15.0f);
        glVertex3f(20.0f,10.0f,15.0f);
        glVertex3f(25.0f,0.0f,15.0f);
    glEnd();


    //right window side
    glBegin(GL_POLYGON);
        glColor3f(1,1,0);
        glVertex3f(-20.0f,0.0f,-15.0f);
        glVertex3f(-10.0f,10.0f,-15.0f);
        glVertex3f(20.0f,10.0f,-15.0f);
        glVertex3f(25.0f,0.0f,-15.0f);
    glEnd();


    //roof side
    glBegin(GL_POLYGON);
        glColor3f(.7,0,1);
        glVertex3f(-10.0f,10.0f,15.0f);
        glVertex3f(-10.0f,10.0f,-15.0f);
        glVertex3f(20.0f,10.0f,-15.0f);
        glVertex3f(20.0f,10.0f,15.0f);
    glEnd();

    //front windshield
    glBegin(GL_POLYGON);
        glColor3f(0,1,1);
        glVertex3f(-10.0f,10.0f,15.0f);
        glVertex3f(-20.0f,0.0f,15.0f);
        glVertex3f(-20.0f,0.0f,-15.0f);
        glVertex3f(-10.0f,10.0f,-15.0f);
    glEnd();

    //back glass side
    glBegin(GL_POLYGON);
        glColor3f(0,1,1);
        glVertex3f(20.0f,10.0f,15.0f);
        glVertex3f(20.0f,10.0f,-15.0f);
        glVertex3f(25.0f,0.0f,-15.0f);
        glVertex3f(25.0f,0.0f,15.0f);
    glEnd();

    //bottom invisible side
    glBegin(GL_POLYGON);
        glColor3f(0,1,1);
        glVertex3f(-30.0f,-15.0f,15.0f);
        glVertex3f(-30.0f,-15.0f,-15.0f);
        glVertex3f(30.0f,-15.0f,-15.0f);
        glVertex3f(30.0f,-15.0f,15.0f);
    glEnd();
    //......................................window body ends here


    //.......................................all the wheels
    glBegin(GL_3D);
    glColor3f(.6, .6, .6);
    glTranslated(-20.0f,-15.0f,15.0f);
    glutSolidTorus(2,5,5,100);



    glTranslated(0.0f,0.0f,-30.0f);
    glutSolidTorus(2,5,5,100);
    glTranslated(45.0f,0.0f,0.0f);
    glutSolidTorus(2,5,5,100);
    glTranslated(0.0f,0.0f,30.0f);
    glutSolidTorus(2,5,5,100);
    glEnd();
    //.....................................wheels end



    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}



void draw_Environment()
{

    // top road
    glPushMatrix();
    glTranslatef(-900,0,0);
    glRotated(90,0,0,1);
    draw_unitRoad();
    glPopMatrix();

    //right road
    glPushMatrix();
    glTranslatef(0,-900,0);
    draw_unitRoad();
    glPopMatrix();


    //left road
    glPushMatrix();
    glTranslatef(0,900,0);
    draw_unitRoad();
    glPopMatrix();


    //down road
    glPushMatrix();
    glTranslatef(900,0,0);
    glRotated(90,0,0,1);
    draw_unitRoad();
    glPopMatrix();

    //corner lower right
    glPushMatrix();
    glTranslatef(740,-740,25);
    glRotatef(272,0,0,1);
    glScalef(1.5,1.5,1);
    draw_corner();
    glPopMatrix();


    //corner lower left
    glPushMatrix();
    glTranslatef(740,740,25);
    glRotatef(2,0,0,1);
    glScalef(1.5,1.5,1);
    draw_corner();
    glPopMatrix();



    //corner upper right
    glPushMatrix();
    glTranslatef(-740,-740,25);
    glRotatef(182,0,0,1);
    glScalef(1.5,1.5,1);
    draw_corner();
    glPopMatrix();



    //corner upper left
    glPushMatrix();
    glTranslatef(-740,740,25);
    glRotatef(92,0,0,1);
    glScalef(1.5,1.5,1);
    draw_corner();
    glPopMatrix();

    //Outer-ground-green
    glPushMatrix();
    glColor3f(0.6,1,0.6);
    drawCircle(1500,70);
    glPopMatrix();


    //sky
    glPushMatrix();
    glRotatef(180,0,1,0);
	drawSphere(1500,70,50);
	glPopMatrix();






}



void keyboardListener(unsigned char key, int xx,int yy){
    double x,y,z;
    double rate = 0.01;

    if(key==13) //enter key
        gameState=1;

    if (key == 27) // escape key
        exit(0);

    if (key==8) //backspace key
    {
        gameState=0;
        keypressed=0;
    }



	switch(key){

		case '1':

			{
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(rate)+1*r.x*sin(rate);
			l.y = l.y*cos(rate)+r.y*sin(rate);
			l.z = l.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-x*sin(rate);
			r.y = r.y*cos(rate)-y*sin(rate);
			r.z = r.z*cos(rate)-z*sin(rate);}
			break;
        case '2':

			{
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(-rate)+r.x*sin(-rate);
			l.y = l.y*cos(-rate)+r.y*sin(-rate);
			l.z = l.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-x*sin(-rate);
			r.y = r.y*cos(-rate)-y*sin(-rate);
			r.z = r.z*cos(-rate)-z*sin(-rate);
			}
			break;
        case '3':
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-x*sin(rate);
			u.y = u.y*cos(rate)-y*sin(rate);
			u.z = u.z*cos(rate)-z*sin(rate);
			break;
        case '4':
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(-rate)+1*u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-x*sin(-rate);
			u.y = u.y*cos(-rate)-y*sin(-rate);
			u.z = u.z*cos(-rate)-z*sin(-rate);
			break;
        case '6':
            x=r.x;y=r.y;z=r.z;
			r.x = r.x*cos(rate)+u.x*sin(rate);
			r.y = r.y*cos(rate)+u.y*sin(rate);
			r.z = r.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-x*sin(rate);
			u.y = u.y*cos(rate)-y*sin(rate);
			u.z = u.z*cos(rate)-z*sin(rate);
			break;
        case '5':
            x=r.x;y=r.y;z=r.z;
			r.x = r.x*cos(-rate)+u.x*sin(-rate);
			r.y = r.y*cos(-rate)+u.y*sin(-rate);
			r.z = r.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-x*sin(-rate);
			u.y = u.y*cos(-rate)-y*sin(-rate);
			u.z = u.z*cos(-rate)-z*sin(-rate);
			break;
        case 'd':

            pos.x-=r.x;
			pos.y-=r.y;
			pos.z-=r.z;
            break;

        case 'a':

            pos.x+=r.x;
			pos.y+=r.y;
			pos.z+=r.z;
            break;
        case 'w':

            pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
            break;

        case 's':

            pos.x-=l.x;
			pos.y-=l.y;
			pos.z-=l.z;
            break;

        case 'i':
            gameState=2;
            break;


		default:
		    moveLeft=false;
            moveRight=false;
            keypressed=0;
			break;
	}

}




void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			moveUp=true;
            moveDown=false;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			moveDown=true;
            moveUp=false;
			break;

		case GLUT_KEY_LEFT :
			moveLeft=true;
            keypressed+=1;
            moveRight=false;
            carCount=50;
			break;
		case GLUT_KEY_RIGHT :
			moveRight=true;
            keypressed+=1;
            moveLeft=false;
            carCount=50;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
		    moveLeft=false;
            moveRight=false;
            keypressed=0;
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
		    if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				/*bullet =1;
				double length = 649;
				//double length = (649/cos(angle_c_x*pi/180))/cos(angle_t_x*pi/180);
                printf("---> %lf\n",length);
                //X = length* sin(angle_c_z*pi/180);
                X= length* tan(angle_c_z*pi/180);

                Z=-100*sin(angle_c_x*pi/180)-(649-100*cos(angle_c_x*pi/180))*tan((angle_t_x+angle_c_x)*pi/180);*/



			}
			break;


		case GLUT_RIGHT_BUTTON:
		    if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
				//(600/cos(angle_c_x*pi/180))/cos(angle_t_x*pi/180);
			}
			break;
			//........

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(-900,-900,-15,	-950,-850,-25,	0,0,-1);
	//gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);
	//gluLookAt(pos.x,pos.y,pos.z,	-950,-850,-25,	0,0,-1);
	//gluLookAt(-carx+25,-cary+25,-100,	0,0,-25,	0,0,-1);
	//gluLookAt(0,0,-1500, 0,0,0,	0,0,1);

    //gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);

//	printf("r vect X: %lf   R vect y: %lf    R vect z: %lf\n",pos.x,pos.y,pos.z);
//	printf("l vect X: %lf   l vect y: %lf    l vect z: %lf\n",pos.x+l.x,pos.y+l.y,pos.z+l.z);
//	printf("u vect X: %lf   u vect y: %lf    u vect z: %lf\n",u.x,u.y,u.z);

//    printf("tx: %lf     ty: %lf \n",tx,ty);
//    printf("keypressed %d\n",keypressed);
	//again select MODEL-VIEW


    if(gameState==0) //menu
    {

        glMatrixMode(GL_MODELVIEW);
       // pix[0].draw();
       gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);
//        drawSquare(5);
       // drawAxes();
        glPushMatrix();
        glColor3d(rr,rr,b);
        glRotatef(angle,1,1,-1);
        glTranslatef(0,0,-80);
        drawSphere1(40,40,25);

        glPopMatrix();

        glPushMatrix();
        //glColor3d(rr,rr,b);
        glRotatef(-angle,1,-1,1);
        glTranslatef(0,0,-80);
        glRotatef(angle,1,1,1);
        drawSphere1(20,20,22);

        glPopMatrix();



       glPushMatrix();
       glColor3f(rr,b,b);
        glScalef(50,50,1);
        glRotatef(angle,-1,0,1);
        glTranslated(2,rr,b);
        glRotatef(45,1,1,0);
        draw_cube2();
        glPopMatrix();
//        int width,height,components;
//        unsigned char *imageData = stbi_load("container.jpg",
//        &width, &height, &components, STBI_rgb_alpha);
        //glColor3d(rr,g,b);


       // printf("%red:%lf green:%lf  blue:%lf \n",rr,g,b);


        glPushMatrix();
        glColor3d(rr,g,b);
            unsigned char str[] = "Press 'ESC' To Exit to Main Menu";
            int w;
            w = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, str);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(30,65);
            glColor3d(rr,g,b);
            int len = strlen( (char*)str  );
            for (i = 0; i < len; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
                }

        glPopMatrix();




        glPushMatrix();
        glColor3d(g,b,g);
            unsigned char strr[] = "Press 'I' for Instructions";
            int wx;
            wx = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, strr);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(15,40);
           // glColor3d(rr,g,b);
            int lenn = strlen( (char*)strr  );
            for (i = 0; i < lenn; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, strr[i]);
                }

        glPopMatrix();



        glPushMatrix();
        glColor3d(b,rr,g);
            unsigned char str1[] = "Press 'Enter' to Start The Game";
            int ww;
            ww = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, str1);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(0,60);

            int len2 = strlen( (char*)str1  );
            for (i = 0; i < len2; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str1[i]);
                }

        glPopMatrix();

        glPushMatrix();
        glColor3d(g,rr,b);
            unsigned char str2[] = "WELCOME TO STAKES RACING";
            int www;
            www = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, str2);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(-70,60);

            int len3 = strlen( (char*)str2  );
            for (i = 0; i < len3; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str2[i]);
                }




        glPopMatrix();

        glPushMatrix();
        glColor3d(g,rr,b);
            unsigned char str3[] = "!!!where the stakes are high!!!";
            int wwww;
            wwww = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, str3);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(-60,-30);

            int len4 = strlen( (char*)str3  );
            for (i = 0; i < len4; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str3[i]);
                }

        glPopMatrix();


        glPushMatrix();
        //glTranslatef(0,0,0);
        //drawCar();
        glPopMatrix();


//        //glClearColor(0,0,0,0);
        glutSwapBuffers();
//        glFlush();
    }




	/****************************
	/ Add your objects from here
	****************************/
	//add objects



    if(gameState==1)//In Game
    {
        glMatrixMode(GL_MODELVIEW);


//            glPushMatrix();
//            glColor3d(0,0,0);
//            unsigned char strr[] = "score";
//            int wx;
//            wx = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, strr);
//            glRasterPos2f(-10, 10);
//            float x = 1; /* Centre in the middle of the window */
//            glRasterPos2f(25,25);
//            glColor3d(rr,g,b);
//            int lenn = strlen( (char*)strr  );
//            for (i = 0; i < lenn; i++)
//                {
//                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, strr[i]);
//                }
//
//
//
//        glPopMatrix();


        //drawAxes();
        if(collision == true)
        {
            gameState=3;
        }



        if (carState==0)  //left to down
        {
           // gluLookAt(0,0,-1400,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);
            gluLookAt(-813.31+cx-35,900,-69.3, 812.32-35,900,-69.22, 0,0,-1);



            //----------------------------------------------------------------------------------------collision logic for state 0
            if ( ( (tx-40) < (-obsX-20) && (tx+40)>(-obsX+20) && ty==obsY )   )
            {
                printf("Collision detected 1-1\n");
                collision=true;
            }

            if ( ( (tx-40) < (-obsX+250-20) && (tx+40)>(-obsX+250+20) && ty>(882.2-20) && ty<882.2+20 ) )
            {
                printf("Collision detected 1-2\n");
                collision=true;
            }

            if ( ( (tx-40) < (-obsX+550-20) && (tx+40)>(-obsX+550+20) && ty==obsY )   )
            {
                printf("Collision detected 1-3\n");
                collision=true;
            }

            if ( ( (tx-40) < (-obsX+950-20) && (tx+40)>(-obsX+950+20) && ty>(882.2-20) && ty<882.2+20 ) )
            {
                printf("Collision detected 1-4\n");
                collision=true;
            }

            if ( ( (tx-40) < (-obsX+1250-20) && (tx+40)>(-obsX+1250+20) && ty>(882.2-20) && ty<882.2+20 ) )
            {
                printf("Collision detected 1-5\n");
                collision=true;
            }
            //--------------------------------------------------------------------------------------------------------------

                //else printf("No collision\n");


                glPushMatrix();
                //printf("step1 TX: %lf TY: %lf obsx: %lf obsy: %lf",tx-40,ty,-obsX+250-20,obsY-37.8);
                //printf("step2 TX: %lf TY: %lf obsx: %lf obsy: %lf\n",tx+40,ty,-obsX+250+20,obsY-37.8);
              // printf("OBSX: %lf OBSY: %lf\n",-obsX+250-20,obsY-37.8);
             // printf("TY: %lf obsY: %lf\n",ty,obsY-37.8);

                glTranslatef(-obsX,obsY,-30);//obj 1
                glRotatef(angle1,1,1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();

                glPushMatrix(); //obj 2
                glTranslatef(-obsX+250,obsY-37.8,-30);
                glRotatef(-angle1,1,1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();

                glPushMatrix();//obj 3
                glTranslatef(-obsX+550,obsY,-30);
                glRotatef(angle1,-1,1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();

                glPushMatrix();//obj 4
                glTranslatef(-obsX+950,obsY-37.8,-30);
                glRotatef(-angle1,1,1,1);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();


                glPushMatrix(); //obj 5
                glTranslatef(-obsX+1250,obsY-37.8,-30);
                glRotatef(-angle1,1,-1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();



            glPushMatrix();



            drawCar();

//            glPushMatrix();//start point
//            glTranslatef(0,0,-1000);
//
//            glScalef(25,25,25);
//            glColor3f(1,1,1);
//            draw_cube();
//            glPopMatrix();

            glPopMatrix();




        }



        if(carState == 1) //lower left rotation
        {
            gluLookAt(696.46,679.60,-69.3, 697.15,680.33,-69.22, 0,0,-1);
            glPushMatrix();
            glRotatef(carRot,0,0,-1);
            //glTranslatef(tx,ty,-25);
            drawCar();
            glPopMatrix();
        }

        if (carState==2) //bottom left to right
        {
            //gluLookAt(0,0,-1400,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);
            gluLookAt(901.2,819.43+cy+40,-69.3, 901.12,-818.44+40,-69.22, 0,0,-1);

            //----------------------------------------------------------------------------------------collision logic for state 2

            //printf("step1 TX: %lf TY: %lf obsx: %lf obsy: %lf",tx,ty-40,obsY,obsX-20-37.8);
            //printf("step2 TX: %lf TY: %lf obsx: %lf obsy: %lf\n",tx,ty+40,obsY,obsX-37.8+20);

            if ( ( (ty-40) < (-obsX-20) && (ty+40)>(-obsX+20) && tx>(882.2-20) && tx<882.2+20 )   )
            {
                printf("Collision detected 2-1\n");
                collision=true;
            }

            if ( ( (ty-40) < (obsX-37.8-20) && (ty+40)>(obsX-37.8+20) && tx==obsY ) )
            {
                printf("Collision detected 2-2\n");
                collision=true;
            }

            if ( ( (ty-40) < (obsX-250-20) && (ty+40)>(obsX-250+20) && tx>(882.2-20) && tx<882.2+20 )   )
            {
                printf("Collision detected 2-3\n");
                collision=true;
            }

            if ( ( (ty-40) < (obsX-500-20) && (ty+40)>(obsX-500+20) && tx>(882.2-20) && tx<882.2+20 ) )
            {
                printf("Collision detected 2-4\n");
                collision=true;
            }

            if ( ( (ty-40) < (obsX-750-20) && (ty+40)>(obsX-750+20) && tx==obsY ) )
            {
                printf("Collision detected 2-5\n");
                collision=true;
            }
            //--------------------------------------------------------------------------------------------------------------

            glPushMatrix();
            //glTranslatef(tx,ty,-25);

                glPushMatrix(); //obj 6
                glTranslatef(obsY-37.8,-obsX,-30);
                glRotatef(angle1,1,1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();

                glPushMatrix(); //obj 7
                glTranslatef(obsY,obsX-37.8,-30);
                glRotatef(-angle1,1,1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();

                glPushMatrix(); //obj 8
                glTranslatef(obsY-37.8,obsX-250,-30);
                glRotatef(angle1,-1,1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();

                glPushMatrix(); //obj 9
                glTranslatef(obsY-37.8,obsX-500,-30);
                glRotatef(-angle1,1,1,1);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();


                glPushMatrix(); //obj 10
                glTranslatef(obsY,obsX-750,-30);
                glRotatef(-angle1,1,-1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();




            drawCar();
            glPopMatrix();
        }

        if (carState==3) //bottom right rotation
        {
            gluLookAt(675.30,-686.63,-69.3, 676.04,-687.30,-69.22, 0,0,-1);
            glPushMatrix();
            glRotatef(carRot,0,0,-1);
            //glTranslatef(tx,ty,-25);
            drawCar();
            glPopMatrix();
        }



        if (carState == 4) // right to up
        {
            //gluLookAt(0,0,-1400,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);
            gluLookAt(778.53+cx+80,-900.88,-69.3, -777.69+80,-900.87,-69.22, 0,0,-1);


            //----------------------------------------------------------------------------------------collision logic for state 4
            if ( ( (tx-40) < (-obsX-20) && (tx+40)>(-obsX+20) && ty==-obsY )   )
            {
                printf("Collision detected 4-1\n");
                collision=true;
            }

            if ( ( (tx-40) < (-obsX+250-20) && (tx+40)>(-obsX+250+20) && ty>(-882.2-20) && ty<(-882.2+20) ) )
            {
                printf("Collision detected 4-2\n");
                collision=true;
            }

            if ( ( (tx-40) < (-obsX+550-20) && (tx+40)>(-obsX+550+20) && ty==-obsY )   )
            {
                printf("Collision detected 4-3\n");
                collision=true;
            }

            if ( ( (tx-40) < (-obsX+950-20) && (tx+40)>(-obsX+950+20) && ty>(-882.2-20) && ty<(-882.2+20) ) )
            {
                printf("Collision detected 4-4\n");
                collision=true;
            }

            if ( ( (tx-40) < (-obsX+1250-20) && (tx+40)>(-obsX+1250+20) && ty>(-882.2-20) && ty<(-882.2+20) ) )
            {
                printf("Collision detected 4-5\n");
                collision=true;
            }
            //--------------------------------------------------------------------------------------------------------------



            glPushMatrix();
            //glTranslatef(tx,ty,-25);

                glPushMatrix();
                glTranslatef(-obsX,-obsY,-30);
                glRotatef(angle1,1,1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(-obsX+250,-obsY+37.8,-30);
                glRotatef(-angle1,1,1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(-obsX+550,-obsY,-30);
                glRotatef(angle1,-1,1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(-obsX+950,-obsY+37.8,-30);
                glRotatef(-angle1,1,1,1);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();


                glPushMatrix();
                glTranslatef(-obsX+1250,-obsY+37.8,-30);
                glRotatef(-angle1,1,-1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();


            drawCar();
            glPopMatrix();
        }



        if(carState ==5) //upper right rotation
        {
           gluLookAt(-690.99,-684.59,-69.3, -691.68,-685.31,-69.22, 0,0,-1);
            glPushMatrix();
            glRotatef(carRot,0,0,-1);
            //glTranslatef(tx,ty,-25);
            drawCar();
            glPopMatrix();
        }

        if(carState==6) //upper right to left
        {
            gluLookAt(-892.97,-836.39+cy-10,-69.3, -892.98,835.39-10,-69.22, 0,0,-1);
            //gluLookAt(0,0,-1400,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);


            //----------------------------------------------------------------------------------------collision logic for state 2

            //printf("step1 TX: %lf TY: %lf obsx: %lf obsy: %lf",tx,ty-40,obsY,obsX-20-37.8);
            //printf("step2 TX: %lf TY: %lf obsx: %lf obsy: %lf\n",tx,ty+40,obsY,obsX-37.8+20);

            if ( ( (ty-40) < (-obsX+50-20) && (ty+40)>(-obsX+50+20) && tx>(-882.2-20) && (tx<-882.2+20) )   )
            {
                printf("Collision detected 6-1\n");
                collision=true;
            }

            if ( ( (ty-40) < (obsX-37.8-20) && (ty+40)>(obsX-37.8+20) && tx== (-obsY) ) )
            {
                printf("Collision detected 6-2\n");
                collision=true;
            }

            if ( ( (ty-40) < (-obsX+250-20) && (ty+40)>(-obsX+250+20) && tx==(-obsY) )   )
            {
                printf("Collision detected 6-3\n");
                collision=true;
            }

            if ( ( (ty-40) < (-obsX+670-20) && (ty+40)>(-obsX+670+20) && tx>(-882.2-20) && tx<(-882.2+20) ) )
            {
                printf("Collision detected 6-4\n");
                collision=true;
            }
//
//            if ( ( (ty-40) < (obsX-750-20) && (ty+40)>(obsX-750+20) && tx== (-obsY) ) )
//            {
//                printf("Collision detected 6-5\n");
//                collision=true;
//            }
            //--------------------------------------------------------------------------------------------------------------



            glPushMatrix();
            //glTranslatef(tx,ty,-25);


            glPushMatrix();
                glTranslatef(-obsY+37.8,-obsX+50,-30);//obj 1
                glRotatef(angle1,1,1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();

                glPushMatrix(); //obj 2
                glTranslatef(-obsY,obsX-37.8,-30);
                glRotatef(-angle1,1,1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();

                glPushMatrix(); //obj 3
                glTranslatef(-obsY,-obsX+250,-30);
                glRotatef(angle1,-1,1,0);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();

                glPushMatrix(); //obj4
                glTranslatef(-obsY+37.8,-obsX+670,-30);
                glRotatef(-angle1,1,1,1);
                //glColor3b(1,.5,.5);
                drawSphere1(20,25,35);
                glPopMatrix();
//
//
//                glPushMatrix();
//                glTranslatef(obsY,obsX-750,-30);
//                glRotatef(-angle1,1,-1,0);
//                //glColor3b(1,.5,.5);
//                drawSphere1(20,25,35);
//                glPopMatrix();



            drawCar();
            glPopMatrix();
        }

         if(carState==7) //upper left rotation
        {
           gluLookAt(-699.06,725.57,-69.3, -699.73,726.15,-69.22, 0,0,-1);
            glPushMatrix();
            glRotatef(carRot,0,0,-1);
            //glTranslatef(tx,ty,-25);
            drawCar();
            glPopMatrix();
        }


        for(int z=0;z<4;z++)  //corner castles
        {
            if(z%2==0)
            {
                casX=casX*(-1);
            }
            else
                casY=casY*(-1);


            glPushMatrix();
            glTranslatef(casX,casY,-25);
            glScalef(45,45,45);
            castle();
            glPopMatrix();

        }

        for(int z=0;z<4;z++)
        {
            if(z%2==0)
            {
                glPushMatrix();
                glTranslatef(casXX,casYY,-25);
                glScalef(45,45,45);
                castle();
                glPopMatrix();

                glPushMatrix();
                glTranslatef(casYY,casXX,-25);
                glScalef(45,45,45);
                castle();
                glPopMatrix();

                casXX=casXX*(-1);
            }
            else
            {
                glPushMatrix();
                glTranslatef(casYY,casXX,-25);
                glScalef(45,45,45);
                castle();
                glPopMatrix();


                glPushMatrix();
                glTranslatef(casXX,casYY,-25);
                glScalef(45,45,45);
                castle();
                glPopMatrix();


                casYY=casYY*(-1);
            }





        }

        draw_Environment();




        //drawCone(20,50,24);

        //draw_cube(5,2,2,2);
        //drawSurface();




        //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
        glutSwapBuffers();

    }


    if (gameState==2)//instructions
        {
            glMatrixMode(GL_MODELVIEW);


             gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);
//        drawSquare(5);
       // drawAxes();
        glPushMatrix();
        glColor3d(rr,rr,b);
        glRotatef(-angle,-1,1,-1);
        glTranslatef(0,0,-80);
        drawSphere1(40,40,25);

        glPopMatrix();

        glPushMatrix();
        //glColor3d(rr,rr,b);
        glRotatef(-angle,1,1,-1);
        glTranslatef(0,0,-80);
        glRotatef(angle,1,1,1);
        drawSphere1(20,20,22);

        glPopMatrix();



       glPushMatrix();
       glColor3f(rr,b,b);
        glScalef(50,50,1);
        glRotatef(-angle,-1,0,-1);
        glTranslated(2,rr,b);
        glRotatef(45,1,1,0);
        draw_cube2();
        glPopMatrix();
//        int width,height,components;
//        unsigned char *imageData = stbi_load("container.jpg",
//        &width, &height, &components, STBI_rgb_alpha);
        //glColor3d(rr,g,b);


       // printf("%red:%lf green:%lf  blue:%lf \n",rr,g,b);


        glPushMatrix();
        glColor3d(1,1,1);
            unsigned char str[] = "->Press The Up Arrow To Move Forward";
            int w;
            w = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, str);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(-55,75);
            glColor3d(rr,g,b);
            int len = strlen( (char*)str  );
            for (i = 0; i < len; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
                }

        glPopMatrix();




        glPushMatrix();
        glColor3d(1,1,1);
            unsigned char strr[] = " ->Press The Down Arrow To Break ";
            int wx;
            wx = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, strr);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(-40,75);
           // glColor3d(rr,g,b);
            int lenn = strlen( (char*)strr  );
            for (i = 0; i < lenn; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, strr[i]);
                }

        glPopMatrix();



        glPushMatrix();
        glColor3d(1,1,1);
            unsigned char strr1[] = " ->Press The Right Arrow To Move Right ";
            int wx1;
            wx1 = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, strr1);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(-25,75);
           // glColor3d(rr,g,b);
            int lenn1 = strlen( (char*)strr1  );
            for (i = 0; i < lenn1; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, strr1[i]);
                }

        glPopMatrix();




        glPushMatrix();
        glColor3d(1,1,1);
            unsigned char str1[] = " ->Press The Left Arrow To Move Right ";
            int w1;
            w1 = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, str1);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(-10,75);
           // glColor3d(rr,g,b);
            int len1 = strlen( (char*)str1  );
            for (i = 0; i < len1; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str1[i]);
                }

        glPopMatrix();


        glPushMatrix();
        glColor3d(1,1,1);
            unsigned char str12[] = " ->Avoid Hitting the Circle of DOOM!! ";
            int w12;
            w12 = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, str12);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(5,75);
           // glColor3d(rr,g,b);
            int len12 = strlen( (char*)str12  );
            for (i = 0; i < len12; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str12[i]);
                }

        glPopMatrix();



        glPushMatrix();
        glColor3d(1,1,1);
            unsigned char str13[] = " ->Press 'BackSpace' At Any Time To Go To Main Menu ";
            int w13;
            w13 = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, str13);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(20,95);
           // glColor3d(rr,g,b);
            int len13 = strlen( (char*)str13  );
            for (i = 0; i < len13; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str13[i]);
                }

        glPopMatrix();


         glPushMatrix();
        glColor3d(1,1,1);
            unsigned char str14[] = " ->Press 'ESC' At Any Time To Go To EXIT ";
            int w14;
            w14 = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, str14);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(35,85);
           // glColor3d(rr,g,b);
            int len14 = strlen( (char*)str14  );
            for (i = 0; i < len14; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str14[i]);
                }

        glPopMatrix();


            glutSwapBuffers();
        }


        if(gameState==3)//gameover
        {
            glMatrixMode(GL_MODELVIEW);


             gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);
            // drawSquare(5);
           // drawAxes();
            glPushMatrix();
            glColor3d(rr,rr,b);
            glRotatef(-angle,-1,1,-1);
            glTranslatef(0,0,-80);
            drawSphere1(40,40,25);

            glPopMatrix();

            glPushMatrix();
            //glColor3d(rr,rr,b);
            glRotatef(-angle,1,1,-1);
            glTranslatef(0,0,-80);
            glRotatef(angle,1,1,1);
            drawSphere1(20,20,22);

            glPopMatrix();



           glPushMatrix();
           glColor3f(rr,b,b);
            glScalef(50,50,1);
            glRotatef(-angle,-1,0,-1);
            glTranslated(2,rr,b);
            glRotatef(45,1,1,0);
            draw_cube2();
            glPopMatrix();
//        int width,height,components;
//        unsigned char *imageData = stbi_load("container.jpg",
//        &width, &height, &components, STBI_rgb_alpha);
        //glColor3d(rr,g,b);


       // printf("%red:%lf green:%lf  blue:%lf \n",rr,g,b);


        glPushMatrix();
        glColor3d(1,1,1);
            unsigned char str[] = "!!!   GameOver   !!!";
            int w;
            w = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, str);
           // glRasterPos2f(-10, 10);
            //float x = 1; /* Centre in the middle of the window */
            glRasterPos2f(-40,30);
            glColor3d(g,g,b);
            int len = strlen( (char*)str  );
            for (i = 0; i < len; i++)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
                }

        glPopMatrix();



            glutSwapBuffers();
        }
}


void animate(){
	//display();
	angle+=.08;
	//tx+=.89;
	angle1+=.5;
	carCount-=.8;
    rotAni-=.4;
	//car animate

    rr-=.0009;
    g-=.0008;
    b-=.00009;

    if(rr<=0){
        rr=1;
    }
    if(g<=0)
    {
        g=1;

    }

    if(b<=0){
        b=1;
    }






	if(carState == 0)
    {
        if(moveUp==true && gameState==1)
        {
            tx+=2;
            cx+=2;

        }

        if(keypressed == 1 && moveRight==true && moveLeft==false && gameState==1)
        {
           // if(ty>=checkY && tx>=checkX)
            //{
                ty+=.6;
              //  checkY=0;
            //}

        }
        if(keypressed == 1 && moveRight==false && moveLeft==true && gameState==1)
        {
            //if(ty>=checkY)
            //{
                ty-=.6;
            //}
        }



        carRot=0;
    }

	if( (ty == 920 && tx>=730) || (tx>=730 && ty>=880) ) {
        carState=1;
        carRot+=.1;
        	if(carRot >=12)
        {
            carState=2;
           // printf("state 2 ");
           // printf("%lf %lf \n",tx,ty);
            ty=750; tx=920;
            cx=-5;cy=-5;
        }
	}



	if (carState == 2)
    {
        carRot=0;
        //printf("state 2");
        if(moveUp==true && gameState==1)
        {
            ty-=2;
            cy-=2;
        }
        if(keypressed == 1 && moveRight==true && moveLeft==false && gameState==1)
        {
           // if(ty>=checkY && tx>=checkX)
            //{
                tx+=.6;
              //  checkY=0;
            //}

        }
        if(keypressed == 1 && moveRight==false && moveLeft==true && gameState==1)
        {
            //if(ty>=checkY)
            //{
                tx-=.6;
            //}
        }

    }

    if( (tx == 920 && ty <= -705) || (tx>=880 && ty <= -705) )
    {
//        tx= -920;
//        ty+=.89;
            carState=3;
            ty=-750; carRot+=.1;
            if(carRot >=12)
            {
                carState=4;

                //printf("%lf %lf \n",tx,ty);
                ty=-920; tx=750;
                cx=-5; cy=-5;
            }
    }

    if (carState==4)
    {
        carRot=0;
        //printf("state 4 ");
        if(moveUp==true && gameState==1)
        {
            tx-=2;
            cx-=2;
        }

        if(keypressed == 1 && moveRight==true && moveLeft==false && gameState==1)
        {
           // if(ty>=checkY && tx>=checkX)
            //{
                ty-=.6;
              //  checkY=0;
            //}

        }
        if(keypressed == 1 && moveRight==false && moveLeft==true && gameState==1)
        {
            //if(ty>=checkY)
            //{
                ty+=.6;
            //}
        }

    }



    if (  (ty==-920 && tx<=-730) || (tx<=-730 && ty<=-880) )
    {
        carState=5; carRot+=.1;
        //printf("State 5 ");
        if(carRot >=12)
        {
            carState=6;

            //printf("%lf %lf \n",tx,ty);
            ty=-750; tx=-920;
            cx=-5;cy=-5;
        }

    }

    if(carState==6 && gameState==1)
    {
        carRot=0;
        //printf("State 6 ");
        if(moveUp==true)
        {
            ty+=2;
            cy+=2;
        }

        if(keypressed == 1 && moveRight==true && moveLeft==false && gameState==1)
        {
           // if(ty>=checkY && tx>=checkX)
            //{
                tx-=.6;
              //  checkY=0;
            //}

        }
        if(keypressed == 1 && moveRight==false && moveLeft==true && gameState==1)
        {
            //if(ty>=checkY)
            //{
                tx+=.6;
            //}
        }


    }

    if( (tx==-920 && ty>=730) || (tx<=-880 && ty>730) )
    {
        carState=7; carRot+=.1;
       // printf("state 7 ");

        if(carRot >=12)
        {
            carState=0;

            //printf("%lf %lf \n",tx,ty);
            tx=-750; ty=920;
            cx=-5; cy=-5;
        }

    }
//    if (tx == -920 && ty>=750) {carState=0; tx=-750; ty= 920;}


	//printf("%lf %lf \n",carx,cary);
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	//cameraHeight=150.0;
	//cameraAngle=1.0;
	//angle=0;

	//clear the screen
	glClearColor(0,0,0,.5);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix


        glMatrixMode(GL_PROJECTION);

        //initialize the matrix
        glLoadIdentity();

        //give PERSPECTIVE parameters
        gluPerspective(80,	1,	1,	5000.0);
        //field of view in the Y (vertically)
        //aspect ratio that determines the field of view in the X direction (horizontally)
        //near distance
        //far distance

}

int main(int argc, char **argv){
//    pos.x=0;
//    pos.y=900;
//    pos.z=-200;

//    pos.x=100;
//    pos.y=100;
//    pos.z=-100;


    pos.x=0;
    pos.y=0;
    pos.z=-150;

//     pos.x=790;
//    pos.y=-901;
//    pos.z=-65.6;

    l.x=0;u.x=-1;r.x=0;
    l.y=0;u.y=0;r.y=1;
    l.z=1;u.z=0;r.z=0;
	glutInit(&argc,argv);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(500, 50);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");
//    pix[0].readBMPFile("bg.bmp");
    //BitMap example_bmp("images.bmp");
    // get the vector <R,G,B> for the pixel at (1,1)
//    std::vector<unsigned int> example_vector = example_bmp.getPixel(1,1);



// ... do something with the image

    //stbi_image_free(image);

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
