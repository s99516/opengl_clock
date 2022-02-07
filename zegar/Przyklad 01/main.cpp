//niezbednde biblioteki
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define M_TWOPI   6.2831853071795862319959 /* 2*pi */

GLUquadricObj *Cylinder;
GLUquadricObj *Disk;

//czas
struct tm *newtime;
time_t ltime;

GLint rot_x, rot_y, crement;
//oswietlenie
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightPosition[]= { 5.0f, 25.0f, 15.0f, 1.0f };
//material
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

//model widoku 0=orto, 1 = manualny
static int view_mode = 0; 

void Sprint( int x, int y, char *st)
{
	int l,i;
	l=strlen( st );
	glRasterPos3i( x, y, -1);
	for( i=0; i < l; i++)
		{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
	}
}

static void TimeEvent(int te)
{
	glutPostRedisplay();
	glutTimerFunc( 100, TimeEvent, 1);
}

void initalize(void)
{
	rot_x = 0.0;
	rot_y = 0.0;
	crement = 5; // inkremantacja rotacji
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	//oswietlenie
	glLightfv(GL_LIGHT1 ,GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1 ,GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1 ,GL_POSITION, LightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);


	Cylinder = gluNewQuadric();
	gluQuadricDrawStyle( Cylinder, GLU_FILL);
	gluQuadricNormals( Cylinder, GLU_SMOOTH);
	gluQuadricOrientation( Cylinder, GLU_OUTSIDE);
	gluQuadricTexture( Cylinder, GL_TRUE);
}

void Draw_clock( GLfloat clock_x, GLfloat clock_y, GLfloat clock_z )
{
	int hour_ticks , sec_ticks;
	glPushMatrix();

	glTranslatef(clock_x, clock_y, clock_z);
	glRotatef( 180, 1.0, 0.0, 0.0);

	glPushMatrix();
	glTranslatef( 0, 0, 1.0);

	glPopMatrix();

	//rysowanie godzinowej wskazówki zegara
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);//niebieska wsakazówka
	glTranslatef( 0, 0, 0.0);
	//glRotatef( (360/12) * newtime->tm_hour  + (360/60) * (60 / (newtime->tm_min+1)), 0.0, 0.0, 1.0);//rotacja
	glRotatef((360 / 12) * newtime->tm_hour, 0.0, 0.0, 1.0);//rotacja
	glPushMatrix();
	glTranslatef(0.0, 0.0, 2.0);
	glPopMatrix();
	glRotatef( 90, 1.0, 0.0, 0.0);
	gluCylinder(Cylinder, 0.5, 0.5, 4, 15, 15);
	glPopMatrix();

	//rysowanie minutowej wskazówki zegara
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);//zielona wskazowka
	glTranslatef( 0, 0, 0.0);
	glRotatef( (360/60) * newtime->tm_min, 0.0, 0.0, 1.0);//rotacja
	glPushMatrix();
	glTranslatef(0.0, 0.0, 3.0);
	glScalef(0.5, 0.5, 1.0);
	glPopMatrix();
	glRotatef( 90, 1.0, 0.0, 0.0);
	gluCylinder(Cylinder, 0.4, 0.4, 6, 15, 15);
	glPopMatrix();

	//rysowanie sekundowej wskazówki zegara
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);//czerwona wskazowka
	glTranslatef( 0, 0, -0.0);
	glRotatef( (360/60) * newtime->tm_sec, 0.0, 0.0, 1.0);// rotacja
	glPushMatrix();
	glTranslatef(0.0, 0.0, 4.0);
	glScalef(0.25, 0.25, 1.0);
	glPopMatrix();
	glRotatef( 90, 1.0, 0.0, 0.0);
	gluCylinder(Cylinder, 0.25, 0.25, 6, 15, 15);
	glPopMatrix();


	for(hour_ticks = 0; hour_ticks < 12; hour_ticks++)
		{
		glPushMatrix();//rysownie wyznaczników dna zegarze
		glColor3f(1.0, 1.0, 1.0); //kolor dla wyznacznikow (biale pajeczynki)
		glTranslatef(0.0, 0.0, 0.0);
		glRotatef( (360/12) * hour_ticks, 0.0, 0.0, 1.0);
		glTranslatef( 7.0, 0.0, 0.0);
		glutWireSphere(.75, 12, 12);
		glPopMatrix();
	}
	glPopMatrix();
}

void display(void)
{
	time(&ltime);//pobieraie czasu
	newtime = localtime(&ltime);// czas lokalny

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-9.0, 9.0, -9.0, 9.0, 1.0, 60.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	// napis z czasem
	glColor3f( 1.0, 1.0, 1.0);
	Sprint(-3, 8, asctime(newtime));

	//przelaczanie perspektywy na manual
	if (view_mode == 1)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(60.0, 1, 1.0, 60.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0, 0.0, 40.0, 0.0, 0.0, -14.0, 0, 1, 0);
		glRotatef(rot_x, 1.0, 0.0, 0.0); // rotacja y
		glRotatef(rot_y, 0.0, 1.0, 0.0); // roatcja x
	}
	if (view_mode == 0)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(60.0, 1, 1.0, 60.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -14.0, 0, 1, 0);
		glRotatef(0.0, 1.0, 0.0, 0.0); // rotacja y
		glRotatef(0.0, 0.0, 1.0, 0.0); // roatcja x
	}
	Draw_clock( 0.0, 0.0, -14.0);
	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'o': // przelaczanei na tryb ortogonalny
			view_mode = 0;
			break;
		case 'm'://tryb manualny
			view_mode = 1;
			break;
		case 'l'://prawo
			rot_y = (rot_y - crement) % 360;
			break;
		case 'j'://lewo
			rot_y = (rot_y + crement) % 360;
			break;
		case 'i'://dol
			rot_x = (rot_x + crement) % 360;
			break;
		case 'k'://gora
			rot_x = (rot_x - crement) % 360;
			break;
		case 27:
			exit(0);//27 - klawisz esc; zamkniecie programu
			break;
		default:
			break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (10, 10);
	glutCreateWindow (argv[0]);
	glutSetWindowTitle("ZEGAR 3D");
	initalize();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(10, TimeEvent, 1);
	glutMainLoop();
	return 0;
}
