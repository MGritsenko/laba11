#include "NASCARlo.h"
#include <cmath>


void  NASCARlo::cross(GLdouble a[3], GLdouble b[3], GLdouble res[3]) {
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];

}

void  NASCARlo::norm(GLdouble a[]) {
	GLdouble len = std::sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	for (size_t i = 0; i < 3; i++)
		a[i] /= len;
}


void NASCARlo::get_cam_settings(GLdouble eye[], GLdouble center[], GLdouble up[])
{
	GLdouble real_angle = angle + cam_xangle;

	GLdouble fwd[3]{ -cam_dist * std::cos(real_angle)*std::sin(pi / 2 - cam_zangle),   -cam_dist * std::sin(real_angle)*std::sin(pi / 2 - cam_zangle) , cam_dist *std::cos(pi / 2 - cam_zangle) };
	GLdouble fwd2[3]{ fwd[0],fwd[1],fwd[2] };
	GLdouble tmp[3]{ std::cos(real_angle - pi / 2),std::sin(real_angle - pi / 2),0 };

	norm(fwd);
	norm(tmp);
	cross(fwd, tmp, up);

	center[0] = x;
	center[1] = y;
	center[2] = z;

	eye[0] = fwd2[0] + x;
	eye[1] = fwd2[1] + y;
	eye[2] = fwd2[2] + z;
}

GLdouble NASCARlo::get_ang()
{
	return angle / pid;
}

void NASCARlo::TurnCam(GLdouble ax, GLdouble az, GLdouble d)
{
	ax *= pid;
	cam_xangle += ax;
	if (cam_xangle < -pi2)
		cam_xangle += pi2;
	else if (cam_xangle > pi2)
		cam_xangle -= pi2;

	az *= pid;
	cam_zangle += az;
	if (cam_zangle < -pi2)
		cam_zangle += pi2;
	else if (cam_zangle > pi2)
		cam_zangle -= pi2;

	cam_dist += d;

}

void NASCARlo::Move(GLdouble d)
{
	x += std::cos(angle)*d;
	y += std::sin(angle)*d;
	moving_back = d < 0;
}

void NASCARlo::Turn(GLdouble ang)
{
	ang *= pid;
	angle += ang;
	if (angle < -pi2)
		angle += pi2;
	else if (angle > pi2)
		angle -= pi2;
}

NASCARlo::NASCARlo()
{
	x = 0;
	y = 0;
	z = 0;
	angle = 0;
	moving_back = true;

	cam_xangle = 0;
	cam_zangle = 45 * pid;
	cam_dist = 1;
}


NASCARlo::~NASCARlo()
{
}
