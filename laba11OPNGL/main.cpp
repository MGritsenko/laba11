#include <GL/glut.h>
#include <SOIL2.h>
#include <cmath>
#include "NASCARlo.h"
#include <utility>

int w = 0, h = 0;
GLfloat xrotate, yrotate, zrotate;
bool front_lights = false, back_lights = false, lamp_lights = false;

NASCARlo stockCar;

GLuint texture_road, texture_car, texture_up, texture_front, texture_side, texture_wheel;

void make_textures()
{
	texture_road = SOIL_load_OGL_texture("road.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA);
	texture_car = SOIL_load_OGL_texture("car.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA);
	texture_front = SOIL_load_OGL_texture("front.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA);
	texture_up = SOIL_load_OGL_texture("up.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA);
	texture_side = SOIL_load_OGL_texture("side.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA);
	texture_wheel = SOIL_load_OGL_texture("wheel.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA);
}


void Init(void)
{
	glClearColor(0, 0, 0, 1.0f);
	make_textures();
	
	glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_NORMALIZE);
}

void Reshape(int x, int y)
{
	if (y == 0 || x == 0) return;

	w = x;
	h = y;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (GLdouble)w / h, 0.1, 50);

}

void draw_wheel(GLfloat x, GLfloat y, GLfloat z, int size, GLfloat of_x, GLfloat of_y, GLfloat of_z) 
{
	glPushMatrix();
	glTranslatef(size*(x + of_x), size*(y + of_y), size*(z + of_z));
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glBindTexture(GL_TEXTURE_2D, texture_wheel);
	glutSolidTorus(size * 0.03, size * 0.04, size * 10, size * 10);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


void draw_light(GLfloat x, GLfloat y, GLfloat z, int size, GLfloat of_x, GLfloat of_y, GLfloat of_z, GLenum num_light, bool dir)
{
	GLfloat px = size * (x + of_x), py = size * (y + of_y), pz = size * (z + of_z);
	glPushMatrix();
	glTranslatef(px, py, pz);
	GLfloat light_diffuse[4];
	GLfloat light_position[4];
	GLfloat light_spot_direction[3];
	GLfloat color_light[4];

	if (dir)
	{
		light_diffuse[0] = 1;
		light_diffuse[1] = 1;
		light_diffuse[2] = 1;
		light_diffuse[3] = 1;
		light_position[0] = 0.01;
		light_position[1] = 0;
		light_position[2] = 0;
		light_position[3] = 1;
		light_spot_direction[0] = 1;
		light_spot_direction[1] = 0;
		light_spot_direction[2] = 0;
		color_light[0] = 1;
		color_light[1] = 1;
		color_light[2] = 1;
		color_light[3] = 1;
		glLightf(num_light, GL_LINEAR_ATTENUATION, 0.0);
		glMaterialfv(GL_FRONT, GL_EMISSION, color_light);
	}
	else
	{
		light_diffuse[0] = 1;
		light_diffuse[1] = 0;
		light_diffuse[2] = 0;
		light_diffuse[3] = 1;
		light_position[0] = -0.01;
		light_position[1] = 0;
		light_position[2] = 0;
		light_position[3] = 1;
		light_spot_direction[0] = -1;
		light_spot_direction[1] = 0;
		light_spot_direction[2] = 0;
		color_light[0] = 1;
		color_light[1] = 0;
		color_light[2] = 0;
		color_light[3] = 1;
		glLightf(num_light, GL_LINEAR_ATTENUATION, 0.5);
		glMaterialfv(GL_FRONT, GL_EMISSION, color_light);
	}

	GLfloat light_ambient[] = { 0, 0, 0, 1 };

	glLightfv(num_light, GL_AMBIENT, light_ambient);
	glLightfv(num_light, GL_DIFFUSE, light_diffuse);
	glLightfv(num_light, GL_POSITION, light_position);
	glLightfv(num_light, GL_SPECULAR, light_diffuse);
	glLightf(num_light, GL_SPOT_CUTOFF, 32);
	glLightf(num_light, GL_SPOT_EXPONENT, 1);
	glLightfv(num_light, GL_SPOT_DIRECTION, light_spot_direction);
	glLightf(num_light, GL_CONSTANT_ATTENUATION, 0.7);

	glColor4fv(color_light);
	glutSolidSphere(size * 0.016, size * 10, size * 10);
	glMaterialfv(GL_FRONT, GL_EMISSION, light_ambient);
	glPopMatrix();
}

void texturing_car(GLfloat size)
{
	glBindTexture(GL_TEXTURE_2D, texture_car);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	// Front 
	glNormal3f(0, 0, 1); glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);  // Bottom Left
	glNormal3f(0, 0, 1); glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, size);  // Bottom Right 
	glNormal3f(0, 0, 1); glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);  // Top Right 
	glNormal3f(0, 0, 1); glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);  // Top Left 

	// Back 
	glNormal3f(0, 0, -1); glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);  // Bottom Right 
	glNormal3f(0, 0, -1); glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, -size);  // Top Right
	glNormal3f(0, 0, -1); glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, -size);  // Top Left 
	glNormal3f(0, 0, -1); glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);  // Bottom Left

	 // Top 
	glNormal3f(0, 1, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);  // Top Left 
	glNormal3f(0, 1, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size);  // Bottom Left
	glNormal3f(0, 1, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);  // Bottom Right 
	glNormal3f(0, 1, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);  // Top Right 

	 // Bottom 
	glNormal3f(0, -1, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);  // Top Right 
	glNormal3f(0, -1, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);  // Top Left 
	glNormal3f(0, -1, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);  // Bottom Left 
	glNormal3f(0, -1, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);  // Bottom Right 

	// Right 
	glNormal3f(1, 0, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, -size);  // Bottom Right 
	glNormal3f(1, 0, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);  // Top Right
	glNormal3f(1, 0, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, size);  // Top Left 
	glNormal3f(1, 0, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);  // Bottom Left  

	// Left 
	glNormal3f(-1, 0, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);  // Bottom Left 
	glNormal3f(-1, 0, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);  // Bottom Right 
	glNormal3f(-1, 0, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);  // Top Right 
	glNormal3f(-1, 0, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);  // Top Left 

	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void texturing_front(GLfloat size)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_side);
	GLfloat light_position[4]{ 0.3,0.3,0.3,1 };
	glMaterialfv(GL_FRONT, GL_EMISSION, light_position);
	glBegin(GL_QUADS);

	// Front 
	glNormal3f(0, 0, 1); glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, size);  // Bottom Left
	glNormal3f(0, 0, 1); glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, size);  // Bottom Right 
	glNormal3f(0, 0, 1); glTexCoord2f(0.0f, 0.0f); glVertex3f(size, size, size);  // Top Right 
	glNormal3f(0, 0, 1); glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, size, size);  // Top Left 

	// Back 
	glNormal3f(0, 0, -1); glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);  // Bottom Right 
	glNormal3f(0, 0, -1); glTexCoord2f(1.0f, 0.0f);  glVertex3f(-size, size, -size);  // Top Right
	glNormal3f(0, 0, -1); glTexCoord2f(0.0f, 0.0f); glVertex3f(size, size, -size);  // Top Left 
	glNormal3f(0, 0, -1); glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);  // Bottom Left

	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture_up);
	glBegin(GL_QUADS);
	// Top 
	glNormal3f(0, 1, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);  // Top Left 
	glNormal3f(0, 1, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size);  // Bottom Left
	glNormal3f(0, 1, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);  // Bottom Right 
	glNormal3f(0, 1, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);  // Top Right 

	 // Bottom 
	glNormal3f(0, -1, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);  // Top Right 
	glNormal3f(0, -1, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);  // Top Left 
	glNormal3f(0, -1, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);  // Bottom Left 
	glNormal3f(0, -1, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);  // Bottom Right 

	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture_front);
	glBegin(GL_QUADS);
	// Right 
	glNormal3f(1, 0, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);  // Bottom Right 
	glNormal3f(1, 0, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(size, size, -size);  // Top Right
	glNormal3f(1, 0, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);  // Top Left 
	glNormal3f(1, 0, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(size, -size, size);  // Bottom Left  

	// Left 
	glNormal3f(-1, 0, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);  // Bottom Left 
	glNormal3f(-1, 0, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);  // Bottom Right 
	glNormal3f(-1, 0, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);  // Top Right 
	glNormal3f(-1, 0, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);  // Top Left 

	glEnd();

	glMaterialfv(GL_FRONT, GL_EMISSION, light_position);
	glDisable(GL_TEXTURE_2D);
}


void draw_car(GLfloat x, GLfloat y, GLfloat z, GLdouble turn, int size) 
{
	glPushMatrix();

	glTranslatef(x, y, z + size / 4.3);
	glRotatef(turn, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);

	texturing_car(size * 0.15f);

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(size * (+0.25), size * (-0.05), 0);
	texturing_front(size*0.1);
	glPopMatrix();

	glColor3f(0.0f, 0.0f, 0.0f);
	draw_wheel(0, 0, 0, size, 0.25, -0.15, 0.07);
	draw_wheel(0, 0, 0, size, 0.25, -0.15, -0.07);
	draw_wheel(0, 0, 0, size, -0.08, -0.15, -0.07);
	draw_wheel(0, 0, 0, size, -0.08, -0.15, 0.07);

	{
		GLfloat light_ambient[] = { 0.05, 0.05, 0.05, 1 };
		GLfloat light_specular[] = { 0.1, 0.1, 0.1, 1 };
		GLfloat light_spot_direction[] = { 0, 0, -1 , 1 };
		GLfloat light2_diffuse[] = { 0.5, 0.5, 0.5 };
		GLfloat light2_position[] = { 0.0, 0.5, 1.0 };
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light2_diffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, light2_position);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0);
		glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.3);

		glEnable(GL_LIGHT2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
		glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0);
		glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0);
		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.3);
	}

	glColor3f(1.0f, 1.0f, 0.0f);
	draw_light(0, 0, 0, size, 0.35, -0.1, -0.07, GL_LIGHT1, true);
	draw_light(0, 0, 0, size, 0.35, -0.1, 0.07, GL_LIGHT2, true);
	draw_light(0, 0, 0, size, -0.15, -0.1, -0.09, GL_LIGHT3, false);
	draw_light(0, 0, 0, size, -0.15, -0.1, 0.09, GL_LIGHT4, false);
	glPopMatrix();
}

void draw_lamp(GLfloat x, GLfloat y, GLfloat z, int size, GLfloat angle, GLenum num_light)
{
	glPushMatrix();
	glTranslatef(x, y, z + (size * 0.2 + size * 0.05 + size * 0.08));
	glRotatef(angle, 0.0, 0.0, 1);
	glRotatef(90, 1, 0, 0);

	glColor3f(0.0f, 0.0f, 0.0f);
	GLUquadricObj *quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);

	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(quadObj, size * 0.01, size * 0.01, size * 0.2, size * 15, size * 15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(size * (0.00), size * (0.03), 0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(0, 0.0, 1.0, 0.0);
	gluCylinder(quadObj, size * 0.01, size * 0.01, size * 0.05, size * 15, size * 15);
	glPopMatrix();

	glColor3f(255.0f, 255.0f, 255.0f);
	glPushMatrix();
	glTranslatef(size * (0.04), size * (0.022), 0);
	glRotatef(-90, 0.0, 1.0, 0.0);

	GLfloat light_diffuse[] = { 0.2, 0.2, 0.2, 1 };
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };

	glLightfv(num_light, GL_DIFFUSE, light_diffuse);
	glLightfv(num_light, GL_POSITION, light_position);
	glLightf(num_light, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(num_light, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(num_light, GL_QUADRATIC_ATTENUATION, 0.2);

	GLfloat material_emission[4] = { 0.3, 0.3, 0.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);

	gluCylinder(quadObj, size * 0.01, size * 0.01, size * 0.03, size * 15, size * 15);
	material_emission[0] = 0;
	material_emission[1] = 0;
	glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);

	glPopMatrix();

	glPopMatrix();
}

void draw_ground(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1, int divx, int divy)
{
	GLfloat light_position[] = { 0.1,0.1,0.1,1 };
	GLfloat material_diffuse[] = { 0.1, 0.1, 0.1, 1 };
	GLfloat material_emission[] = { 0, 0, 0, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_position);

	glColor3f(1.0f, 1.0f, 1.0f);
	glNormal3f(0, 0, 1);

	GLdouble stepx = (x1 - x0) / divx;
	GLdouble stepy = (y1 - y0) / divy;

	GLdouble x, y;
	y = y0;
	x = x0;
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texture_road);
	float k = 0.1;
	glBegin(GL_QUADS);
	for (size_t i = 0; i < divx; i++)
	{
		y = y0;
		for (size_t j = 0; j < divy; j++)
		{
			glVertex3f(x, y, 0.0f); glTexCoord2f(0, 0);
			glVertex3f(x, y - stepy, 0.0f); glTexCoord2f(0, 1);
			glVertex3f(x - stepx, y - stepy, 0.0f); glTexCoord2f(1, 1);
			glVertex3f(x - stepx, y, 0.0f); glTexCoord2f(1, 0);
			y += stepy;
		}
		x += stepx;
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void initCmaera() 
{
	glLoadIdentity();

	GLfloat light_ambient[] = { 0.05, 0.05, 0.05, 1 };
	GLfloat light_specular[] = { 0.1, 0.1, 0.1, 1 };
	GLfloat light_spot_direction[] = { 0, 0, -1 , 1 };

	GLfloat light2_diffuse[] = { 0.5, 0.5, 0.5 };
	GLfloat light2_position[] = { 0.0, 0.5, 1.0 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.3);

	GLdouble eye[3]{0,0,0};
	GLdouble center[3]{ 0, 0, 0 };
	GLdouble up[3]{ 0, 0, 0 };
	stockCar.get_cam_settings(eye, center, up);
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

}

void Update(void)
{
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	initCmaera();
	glPushMatrix();
	draw_ground(-15, 15, -10, 10, 200, 200);
	draw_car(stockCar.x, stockCar.y, stockCar.z, stockCar.get_ang(), 1);
	draw_lamp(4, -0.5, -0.5, 3, 90, GL_LIGHT5);
	draw_lamp(6, -0.5, -0.5, 3, 90, GL_LIGHT6);
	draw_lamp(2, -0.5, -0.5, 3, 90, GL_LIGHT7);

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':	// A
		stockCar.Turn(5);
		break;
	case 'd':	// D
		stockCar.Turn(-5);
		break;
	case 'w':	// W
		stockCar.Move(0.1);
		break;
	case 's':	// S	
		stockCar.Move(-0.1);
		break;
	case 'q':
	{
		if (lamp_lights)
		{
			glDisable(GL_LIGHT0);
			glDisable(GL_LIGHT5);
			glDisable(GL_LIGHT6);
			glDisable(GL_LIGHT7);
			lamp_lights = false;
		}
		else
		{
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT5);
			glEnable(GL_LIGHT6);
			glEnable(GL_LIGHT7);
			lamp_lights = true;
		}
	}
	break;

	case 27:
		exit(0);
		break;

	default:
		break;
	}

	glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
	GLdouble xa = 0, za = 0, d = 0;
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
		if (front_lights)
		{
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT2);
			front_lights = false;
		}
		else
		{
			glEnable(GL_LIGHT1);
			glEnable(GL_LIGHT2);
			front_lights = true;
		}
		break;

	case GLUT_KEY_PAGE_DOWN:
		if (back_lights) 
		{
			glDisable(GL_LIGHT3);
			glDisable(GL_LIGHT4);
			back_lights = false;
		}
		else
		{
			glEnable(GL_LIGHT3);
			glEnable(GL_LIGHT4);
			back_lights = true;
		}
		break;
	case GLUT_KEY_UP:
		za = 1;
		break;

	case GLUT_KEY_DOWN:
		za = -1;
		break;

	case GLUT_KEY_LEFT:
		xa = -1;
		break;

	case GLUT_KEY_RIGHT:
		xa = 1;
		break;
	default:
		break;
	case GLUT_KEY_F1:
		d = 0.1;
		break;
	case GLUT_KEY_F2:
		d = -0.1;
		break;
	}

	stockCar.TurnCam(xa, za, d);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("11 LAB");
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(Update);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);

	Init();
	glutMainLoop();
	return 0;
}