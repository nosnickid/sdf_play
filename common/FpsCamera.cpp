#include "FpsCamera.h"

FpsCamera::FpsCamera()
{
	// initialise camera at (2,2,2) looking down the -y axis
	pos[0] = 2;
	pos[1] = 2;
	pos[2] = 2;
	pitch = yaw = 0;
	viewvars[0] = 9999;
	up[0] = up[2] = 0;
	up[1] = 1;
	up[3] = 0;
	/*view[0] = -0.40;
	view[1] = -0.90;
	view[2] = 0;
	view[3] = 0;*/
}

void FpsCamera::GlMult()
{
	//GLfloat *view;
	//view = GetLookVec();
	glRotatef(90,0,1,0);
	glRotatef(-pitch, 0, 0, 1);
	glRotatef(yaw, 0, 1, 0);
	glTranslated(-pos[0], -pos[1],- pos[2]);
}

void FpsCamera::InitHax(GLfloat x, GLfloat y, GLfloat z, GLfloat pitch, GLfloat yaw) 
{
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->pitch = pitch;
	this->yaw = yaw;
}

#define FPSC_CROSS(c,a,b) c[0] = a[1]*b[2]-a[2]*b[1]; c[1] = a[2]*b[0]-a[0]*b[2]; c[2] = a[0]*b[1]-a[1]*b[0];

const GLfloat *FpsCamera::GetLookVec()
{
	if (viewvars[0] != pitch || viewvars[1] != yaw)
	{
		GLfloat s, c, norm[4], rotq[4], tmp[4];
		c = cos(3.14159f*yaw/180.0);
		s = sin(3.14159f*yaw/180.0);
		// shortcircuit all teh calculations :))
		view[0] = c;
		view[1] = 0;
		view[2] = s;
		view[3] = 0;

		// get the normal vector to our view
		FPSC_CROSS(norm,view,up);
		
		// here, we make a quaternion based on the 
		// two angles that we have set in the fps camera.
		// so - this quaternion represents rotation around the
		// axis that goes 'through the players head'
		s = (3.14159f*pitch/180)/2;
		rotq[3] = cos(s);
		s = sin(s);
		rotq[0] = norm[0] * s;
		rotq[1] = norm[1] * s;
		rotq[2] = norm[2] * s;

		mult(tmp, rotq, view);
		conjugate(rotq);
		mult(view, tmp, rotq);

		viewvars[0] = pitch;
		viewvars[1] = yaw;//*/
	}
	/*vec[0] = view[0];
	vec[1] = view[1];
	vec[2] = view[2];*/
	return view;
}

void FpsCamera::MoveOnRelXY(GLfloat fb, GLfloat lr)
{
	const GLfloat *look;
	look = this->GetLookVec();
	if (fb) 
	{
		this->pos[0] += look[0] * fb;
		this->pos[1] += look[1] * fb;
		this->pos[2] += look[2] * fb;
	}
	if (lr)
	{
		GLfloat lrv[3];
		FPSC_CROSS(lrv,look,up);
		this->pos[0] += lrv[0] * lr;
		this->pos[1] += lrv[1] * lr;
		this->pos[2] += lrv[2] * lr;

		/*this->view[0] += lrv[0] * lr;
		this->view[1] += lrv[1] * lr;
		this->view[2] += lrv[2] * lr;*/
	}
}

int first=100;

void FpsCamera::RotateBy(GLfloat x, GLfloat y)
{
	pitch -= y/10;
	yaw += x/10;
	GetLookVec();
	/*
	return;

	if (first)
	{
		first=0;
		return;
	} 

	//y=0;

	GLfloat tmp1[4], tmp2[4], tmp3[4], len;

	// /-250/2 reallu
	x /= -500; 
	y /= -500;

	tmp1[3] = cos(x);
	x = sin(x);
	tmp1[0] = up[0] * x;			
	tmp1[1] = up[1] * x;
	tmp1[2] = up[2] * x;
	len = sqrt(tmp1[0]*tmp1[0]+tmp1[1]*tmp1[1]+tmp1[2]*tmp1[2]+tmp1[3]*tmp1[3]);
	tmp1[0] /= len; tmp1[1] /= len; tmp1[2] /= len; tmp1[3] /= len;
	mult(tmp2,tmp1,view);
	conjugate(tmp1);
	mult(view,tmp2,tmp1);
	view[3] = 0;

	FPSC_CROSS(tmp3,view,up);
	tmp3[3] = 0;
	len = sqrt(tmp3[0]*tmp3[0]+tmp3[1]*tmp3[1]+tmp3[2]*tmp3[2]+tmp3[3]*tmp3[3]);
	tmp3[0] /= len; tmp3[1] /= len; tmp3[2] /= len; //tmp3[3] /= len;

	tmp1[3] = cos(y);
	y = sin(y);
	tmp1[0] = tmp3[0] * y;
	tmp1[1] = tmp3[1] * y;
	tmp1[2] = tmp3[2] * y;
	len = sqrt(tmp1[0]*tmp1[0]+tmp1[1]*tmp1[1]+tmp1[2]*tmp1[2]+tmp1[3]*tmp1[3]);
	tmp1[0] /= len; tmp1[1] /= len; tmp1[2] /= len; tmp1[3] /= len;
	mult(tmp2,tmp1,view);
	conjugate(tmp1);
	mult(view,tmp2,tmp1);
	view[3] = 0;*/
}

void FpsCamera::conjugate(GLfloat *quat)
{
	quat[0] = -quat[0];	
	quat[1] = -quat[1];
	quat[2] = -quat[2];
}

void FpsCamera::mult(GLfloat *C, GLfloat *A, GLfloat *B)
{
  C[0] = A[3]*B[0] + A[0]*B[3] + A[1]*B[2] - A[2]*B[1];
  C[1] = A[3]*B[1] - A[0]*B[2] + A[1]*B[3] + A[2]*B[0];
  C[2] = A[3]*B[2] + A[0]*B[1] - A[1]*B[0] + A[2]*B[3];
  C[3] = A[3]*B[3] - A[0]*B[0] - A[1]*B[1] - A[2]*B[2];
}


