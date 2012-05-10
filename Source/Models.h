#ifndef _MODELS_H_
#define _MODELS_H_

/**> Model Loading <**/
//
// Model Maximums
//
#include "Quaternions.h"
#ifdef OS9 
#include <gl.h>
#include <glu.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "Files.h"
#include "Constants.h"

#define max_textured_triangle		400		// maximum number of texture-filled triangles in a model
#define max_model_vertex			max_textured_triangle*3		// maximum number of vertexs


//
// Model Structures
//
					
class TexturedTriangle{
	public:
				short			vertex[3];
				float r,g,b;
};

class Model{
	public:
				short	vertexNum,TriangleNum;
				
				XYZ					vertex[max_model_vertex];
				XYZ					normals[max_textured_triangle];
				TexturedTriangle	Triangles[max_textured_triangle];
				GLfloat 			vArray[max_textured_triangle*27];
				
				XYZ boundingspherecenter;
				float boundingsphereradius;
				int LineCheck(XYZ p1,XYZ p2, XYZ *p);
				int LineCheck2(XYZ p1,XYZ p2, XYZ *p,XYZ move,float rotate);
				int LineCheck2(XYZ *p1,XYZ *p2, XYZ *p,XYZ *move,float *rotate);
				int LineCheck3(XYZ p1,XYZ p2, XYZ *p,XYZ move,float rotate,float *d);
				
				void UpdateVertexArray();
				bool load(Str255 Name);
				void Scale(float xscale,float yscale,float zscale);
				void ScaleNormals(float xscale,float yscale,float zscale);
				void Translate(float xtrans,float ytrans,float ztrans);
				void CalculateNormals();
				void draw();
				void draw(float r,float g,float b);
				void draw(float r,float g,float b, float o);
				void draw(float r,float g,float b, float x, float y, float z);
				void Rotate(float xang,float yang,float zang);
				void MultColor(float howmuch);
				
				XYZ boundingboxmin,boundingboxmax;
};

#endif

