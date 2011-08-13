#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

class FpsCamera 
{
protected:
	void conjugate(GLfloat *quat);
	void mult(GLfloat *C, GLfloat *A, GLfloat *B);

public:
	GLfloat view[4], viewvars[2], up[4];

	GLfloat pos[3];
	GLfloat yaw;
	GLfloat pitch;

	FpsCamera();
	
	/**
	 * Receives raw mouse input, rotates camera appropriately.
	 * \param x mouse rel x
	 * \param y mouse rel y
	 */
	void RotateBy(GLfloat x, GLfloat y);
	/**
	 * multiples the current OpenGL matrix so that it conforms to the camera values
	 */
	void GlMult();
	/**
	 * Return the look direction vector.
	 * \param vec pointer to an array[3] that receives the look vector
	 */
	const GLfloat *GetLookVec();

	/**
	 * Move forwards/backwards/left/right
	 * \param fb forwards/back
	 * \param lr left/right
	 */
	void MoveOnRelXY(GLfloat fb, GLfloat lr);


	/**
	 * Init some values directly into the camera. Because you broke point and found some nice ones.
	 */
	void InitHax(GLfloat x, GLfloat y, GLfloat z, GLfloat pitch, GLfloat yaw);
};
