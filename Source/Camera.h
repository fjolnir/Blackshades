#ifndef _CAMERA_H_
#define _CAMERA_H_


/**> HEADER FILES <**/
#ifdef OS9 
#include <gl.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Quaternions.h"

class Camera
{
	public:
		XYZ position;
		XYZ oldposition;
		XYZ targetoffset;
		
	 	float rotation, rotation2;
	 	float oldrotation, oldrotation2;
	 	float oldoldrotation, oldoldrotation2;
	 	float visrotation,  visrotation2;
		void Apply();
};

#endif

