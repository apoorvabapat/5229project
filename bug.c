/*
 *  Cockpit
 *
 *  Demonstrates how to draw a cockpit view for a 3D scene.
 *
 *  Key bindings:
 *  m          Toggle cockpit mode
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
int axes=1;       //  Display axes
int mode=0;       //  Display cockpit
int th=0;         //  Azimuth of view angle
int ph=30;         //  Elevation of view angle
int one       =   1;  // Unit value
int distance  =   20;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
int light=1;      //  Lighting


int fov=25;       //  Field of view
double asp=1;     //  Aspect ratio
double dim=15.0;   //  Size of world
int sky_dim=5;

double pos[5][3]={
	{3,-4.2,2.8},
{4,-4.2,1.2},
{1,-4.2,3.2},
{8,-4.2,4.2},
{2.8,-4.2,7.4}
};



//Textures

int ground1[2];
int leaf[5];
int sky[2];

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
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
static void ball1(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r/2,r);
   //  White ball
   glColor3f(0.133, 0.545, 0.133);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}


static void stem(double x,double y,double z,double r,double d,double th)
{
	   int i,k;
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   // glScalef(1.4, 0.7, 1.0);
   glColor3f(0.3,0.8,0);
   glRotated(th,0,0,0);
   glScaled(r,r,d);
   //  Head & Tail
   for (i=1;i>=-1;i-=2)
   {
      // glBindTexture(GL_TEXTURE_2D,i>0?tail:head);
      glNormal3f(0,0,i);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,i);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
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


static void ball(double x, double y,double z, double r,double cx,double cy,double cz)
{
   int th,ph;
   
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(cx,cy,cz);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   
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

static void bug_shell(double x, double y,double z, double r)
{
int th,ph;
   // float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,0,0);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   // glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   // glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=-90;th<=90;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
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
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
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

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,ground1[0]);

   glBegin(GL_QUADS);

   glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);


   glPopMatrix();

}


static void ground(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

   //  Save transformation
   glPushMatrix();
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

static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-1.0,-1.0, 1.0);
   glVertex3f(+1.0,-1.0, 1.0);
   glVertex3f(+1.0,+1.0, 1.0);
   glVertex3f(-1.0,+1.0, 1.0);
   //  Back
   glColor3f(0,0,1);
   glVertex3f(+1.0,-1.0,-1.0);
   glVertex3f(-1.0,-1.0,-1.0);
   glVertex3f(-1.0,+1.0,-1.0);
   glVertex3f(+1.0,+1.0,-1.0);
   //  Right
   glColor3f(1,1,0);
   glVertex3f(+1.0,-1.0,+1.0);
   glVertex3f(+1.0,-1.0,-1.0);
   glVertex3f(+1.0,+1.0,-1.0);
   glVertex3f(+1.0,+1.0,+1.0);
   //  Left
   glColor3f(0,1,0);
   glVertex3f(-1.0,-1.0,-1.0);
   glVertex3f(-1.0,-1.0,+1.0);
   glVertex3f(-1.0,+1.0,+1.0);
   glVertex3f(-1.0,+1.0,-1.0);
   //  Top
   glColor3f(0,1,1);
   glVertex3f(-1.0,+1.0,+1.0);
   glVertex3f(+1.0,+1.0,+1.0);
   glVertex3f(+1.0,+1.0,-1.0);
   glVertex3f(-1.0,+1.0,-1.0);
   //  Bottom
   glColor3f(1,0,1);
   glVertex3f(-1.0,-1.0,-1.0);
   glVertex3f(+1.0,-1.0,-1.0);
   glVertex3f(+1.0,-1.0,+1.0);
   glVertex3f(-1.0,-1.0,+1.0);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}

void Drawleaf(double th,int x,int y,double dx, double dy,double dz) 
{ 
   glPushMatrix();
	glTranslated(dx,dy,dz);
   glScaled(2,2,2);
   glRotated(th,1,-1,1);
   glColor3f(x,y,0);
        glBegin(GL_TRIANGLES); 
          glNormal3f(-0.1, 0, 0.25); 
          glVertex3f(0, 0, 0); 
          glVertex3f(0.25, 0.25, 0.1); 
          glVertex3f(0, 0.5, 0); 
       glColor3f(x-0.5,y,0);

          glNormal3f(0.1, 0, 0.25); 
          glVertex3f(0, 0, 0); 
          glVertex3f(0, 0.5, 0); 
          glVertex3f(-0.25, 0.25, 0.1); 
        glEnd(); 
   glPopMatrix();
} 


/*
 *  Draw the cockpit as an overlay
 *  Must be called last
 */


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=1.5;  //  Length of axes
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   gluLookAt(Ex,Ey,Ez , sky_dim,0,sky_dim , 0,Cos(ph),0);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.2};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        light_ball(Position[0],Position[1],Position[2] , 0.1);
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
     glDisable(GL_LIGHTING);
   //  Draw cubes
   
    ground(sky_dim,-sky_dim,sky_dim , sky_dim,0.04,sky_dim , 0);
   
   glEnable(GL_CULL_FACE);
        glColor3f(1,1,1);

    Sky(sky_dim);
    glDisable(GL_CULL_FACE);    
    stem(4,-4.2,2,0.1,0.8,90);
        ball1(4,-3.2,2,0.5);

    stem(1.1,-4.2,3.2,0.1,0.8,90);
        ball1(1.1,-3.2,3.2,0.5);

    stem(7.5,-4.2,1,0.1,0.8,90);
        ball1(7.5,-3.2,1,0.5);

    stem(7.5,-4.2,1,0.1,0.8,90);
        ball1(7.5,-3.2,1,0.5);

    stem(4.5,-4.2,7,0.1,0.8,90);
        ball1(4.5,-3.2,7,0.5);

    stem(4.5,-4.2,4.7,0.1,0.8,90);    
    ball1(4.5,-3.2,4.7,0.5);

    bug_shell(4.5,-5,3,0.6);
    ball(5.2,-4.8,3,0.3,0.184, 0.310, 0.310);
    ball(5.5,-4.75,3,0.05,1, 1, 1);
    ball(5.5,-4.75,3.2,0.05,1, 1, 1);

    //     Drawleaf(315);



   glColor3f(1,1,1);
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
   //  Disable Z-buffering in OpenGL
   glDisable(GL_DEPTH_TEST);
   //  Draw cockpit
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d",th,ph,dim,fov);
   
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
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
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Reproject
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
      
   
   //  Tell GLUT it is necessary to redisplay the scene
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
   //  Set projection
   Project(fov,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Cockpit");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   ground1[0] = LoadTexBMP("ground.bmp");
   leaf[0]=LoadTexBMP("leaf0.bmp");
   sky[0]=LoadTexBMP("night.bmp");


   //  Load cockpit
   // LoadTexBMP("737737.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
