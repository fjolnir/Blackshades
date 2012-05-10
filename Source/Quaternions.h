
#ifndef _QUATERNIONS_H_
#define _QUATERNIONS_H_

#ifdef OS9 
#pragma mark -
#endif

#include "Maths.h"
#ifdef OS9 
#include <gl.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include "PhysicsMath.h"

/**> Quaternion Structures <**/
#define PI      3.14159265355555897932384626
#define RADIANS 0
#define DEGREES 1
#define deg2rad .0174532925

using namespace std;
typedef float Matrix_t [4][4];
struct euler
{
        float x, y, z;
};
struct angle_axis
{
        float x, y, z, angle;
};
struct quaternion
{
        float x, y, z, w;
};

class XYZ{
	public:
		float x;
		float y;
		float z;
		XYZ operator+(XYZ add);
		XYZ operator-(XYZ add);
		XYZ operator*(float add);
		XYZ operator*(XYZ add);
		XYZ operator/(float add);
		void operator+=(XYZ add);
		void operator-=(XYZ add);
		void operator*=(float add);
		void operator*=(XYZ add);
		void operator/=(float add);
		void operator=(float add);
		void vec(Vector add);
		bool operator==(XYZ add);
};

/*********************> Quaternion Function definition <********/
quaternion To_Quat(int Degree_Flag, euler Euler);
quaternion To_Quat(angle_axis Ang_Ax);
quaternion To_Quat(Matrix_t m);
angle_axis Quat_2_AA(quaternion Quat);
void Quat_2_Matrix(quaternion Quat, Matrix_t m);
quaternion Normalize(quaternion Quat);
quaternion Quat_Mult(quaternion q1, quaternion q2);
quaternion QNormalize(quaternion Quat);
XYZ Quat2Vector(quaternion Quat);

void CrossProduct(XYZ P, XYZ Q, XYZ *V);
void Normalise(XYZ *vectory);
float normaldotproduct(XYZ point1, XYZ point2);
float fast_sqrt (register float arg);
bool PointInTriangle(XYZ *p, XYZ normal, XYZ *p1, XYZ *p2, XYZ *p3);
bool LineFacet(XYZ p1,XYZ p2,XYZ pa,XYZ pb,XYZ pc,XYZ *p);
float LineFacetd(XYZ p1,XYZ p2,XYZ pa,XYZ pb,XYZ pc,XYZ *p);
float LineFacetd(XYZ p1,XYZ p2,XYZ pa,XYZ pb,XYZ pc,XYZ n, XYZ *p);
float LineFacetd(XYZ *p1,XYZ *p2,XYZ *pa,XYZ *pb,XYZ *pc,XYZ *n, XYZ *p);
bool PointInTriangle(Vector *p, Vector normal, float p11, float p12, float p13, float p21, float p22, float p23, float p31, float p32, float p33);
bool LineFacet(Vector p1,Vector p2,Vector pa,Vector pb,Vector pc,Vector *p);
void ReflectVector(XYZ *vel, XYZ *n);
XYZ DoRotation(XYZ thePoint, float xang, float yang, float zang);
XYZ DoRotationRadian(XYZ thePoint, float xang, float yang, float zang);
float findDistance(XYZ point1, XYZ point2);
float findLength(XYZ point1);
float findLengthfast(XYZ point1);
float findDistancefast(XYZ point1, XYZ point2);
float dotproduct(XYZ point1, XYZ point2);
bool sphere_line_intersection (
    float x1, float y1 , float z1,
    float x2, float y2 , float z2,
    float x3, float y3 , float z3, float r );

#endif

