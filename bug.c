/*
 *  3D Objects
 *
 *  Demonstrates how to draw objects in 3D.
 *
 *  Key bindings:
 *  m/M        Cycle through different sets of objects
 *  a          Toggle axes
 *  arrows     Change view angle
 *  0          Reset view angle
 *  ESC        Exit
 */

#include "CSCIx229.h"

int th=0;         //  Azimuth of view angle
int ph=30;         //  Elevation of view angle
double zh=0;      //  Rotation of teapot
int axes=1;       //  Display axes
int light=1;
int ylight=5;
int mode=0;       //  What to display
double color=0;
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=10.0;   //  Size of world
float angle=0.0;
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   15;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=2.0f,z=3.5f;

double Ex, Ey, Ez;
int sky_dim=5;
int ground1[2];
int sky[2];
int leaf[2];
GLuint makeaTree;
double bug1[3]={2.5,-2.4,3.1};
double bug2[3]={2.5,-2.4,3.1};
double small[3]={0.7,-2.4,0.95};
double pos[5][3]={
   {3,-4.2,2.8},
{4,-4.2,1.2},
{1,-4.2,7.2},
{8,-4.2,2.2},
{2.8,-4.2,7.4}
};
double incry=0.2;
double incr=0.2;
int wind=0;
int fog=0;
GLfloat density = 0.05; 

GLfloat fogColor[4] = {0.6, 0.6, 0.6, 1.0}; //set the for



static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}
static void light_ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,0);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny+15);
   // glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

static void Sky(double D)
{
   glPushMatrix();

   glColor3f(1,1,1);
   glTranslated(D,0,D);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,sky[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   //  Top and bottom
   // glBegin(GL_QUADS);
   // glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
   // glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
   // glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
   // glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);
   // glEnd();

   // glEnable(GL_TEXTURE_2D);
   // glBindTexture(GL_TEXTURE_2D,ground1[0]);

   glBegin(GL_QUADS);

   glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glEnd();

   // glDisable(GL_TEXTURE_2D);


   glPopMatrix();

}



static void stem(double x,double y,double z,double cx,double cy,double cz,double r,double d,double th,double tx, double ty,double tz)
{
   int i,k;
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   // glScalef(1.4, 0.7, 1.0);
   glColor3f(cx,cy,cz);
   glRotated(th,tx,ty,tz);
   glScaled(r,r,d);
   //  Head & Tail
   for (i=1;i>=-1;i-=2)
   {
      // glBindTexture(GL_TEXTURE_2D,i>0?tail:head);
      glNormal3f(0,0,i);
      glBegin(GL_TRIANGLE_FAN);
      glVertex3f(0,0,i);
      for (k=0;k<=360;k+=10)
      {
         // glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
         glVertex3f(i*Cos(k),Sin(k),i);
      }
      glEnd();
   }
   //  Edge
   glBegin(GL_QUAD_STRIP);

   for (k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),+1);
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
}


static void ground(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

   //  Save transformation
   glPushMatrix();
   glColor3f(0.5,0.6,0.5);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,ground1[0]);
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glVertex3f(-1.0,-1.0, 1.0);
   glVertex3f(+1.0,-1.0, 1.0);
   glVertex3f(+1.0,+1.0, 1.0);
   glVertex3f(-1.0,+1.0, 1.0);
   //  Back
   glVertex3f(+1.0,-1.0,-1.0);
   glVertex3f(-1.0,-1.0,-1.0);
   glVertex3f(-1.0,+1.0,-1.0);
   glVertex3f(+1.0,+1.0,-1.0);
   //  Right
   glVertex3f(+1.0,-1.0,+1.0);
   glVertex3f(+1.0,-1.0,-1.0);
   glVertex3f(+1.0,+1.0,-1.0);
   glVertex3f(+1.0,+1.0,+1.0);
   //  Left
   glVertex3f(-1.0,-1.0,-1.0);
   glVertex3f(-1.0,-1.0,+1.0);
   glVertex3f(-1.0,+1.0,+1.0);
   glVertex3f(-1.0,+1.0,-1.0);
   //  Top
   glTexCoord2f(0.0,0.0); glVertex3f(-1.0,+1.0,+1.0);
   glTexCoord2f(1.0,0.0);glVertex3f(+1.0,+1.0,+1.0);
   glTexCoord2f(1.0,1.0);glVertex3f(+1.0,+1.0,-1.0);
   glTexCoord2f(0.0,1.0);glVertex3f(-1.0,+1.0,-1.0);   
   
   //  Bottom
   glVertex3f(-1.0,-1.0,-1.0);
   glVertex3f(+1.0,-1.0,-1.0);
   glVertex3f(+1.0,-1.0,+1.0);
   glVertex3f(-1.0,-1.0,+1.0);
   //  End
   glEnd();
   glDisable(GL_TEXTURE_2D);

   //  Undo transofrmations
   glPopMatrix();


}

static void ball(double x, double y,double z, double r,double cx,double cy,double cz,int th1,int th2, int thx,int thy, int thz,int ang)
{
   int th,ph;
   
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(2*r,2*r,2*r);
   glRotated(ang,thx,thy,thz);
   //  White ball
   glColor3f(cx,cy,cz);
   //  Latitude bands
   for (ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=th1;th<=th2;th+=5)
      {
         Vertex(th,ph);
         Vertex(th,ph+5);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
   
}



void makeCylinder(float height, float base)
{
   GLUquadric *obj = gluNewQuadric();
   //gluQuadricDrawStyle(obj, GLU_LINE);
   glPushMatrix();
   glColor3f(0.845, 0.271, 0.075);
   glRotatef(-90, 1.0,0.0,0.0);
   gluCylinder(obj, base,base-(0.2*base), height, 20,20);
   glPopMatrix();
   glutSwapBuffers();
}

 
void Drawleaf1(double th,int x,int y,double dx, double dy,double dz) 
{ 
   glPushMatrix();
   glTranslated(dx,dy,dz);
   glScaled(1.5,1.5,1.5);
   glRotated(th,1,0,0);
   glColor3f(0.1,1,0);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,leaf[0]);
        glBegin(GL_TRIANGLES); 
          glNormal3f(-0.371390676354, 0.0 ,0.928476690885); 
          glTexCoord2f(0.5,0);  glVertex3f(0, 0, 0); 
          glTexCoord2f(1,0.5); glVertex3f(0.25, 0.25, 0.1); 
          glTexCoord2f(0.5,1); glVertex3f(0, 0.5, 0); 
          
          glColor3f(x-0.5,y,0);

          glNormal3f(0.371390676354, -0.0, 0.928476690885); 
          glTexCoord2f(0.5,0);glVertex3f(0, 0, 0); 
          glTexCoord2f(0.5,1);glVertex3f(0, 0.5, 0); 
          glTexCoord2f(0,0.5);glVertex3f(-0.25, 0.25, 0.1); 
        glEnd(); 
        glDisable(GL_TEXTURE_2D);
   glPopMatrix();
} 

void Drawleaf2(double th,int x,int y,double dx, double dy,double dz) 
{ 
   glPushMatrix();
   glTranslated(dx,dy,dz);
   glScaled(1.5,1.5,1.5);
   glRotated(th,1,0,0);
   glColor3f(0, 1, 0.000);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,leaf[0]);
        glBegin(GL_TRIANGLES); 
          glNormal3f(0.371390676354 ,-0.0, 0.928476690885); 
          glTexCoord2f(0.5,0);  glVertex3f(0, 0, 0); 
          glTexCoord2f(1,0.5); glVertex3f(0.25, 0.25, -0.1); 
          glTexCoord2f(0.5,1); glVertex3f(0, 0.5, 0); 

          glNormal3f(0.371390676354 ,-0.0, -0.928476690885); 
          glTexCoord2f(0.5,0);glVertex3f(0, 0, 0); 
          glTexCoord2f(0.5,1);glVertex3f(0, 0.5, 0); 
          glTexCoord2f(0,0.5);glVertex3f(-0.25, 0.25, -0.1); 
        glEnd(); 
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}
void yellowleaf(double th,int x,int y,double dx, double dy,double dz) 
{ 
   int t;
   glPushMatrix();
   t=rand()%3;

   if(wind)
   {
      if(t==1)
         glTranslated(dx,dy+Cos(zh)+1,dz);
      else if(t==0)
         glTranslated(dx+Cos(zh),dy,dz);
      else if(t==2)
         glTranslated(dx,dy,dz+Cos(zh));
      glRotated(th+incr,1,1,1);




   }
   else
   {
      glTranslated(dx,dy,dz);
      glRotated(th,0,0,0);

   }

   glScaled(1.5,1.5,1.5);
   glColor3f(1,1,0);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,leaf[1]);
        glBegin(GL_TRIANGLES); 
          glNormal3f(-0.1, 0, 0.25); 
          glTexCoord2f(0.5,0);  glVertex3f(0, 0, 0); 
          glTexCoord2f(1,0.5); glVertex3f(0.25, 0.25, -0.1); 
          glTexCoord2f(0.5,1); glVertex3f(0, 0.5, 0); 

          glNormal3f(0.1, 0, 0.25); 
          glTexCoord2f(0.5,0);glVertex3f(0, 0, 0); 
          glTexCoord2f(0.5,1);glVertex3f(0, 0.5, 0); 
          glTexCoord2f(0,0.5);glVertex3f(-0.25, 0.25, -0.1); 
        glEnd(); 
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}  


void makeTree(float height, float base)
{


float angle;
int temp;
glPushMatrix();

makeCylinder(height, base);
glEnable(GL_TEXTURE_2D);

glTranslatef(0.0, height, 0.0);
height -= height*.2; base-= base*0.3;
for(int a= 0; a<3; a++)
{
   angle = rand()%60+20;
   if(angle >48)
      angle = -(rand()%50+20);
   if (height >0.5)
   {
      glPushMatrix();
      glRotatef(angle,1,1.0,1);
      makeTree(height,base);
      glPopMatrix();
   }
   else 
   {  
      temp=rand()%3;
      if (temp==1){
         glPushMatrix();
         glTranslated(0,height-0.6,0);
         glScaled(1.5,1.5,1.5);
         glRotated(0,1,0,0);
         glColor3f(0.1,1,0);
              glBegin(GL_TRIANGLES); 
          glNormal3f(-0.37,0.0,0.92); 
                glTexCoord2f(0.5,0);  glVertex3f(0, 0, 0); 
                glTexCoord2f(1,0.5); glVertex3f(0.25, 0.25, 0.1); 
                glTexCoord2f(0.5,1); glVertex3f(0, 0.5, 0); 
          glNormal3f(0.37,0.0,0.92); 
                glTexCoord2f(0.5,0);glVertex3f(0, 0, 0); 
                glTexCoord2f(0.5,1);glVertex3f(0, 0.5, 0); 
                glTexCoord2f(0,0.5);glVertex3f(-0.25, 0.25, 0.1); 
              glEnd(); 
         glPopMatrix();
} 
      else if (temp==0){
         glPushMatrix();
            glTranslated(0,height-0.6,0);
            glScaled(1.5,1.5,1.5);
            glRotated(0,1,0,0);
            glColor3f(0, 1, 0.000);
            glBegin(GL_TRIANGLES); 
          glNormal3f(0.37,-0.0,0.92); 
                   glTexCoord2f(0.5,0);  glVertex3f(0, 0, 0); 
                   glTexCoord2f(1,0.5); glVertex3f(0.25, 0.25, -0.1); 
                   glTexCoord2f(0.5,1); glVertex3f(0, 0.5, 0); 

          glNormal3f(0.37,0.0,-0.92); 
                   glTexCoord2f(0.5,0);glVertex3f(0, 0, 0); 
                   glTexCoord2f(0.5,1);glVertex3f(0, 0.5, 0); 
                   glTexCoord2f(0,0.5);glVertex3f(-0.25, 0.25, -0.1); 
            glEnd(); 
         glPopMatrix();}
      


   }
}
glPopMatrix();
}
void init(void)
{ 

   glShadeModel(GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);
   makeaTree=glGenLists(1);
   glNewList(makeaTree, GL_COMPILE);
   glEnable(GL_TEXTURE_2D);

   glPushMatrix();
   glTranslated(8,-sky_dim,4);
   makeTree(1,0.08);
   glPopMatrix(); 
   glPushMatrix();
   glTranslated(1.5,-sky_dim,1.7);
   makeTree(1,0.2);
   glPopMatrix();  
   glPushMatrix();
   glTranslated(3.4,-sky_dim,4.1);
   makeTree(1,0.08);
   glPopMatrix();  
   glPushMatrix();
   glTranslated(7,-sky_dim,6.5);
   makeTree(1,0.08);
   glPopMatrix(); 
   glPushMatrix();
   glTranslated(2,-sky_dim,4.5);
   makeTree(1,0.08);
   glPopMatrix();    
   glPushMatrix();
   glTranslated(6.3,-sky_dim,1.5);
   makeTree(1,0.08);
   glPopMatrix();
   glPushMatrix();
   glTranslated(7.3,-sky_dim,6.5);
   makeTree(1,0.08);
   glPopMatrix();glPushMatrix();
   glTranslated(2,-sky_dim,4.5);
   makeTree(1,0.08);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);    
   glEndList(); 
}


static void smallbug(double x, double y,double z, double r,double th)
{
   // float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   glTranslated(x,y+incry,z);
   glRotated(th,0,1,0);
   glColor3f(0.000, 1.000, 1.000);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny+2);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   
   ball(x,y,z,r,0.000, 1.000, 1.000,-90,90,1,0,0,90);
   
   // glRotated(90,0,1,0);
      ball(x,y+0.15,z,r/2,0.1, 0.1, 0.10,0,360,0,0,0,0);
      ball(x,y+0.1,z+0.1,r/4,0.1, 0.1, 0.10,0,360,0,0,0,0);
      ball(x,y,z+0.14,r/4,0.1, .1, 0.10,0,360,0,0,0,0);
      ball(x-0.1,y,z+0.14,r/4,0.1, .1, 0.10,0,360,0,0,0,0);
      ball(x+0.1,y,z+0.14,r/4,0.1, .1, 0.10,0,360,0,0,0,0);

      ball(x,y-0.1,z+0.13,r/4,0.1, .1, 0.10,0,360,0,0,0,0);

   glPopMatrix();
}

static void bug(double x, double y,double z, double r,double th)
{
   // float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glColor3f(1,0,0);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny+2);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   ball(x,y,z,r,1,0,0,-90,90,0,0,0,0);
   // glRotated(90,0,1,0);
      ball(x+0.29,y+0.15,z,r/2,0.01, 0.01, 0.010,0,360,0,0,0,0);
      ball(x+0.45,y+0.15,z-0.07,r/15,1, 1, 1,0,360,0,0,0,0);
      ball(x+0.45,y+0.15,z+0.04,r/15,1, 1, 1,0,360,0,0,0,0);

      ball(x+0.1,y+0.255,z+0.04,r/4,0.01, 0.01, .01,0,360,0,0,0,0);
      ball(x-0.2,y+0.22,z+0.01,r/4,0.01, 0.01, .01,0,360,0,0,0,0);

      ball(x-0.1,y+0.26,z+0.04,r/4,0.01, 0.01, .01,0,360,0,0,0,0);
      ball(x+0.1,y+0.2,z+0.2,r/4,0.01, 0.01, .01,0,360,0,0,0,0);
      ball(x,y+0.2,z-0.2,r/4,0.01, 0.01, .01,0,360,0,0,0,0);


      stem(x*1.035,y,z*1.07,0.1,0.1,0.1,0.03,0.2,65,0,0,0);
      stem(x,y,z*1.09,0.1,0.1,0.1,0.03,0.2,65,0,0,0);
      stem(x*0.965,y,z*1.07,0.1,0.1,0.1,0.03,0.2,65,0,0,0);

      stem(x*1.035,y,z*0.93,0.1,0.1,0.1,0.03,0.2,-65,0,0,0);
      stem(x,y,z*0.91,0.1,0.1,0.1,0.03,0.2,-65,0,0,0);
      stem(x*0.965,y,z*0.93,0.1,0.1,0.1,0.03,0.2,-65,0,0,0);
   //  Undo transofrmations
   glPopMatrix();
}

void display()

{

    
   const double len=1.8;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position

   switch(mode)
   {

    case 0: //Overhead Perspective
      Ex = -2*dim*Sin(th)*Cos(ph);
      Ey = +2*dim        *Sin(ph);
      Ez = +2*dim*Cos(th)*Cos(ph);

      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
      break;

    case 1: // First Person Perspective
      Ex = -2*dim*Sin(th)*Cos(ph);
      Ey = +2*dim        *Sin(ph);
      Ez = +2*dim*Cos(th)*Cos(ph);

      gluLookAt(  5,-2.000000,z+8, x+lx, 0.8f,  z+lz, 0.0f, 1.0f,  0.0f);
      break;
   }

   if (light)
      {
               printf("Light\n");

           //  Translate intensity to color vectors
           float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
           float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
           float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
           //  Light position
           float Position[]  = {distance*Cos(zh)+4,ylight,distance*Sin(zh)+4,1.2+5};
           //  Draw light position as ball (still no lighting here)
           glColor3f(1,1,1);
           light_ball(Position[0],Position[1],Position[2] , 0.3);
           //  OpenGL should normalize normal vectors
           glEnable(GL_NORMALIZE);
           //  Enable lighting
           glEnable(GL_LIGHTING);
           //  Location of viewer for specular calculations
           glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
           //  glColor sets ambient and diffuse color materials
           glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
           glEnable(GL_COLOR_MATERIAL);
           //  Enable light 0
           glEnable(GL_LIGHT0);
           //  Set ambient, diffuse, specular components and position of light 0
           glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
           glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
           glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
           glLightfv(GL_LIGHT0,GL_POSITION,Position);
      }
      else
        {
         glDisable(GL_LIGHTING);
      }
   if (fog)
  {
   glEnable (GL_DEPTH_TEST); //enable the depth testing

   glEnable (GL_FOG); //enable the fog

   glFogi (GL_FOG_MODE, GL_EXP2); //set the fog mode to GL_EXP2

   glFogfv (GL_FOG_COLOR, fogColor); //set the fog color to
// our color chosen above

   glFogf (GL_FOG_DENSITY, density); //set the density to the
// value above

   glHint (GL_FOG_HINT, GL_NICEST); // set the fog to look the
  } 

   
   glEnable(GL_CULL_FACE);
   glColor3f(1,1,1);
    Sky(sky_dim);
   glDisable(GL_CULL_FACE);  

   ground(sky_dim,-sky_dim,sky_dim , sky_dim,0.04,sky_dim , 0);
   glPushMatrix();
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,leaf[0]);
      glCallList(makeaTree);
      glDisable(GL_TEXTURE_2D);
   glPopMatrix();

   for (int i = 0; i < 10; i+=2)
   {
      for (int j = 0; j < 10; j+=2)
      {
         Drawleaf2(40,1,0,i+1,-sky_dim,j+1);
         ball(i+1,-sky_dim+0.35,j+1, 0.1,1.000, 0.078, 0.576,0,360,0,0,0,0);
         Drawleaf1(-40,1,1,i+1,-sky_dim,j+1);      
      }    
   }  
    for (int i = 0; i < 5; ++i)
      for (int j = 0; j <3; ++j)
      {
         yellowleaf(50,1,0,pos[i][0],-sky_dim,pos[j][2]);    
      }

   bug(bug1[0],bug1[1],bug1[2],0.17,0);
   bug(bug2[0],bug2[1],bug2[2],0.17,45);
   bug(bug2[0],bug2[1],bug2[2],0.17,15);
   smallbug(small[0],small[1],small[2],0.08,0);




         

        
        

         
    // White
   glColor3f(1,1,1);
   //  Draw axes
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(20,20);
  Print("Settlers of Catan");
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode==1?"First Person Pespective":"Overhead Perspective");
   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}





/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
  float fraction=0.02f;
   //  Right arrow key - increase angle by 5 degrees
  if (key == GLUT_KEY_RIGHT)
  { 
    th+=5;
    angle += 0.1f;
    lx = sin(angle);
    lz = -cos(angle);
  }
   //  Left arrow key - decrease angle by 5 degrees
  else if (key == GLUT_KEY_LEFT)
   {
      angle -= 0.1f;
      lx = sin(angle);
      lz = -cos(angle);
      th-=5;

   }
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
   {
    x += lx * fraction;
    printf("x=%f\n", x);
      z += lz * fraction;
      ph+=5;

   }
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
   {

      x -= lx * fraction;
      z -= lz * fraction;
      ph-=5;
   }
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   Project(fov,asp,dim);

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{

   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
   {
      th = 0;
      ph = 30;
    } 
   //  Toggle axes
    
   else if (ch == 'm')
   {
      mode = (mode+1)%2;
      printf("%d\n",mode);

   }
  //  Change field of view angle

   else if (ch=='a')
   {
        fov++;

   }
   else if (ch=='s')
    {
      fov--;
   }
  // Toggle axes
   else if (ch=='z' || ch=='Z')
      {
         axes=1-axes;
      }
   else if (ch == 'f' || ch == 'F')
   {
      fog=1-fog;
   }
   //  Switch display mode
   else if (ch == 'w')
   {
      wind=1-wind;   
   }
   else if (ch=='j' || ch=='J')
   {
      incry+=0.05;
   }
   else if (ch=='k' || ch=='k')
   {
      incry-=0.05;
   }
   else if (ch=='o' || ch=='O')
   {
      ylight+=0.5;
   }
   else if (ch=='p' || ch=='p')
   {
      ylight-=0.5;
   }

   Project(fov,asp,dim);


   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
      Project(fov,asp,dim);

   //  Set projection
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Palying Catan");
   init();
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
      ground1[0] = LoadTexBMP("gr1.bmp");
      sky[0]=LoadTexBMP("sk3.bmp");
      leaf[0]=LoadTexBMP("leaf1.bmp");
      leaf[1]=LoadTexBMP("yleaf.bmp");

   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
