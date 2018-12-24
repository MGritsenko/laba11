#pragma once
#include <GL/glut.h>

class NASCARlo
{
	const GLdouble pid = 3.14159265359 / 180;
	const GLdouble pi2 = 2 * 3.14159265359;
	const GLdouble pi = 3.14159265359;

	void  static cross(GLdouble a[3], GLdouble b[3], GLdouble res[3]);

	void static norm(GLdouble a[]);



public:
	bool moving_back;
	GLdouble x;
	GLdouble y;
	GLdouble z;
	GLdouble angle;
	GLdouble cam_xangle;
	GLdouble cam_zangle;
	GLdouble cam_dist;

	void get_cam_settings(GLdouble eye[], GLdouble center[], GLdouble up[]);
	GLdouble get_ang();
	void TurnCam(GLdouble ax, GLdouble az, GLdouble d);
	void Move(GLdouble d);
	void Turn(GLdouble ang);

	NASCARlo();
	~NASCARlo();
};