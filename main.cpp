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
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


#define KEY_ESCAPE 27
#define PI 3.14159265358f



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
float ellipseAngle = 0;
float ellipseXRadius = 0.4;
float ellipseYRadius = 0.15;
float earthDisplacementX = 0;
float increment = 0.003;

float accelerogram[5000];
float earthAcceleration = 0;
float earthVelocity = 0;
float earthDisplacement = 0;

float baseAcceleration = 0;
float baseVelocity = 0;
float baseDisplacement = 0;
float baseMass = 27000/45 ; 

float k = 30000 ;
float g = 9.81;

//float raport = ellipseXRadius / ellipseYRadius;


int simulationStep = 0;
int accelerogramLength = 0;

unsigned int timeInterval = 20; //ms
unsigned int previousTime = 0;
std::ifstream accelerogramFile("accelerograma.txt");
std::ofstream outputFile("rezultat.csv");
 
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
   glBegin(GL_POLYGON);
 
   for (int i=0; i < 180; i++)
   {
      //convert degrees into radians
      float degInRad = 2*i*DEG2RAD;
      glVertex2f(cos(degInRad)*xradius,sin(degInRad)*yradius);
   }
 
   glEnd();
}

void nop()
{
	return;
}


void display() 
{

	float raport = ellipseXRadius / ellipseYRadius;
	//baseMass = 5000 * 2;

	unsigned int time = glutGet(GLUT_ELAPSED_TIME);
	if (time < previousTime + timeInterval){
		return;
	}
	previousTime = time;

	float ellpsePerimeter = 2*PI*sqrtf(ellipseXRadius*ellipseYRadius);

	earthAcceleration = accelerogram[simulationStep];
	earthVelocity = earthVelocity + earthAcceleration*0.02;
	earthDisplacement = earthDisplacement + earthVelocity * 0.02 + earthAcceleration * 0.02 * 0.02 / 2;

	
	float deltaD = earthDisplacement - baseDisplacement;

	ellipseAngle = (2*PI*deltaD/ellpsePerimeter)/DEG2RAD;

	float elasticComponent = 2 * baseMass * g * sinf(ellipseAngle * DEG2RAD) * cosf(ellipseAngle * DEG2RAD) / baseMass;
	float dampingComponent = ( 10000 * (earthVelocity - baseVelocity) ) / baseMass;
	baseAcceleration = dampingComponent; //elasticComponent;

	//Fi + Ff + Fe = 0;
	//ma + cv + kd = 0;
	//a = -cv/m - kd/m;
	

	//baseAcceleration = k * deltaD / baseMass;
	baseVelocity = baseVelocity + baseAcceleration * 0.02;
	baseDisplacement = baseDisplacement + baseVelocity * 0.02 + baseAcceleration * 0.02 * 0.02 / 2;

	simulationStep++;

	earthDisplacementX = earthDisplacement - deltaD/2;

	if (simulationStep < accelerogramLength)
	{
		outputFile << simulationStep * 0.02 << "," << earthAcceleration << "," << baseAcceleration  << "," << baseVelocity  << "," << baseDisplacement << "\r";
	} 
	else 
	{
		outputFile.close();
		exit(0);
	}


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-3.0f);			

	//ellipseAngle+=90;

	//MOVEMENT EQUATION
	//earthDisplacementX += increment;
	//if (earthDisplacementX > 0.08 || earthDisplacementX < -0.08)
	//{
	//	increment = -increment;
	//}

	
	// Kepler
	//$per = 2.0*$pi*sqrt($rA*$rB);
	//$rot = -2*$pi*$x/$per;

	

	glTranslatef(0.0f, -0.9f, 0.0f);

	//earth line
	glPushMatrix();
		glTranslatef(earthDisplacement, 0.0f, 0.0f);
		glBegin(GL_LINES);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f( -1.5f, 0.0f, 0.0f);
			glVertex3f( 1.5f, 0.0f, 0.0f);
		glEnd();
	glPopMatrix();


	//ellipse
	glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		float degRad = ellipseAngle * DEG2RAD;
		float rx = sin(degRad)*ellipseXRadius;
		float ry = cos(degRad)*ellipseYRadius;
		float r = sqrtf(rx*rx+ry*ry);
		glTranslatef(earthDisplacementX-0.8, r, 0.0f);
		glRotatef(ellipseAngle, 0.0f, 0.0f, 0.1f);
		drawEllipse(ellipseXRadius, ellipseYRadius);
	glPopMatrix();

	//2nd ellipse
	glPushMatrix();
		glTranslatef(earthDisplacementX+0.8, r, 0.0f);
		glRotatef(ellipseAngle, 0.0f, 0.0f, 0.1f);
		drawEllipse(ellipseXRadius, ellipseYRadius);
	glPopMatrix();

	//Translation for entire building
	//glTranslatef(2*earthDisplacementX, 2*r, 0.0f);
	glTranslatef(baseDisplacement, 2*r, 0.0f);


	//building base
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f( -1.5f, 0.0f, 0.0f);
		glVertex3f( 1.5f, 0.0f, 0.0f);
		glVertex3f( 1.5f, 1.0f, 0.0f);
		glVertex3f( -1.5f, 1.0f, 0.0f);
	glEnd();




 
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
	win.width = 1366;
	win.height = 768;
	win.title = "Earthquake simulation";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;

		
	accelerogramLength = 0;
	string line;
	if (accelerogramFile.is_open())
	{
		while ( std::getline(accelerogramFile, line) )	
		{
			accelerogram[accelerogramLength] = atof(line.c_str());
			accelerogramLength++;
		}
		accelerogramFile.close();
	}

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