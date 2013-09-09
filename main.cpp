/*
 * OpenGLSamples (openglsamples.sf.net) Examples
 * VC++ users should create a Win32 Console project and link 
 * the program with glut32.lib, glu32.lib, opengl32.lib
 *
 * GLUT can be downloaded from http://www.xmission.com/~nate/glut.html
 * OpenGL is by default installed on your system.
 * For an installation of glut on windows for MS Visual Studio 2010 see: http://nafsadh.wordpress.com/2010/08/20/glut-in-ms-visual-studio-2010-msvs10/
 *
 */

#include <stdio.h>
#include <windows.h>	   // Standard header for MS Windows applications
#include <GL/gl.h>		   // Open Graphics Library (OpenGL) header
#include <GL/glut.h>	   // The GL Utility Toolkit (GLUT) Header
#include <math.h>

#define KEY_ESCAPE 27



typedef struct {
    int width;
	int height;
	char* title;

	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;

glutWindow win;

const float DEG2RAD = 3.14159/180;
 
void drawCircle(float radius)
{
   glBegin(GL_LINE_LOOP);
 
   for (int i=0; i < 360; i++)
   {
      float degInRad = i*DEG2RAD;
      glVertex3f(cos(degInRad)*radius,sin(degInRad)*radius, 0.0f);
   }
 
   glEnd();
}

void drawEllipse(float xradius, float yradius)
{
   glBegin(GL_LINE_LOOP);
 
   for (int i=0; i < 360; i++)
   {
      //convert degrees into radians
      float degInRad = i*DEG2RAD;
      glVertex2f(cos(degInRad)*xradius,sin(degInRad)*yradius);
   }
 
   glEnd();
}



void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-3.0f);			
 
	/*
	 * Triangle code starts here
	 * 3 verteces, 3 colors.
	 */

	glBegin(GL_LINES);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f( 0.0f, 0.0f, 0.0f);
		glVertex3f( 1.0f, 0.0f, 0.0f);
		glVertex3f( 0.0f, 1.0f, 0.0f);
		glVertex3f( 0.0f, 1.0f, 0.0f);
	glEnd();


	glBegin(GL_TRIANGLES);					
		glColor3f(0.0f,0.0f,1.0f);			
		glVertex3f( 0.0f, 1.0f, 0.0f);		
		glColor3f(0.0f,1.0f,0.0f);			
		glVertex3f(-1.0f,-1.0f, 0.0f);		
		//glColor3f(1.0f,0.0f,0.0f);			
		//glVertex3f( 1.0f,-1.0f, 0.0f);		
	glEnd();
	glColor3f(0.0f,1.0f,0.0f);
	drawCircle(1.0f);
	drawEllipse(1.0, 0.5);


	
 
	glutSwapBuffers();
}


void initialize () 
{
    glMatrixMode(GL_PROJECTION);												// select projection matrix
    glViewport(0, 0, win.width, win.height);									// set the viewport
    glMatrixMode(GL_PROJECTION);												// set matrix mode
    glLoadIdentity();															// reset projection matrix
    GLfloat aspect = (GLfloat) win.width / win.height;
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);		// set up a perspective projection matrix
    glMatrixMode(GL_MODELVIEW);													// specify which matrix is the current matrix
    glShadeModel( GL_SMOOTH );
    glClearDepth( 1.0f );														// specify the clear value for the depth buffer
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );						// specify implementation-specific hints
	glClearColor(1.0, 1.0, 1.0, 1.0);											// specify clear values for the color buffers								
}


void keyboard ( unsigned char key, int mousePositionX, int mousePositionY )		
{ 
  switch ( key ) 
  {
    case KEY_ESCAPE:        
      exit ( 0 );   
      break;      

    default:      
      break;
  }
}

int main(int argc, char **argv) 
{
	// set window values
	win.width = 640;
	win.height = 480;
	win.title = "OpenGL/GLUT Example. Visit http://openglsamples.sf.net ";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;

	// initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );  // Display Mode
	glutInitWindowSize(win.width,win.height);					// set window size
	glutCreateWindow(win.title);								// create Window
	glutDisplayFunc(display);									// register Display Function
	glutIdleFunc( display );									// register Idle Function
    glutKeyboardFunc( keyboard );								// register Keyboard Handler
	initialize();
	glutMainLoop();												// run GLUT mainloop
	return 0;
}