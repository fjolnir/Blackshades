#include "Quaternions.h"

// Functions
quaternion Quat_Mult(quaternion q1, quaternion q2)
{
        quaternion QResult;
        float a, b, c, d, e, f, g, h;
        a = (q1.w + q1.x) * (q2.w + q2.x);
        b = (q1.z - q1.y) * (q2.y - q2.z);
        c = (q1.w - q1.x) * (q2.y + q2.z);
        d = (q1.y + q1.z) * (q2.w - q2.x);
        e = (q1.x + q1.z) * (q2.x + q2.y);
        f = (q1.x - q1.z) * (q2.x - q2.y);
        g = (q1.w + q1.y) * (q2.w - q2.z);
        h = (q1.w - q1.y) * (q2.w + q2.z);
        QResult.w = b + (-e - f + g + h) / 2;
        QResult.x = a - (e + f + g + h) / 2;
        QResult.y = c + (e - f + g - h) / 2;
        QResult.z = d + (e - f - g + h) / 2;
        return QResult;
}

XYZ XYZ::operator+(XYZ add){
	XYZ ne;
	ne=add;
	ne.x+=x;
	ne.y+=y;
	ne.z+=z;
	return ne;
}

XYZ XYZ::operator-(XYZ add){
	XYZ ne;
	ne=add;
	ne.x=x-ne.x;
	ne.y=y-ne.y;
	ne.z=z-ne.z;
	return ne;
}

XYZ XYZ::operator*(float add){
	XYZ ne;
	ne.x=x*add;
	ne.y=y*add;
	ne.z=z*add;
	return ne;
}

XYZ XYZ::operator*(XYZ add){
	XYZ ne;
	ne.x=x*add.x;
	ne.y=y*add.y;
	ne.z=z*add.z;
	return ne;
}

XYZ XYZ::operator/(float add){
	XYZ ne;
	ne.x=x/add;
	ne.y=y/add;
	ne.z=z/add;
	return ne;
}

void XYZ::operator+=(XYZ add){
	x+=add.x;
	y+=add.y;
	z+=add.z;
}

void XYZ::operator-=(XYZ add){
	x=x-add.x;
	y=y-add.y;
	z=z-add.z;
}

void XYZ::operator*=(float add){
	x=x*add;
	y=y*add;
	z=z*add;
}

void XYZ::operator*=(XYZ add){
	x=x*add.x;
	y=y*add.y;
	z=z*add.z;
}

void XYZ::operator/=(float add){
	x=x/add;
	y=y/add;
	z=z/add;
}

void XYZ::operator=(float add){
	x=add;
	y=add;
	z=add;
}

void XYZ::vec(Vector add){
	x=add.x;
	y=add.y;
	z=add.z;
}

bool XYZ::operator==(XYZ add){
	if(x==add.x&&y==add.y&&z==add.z)return 1;
	return 0;
}

quaternion To_Quat(Matrix_t m)
{
        // From Jason Shankel, (C) 2000.
        quaternion Quat;
        
        double Tr = m[0][0] + m[1][1] + m[2][2] + 1.0, fourD;
        double q[4];
        
        int i,j,k;
        if (Tr >= 1.0)
        {
                fourD = 2.0*fast_sqrt(Tr);
                q[3] = fourD/4.0;
                q[0] = (m[2][1] - m[1][2]) / fourD;
                q[1] = (m[0][2] - m[2][0]) / fourD;
                q[2] = (m[1][0] - m[0][1]) / fourD;
        }
        else
        {
                if (m[0][0] > m[1][1])
                {
                        i = 0;
                }
                else
                {
                        i = 1;
                }
                if (m[2][2] > m[i][i])
                {
                        i = 2;
                }
                j = (i+1)%3;
                k = (j+1)%3;
                fourD = 2.0*fast_sqrt(m[i][i] - m[j][j] - m[k][k] + 1.0);
                q[i] = fourD / 4.0;
                q[j] = (m[j][i] + m[i][j]) / fourD;
                q[k] = (m[k][i] + m[i][k]) / fourD;
                q[3] = (m[j][k] - m[k][j]) / fourD;
        }
        
        Quat.x = q[0];
        Quat.y = q[1];
        Quat.z = q[2];
        Quat.w = q[3];
        return Quat;
}
void Quat_2_Matrix(quaternion Quat, Matrix_t m)
{
        // From the GLVelocity site (http://glvelocity.gamedev.net)
        float fW = Quat.w;
        float fX = Quat.x;
        float fY = Quat.y;
        float fZ = Quat.z;
        float fXX = fX * fX;
        float fYY = fY * fY;
        float fZZ = fZ * fZ;
        m[0][0] = 1.0f - 2.0f * (fYY + fZZ);
        m[1][0] = 2.0f * (fX * fY + fW * fZ);
        m[2][0] = 2.0f * (fX * fZ - fW * fY);
        m[3][0] = 0.0f;
        m[0][1] = 2.0f * (fX * fY - fW * fZ);
        m[1][1] = 1.0f - 2.0f * (fXX + fZZ);
        m[2][1] = 2.0f * (fY * fZ + fW * fX);
        m[3][1] = 0.0f;
        m[0][2] = 2.0f * (fX * fZ + fW * fY);
        m[1][2] = 2.0f * (fX * fZ - fW * fX);
        m[2][2] = 1.0f - 2.0f * (fXX + fYY);
        m[3][2] = 0.0f;
        m[0][3] = 0.0f;
        m[1][3] = 0.0f;
        m[2][3] = 0.0f;
        m[3][3] = 1.0f;
}
quaternion To_Quat(angle_axis Ang_Ax)
{
        // From the Quaternion Powers article on gamedev.net
        quaternion Quat;
        
        Quat.x = Ang_Ax.x * sin(Ang_Ax.angle / 2);
        Quat.y = Ang_Ax.y * sin(Ang_Ax.angle / 2);
        Quat.z = Ang_Ax.z * sin(Ang_Ax.angle / 2);
        Quat.w = cos(Ang_Ax.angle / 2);
        return Quat;
}
angle_axis Quat_2_AA(quaternion Quat)
{
        angle_axis Ang_Ax;
        float scale, tw;
        tw = (float)acos(Quat.w) * 2;
        scale = (float)sin(tw / 2.0);
        Ang_Ax.x = Quat.x / scale;
        Ang_Ax.y = Quat.y / scale;
        Ang_Ax.z = Quat.z / scale;
        
        Ang_Ax.angle = 2.0 * acos(Quat.w)/(float)PI*180;
        return Ang_Ax;
}

quaternion To_Quat(int In_Degrees, euler Euler)
{
        // From the gamasutra quaternion article
        quaternion Quat;
        float cr, cp, cy, sr, sp, sy, cpcy, spsy;
        //If we are in Degree mode, convert to Radians
        if (In_Degrees) {
                Euler.x = Euler.x * (float)PI / 180;
                Euler.y = Euler.y * (float)PI / 180;
                Euler.z = Euler.z * (float)PI / 180;
        }
        //Calculate trig identities
        //Formerly roll, pitch, yaw
        cr = float(cos(Euler.x/2));
        cp = float(cos(Euler.y/2));
        cy = float(cos(Euler.z/2));
        sr = float(sin(Euler.x/2));
        sp = float(sin(Euler.y/2));
        sy = float(sin(Euler.z/2));
        
        cpcy = cp * cy;
        spsy = sp * sy;
        Quat.w = cr * cpcy + sr * spsy;
        Quat.x = sr * cpcy - cr * spsy;
        Quat.y = cr * sp * cy + sr * cp * sy;
        Quat.z = cr * cp * sy - sr * sp * cy;
        
        return Quat;
}
        
quaternion QNormalize(quaternion Quat)
{
        float norm;
        norm =  Quat.x * Quat.x + 
                Quat.y * Quat.y + 
                Quat.z * Quat.z + 
                Quat.w * Quat.w;
        Quat.x = float(Quat.x / norm);
        Quat.y = float(Quat.y / norm);
        Quat.z = float(Quat.z / norm);
        Quat.w = float(Quat.w / norm);
        return Quat;
}

XYZ Quat2Vector(quaternion Quat)
{
	QNormalize(Quat);

	float fW = Quat.w;
	float fX = Quat.x;
	float fY = Quat.y;
	float fZ = Quat.z;
	
	XYZ tempvec;

	tempvec.x = 2.0f*(fX*fZ-fW*fY);
	tempvec.y = 2.0f*(fY*fZ+fW*fX);
	tempvec.z = 1.0f-2.0f*(fX*fX+fY*fY);
	
	return tempvec;
}

void CrossProduct(XYZ P, XYZ Q, XYZ *V){
	V->x = P.y * Q.z - P.z * Q.y;
	V->y = P.z * Q.x - P.x * Q.z;
	V->z = P.x * Q.y - P.y * Q.x;
}

void Normalise(XYZ *vectory) {
	float d = fast_sqrt(vectory->x*vectory->x+vectory->y*vectory->y+vectory->z*vectory->z);
	if(d==0){return;}
	vectory->x /= d;
	vectory->y /= d;
	vectory->z /= d;
}

float fast_sqrt (register float arg)
{	
#ifdef OS9 
	// Can replace with slower return std::sqrt(arg);
	register float result;

	if (arg == 0.0) return 0.0;
	
	asm {
		frsqrte		result,arg			// Calculate Square root
	}	
	
	// Newton Rhapson iterations.
	result = result + 0.5 * result * (1.0 - arg * result * result);
	result = result + 0.5 * result * (1.0 - arg * result * result);
	
	return result * arg;
#else
	return sqrt(arg);
#endif
}

float normaldotproduct(XYZ point1, XYZ point2){
	GLfloat returnvalue;
	Normalise(&point1);
	Normalise(&point2);
	returnvalue=(point1.x*point2.x+point1.y*point2.y+point1.z*point2.z);
	return returnvalue;
}

extern float u0, u1, u2;
extern float v0, v1, v2;
extern float a, b;
extern float max;
extern int i, j;
extern bool bInter;
extern float pointv[3];
extern float p1v[3];
extern float p2v[3];
extern float p3v[3];
extern float normalv[3];

bool PointInTriangle(Vector *p, Vector normal, float p11, float p12, float p13, float p21, float p22, float p23, float p31, float p32, float p33)
{
	bInter=0;
	
	pointv[0]=p->x;
	pointv[1]=p->y;
	pointv[2]=p->z;
	
	
	p1v[0]=p11;
	p1v[1]=p12;
	p1v[2]=p13;
	
	p2v[0]=p21;
	p2v[1]=p22;
	p2v[2]=p23;
	
	p3v[0]=p31;
	p3v[1]=p32;
	p3v[2]=p33;
	
	normalv[0]=normal.x;
	normalv[1]=normal.y;
	normalv[2]=normal.z;
	
#define ABS(X) (((X)<0.f)?-(X):(X) )
#define MAX(A, B) (((A)<(B))?(B):(A))	
	max = MAX(MAX(ABS(normalv[0]), ABS(normalv[1])), ABS(normalv[2]));
#undef MAX
	if (max == ABS(normalv[0])) {i = 1; j = 2;} // y, z
	if (max == ABS(normalv[1])) {i = 0; j = 2;} // x, z
	if (max == ABS(normalv[2])) {i = 0; j = 1;} // x, y
#undef ABS
	
	u0 = pointv[i] - p1v[i];
	v0 = pointv[j] - p1v[j];
	u1 = p2v[i] - p1v[i];
	v1 = p2v[j] - p1v[j];
	u2 = p3v[i] - p1v[i];
	v2 = p3v[j] - p1v[j];

	if (u1 > -1.0e-05f && u1 < 1.0e-05f)// == 0.0f)
	{
		b = u0 / u2;
		if (0.0f <= b && b <= 1.0f)
		{
			a = (v0 - b * v2) / v1;
			if ((a >= 0.0f) && (( a + b ) <= 1.0f))
				bInter = 1;
		}
	}
	else
	{
		b = (v0 * u1 - u0 * v1) / (v2 * u1 - u2 * v1);
		if (0.0f <= b && b <= 1.0f)
		{
			a = (u0 - b * u2) / u1;
			if ((a >= 0.0f) && (( a + b ) <= 1.0f ))
				bInter = 1;
		}
	}

	return bInter;
}

bool LineFacet(Vector p1,Vector p2,Vector pa,Vector pb,Vector pc,Vector *p)
{
   float d;
   float denom,mu;
   Vector n,pa1,pa2,pa3;

   //Calculate the parameters for the plane 
   n.x = (pb.y - pa.y)*(pc.z - pa.z) - (pb.z - pa.z)*(pc.y - pa.y);
   n.y = (pb.z - pa.z)*(pc.x - pa.x) - (pb.x - pa.x)*(pc.z - pa.z);
   n.z = (pb.x - pa.x)*(pc.y - pa.y) - (pb.y - pa.y)*(pc.x - pa.x);
   n.Normalize();
   d = - n.x * pa.x - n.y * pa.y - n.z * pa.z;

   //Calculate the position on the line that intersects the plane 
   denom = n.x * (p2.x - p1.x) + n.y * (p2.y - p1.y) + n.z * (p2.z - p1.z);
   if (abs(denom) < 0.0000001)        // Line and plane don't intersect 
      return 0;
   mu = - (d + n.x * p1.x + n.y * p1.y + n.z * p1.z) / denom;
   p->x = p1.x + mu * (p2.x - p1.x);
   p->y = p1.y + mu * (p2.y - p1.y);
   p->z = p1.z + mu * (p2.z - p1.z);
   if (mu < 0 || mu > 1)   // Intersection not along line segment 
      return 0;
    
   if(!PointInTriangle( p, n, pa.x, pa.y, pa.z, pb.x, pb.y, pb.z, pc.x, pc.y, pc.z)){return 0;}
   
   return 1;
}

bool PointInTriangle(XYZ *p, XYZ normal, XYZ *p1, XYZ *p2, XYZ *p3)
{
	bInter=0;
		
	pointv[0]=p->x;
	pointv[1]=p->y;
	pointv[2]=p->z;
	
	
	p1v[0]=p1->x;
	p1v[1]=p1->y;
	p1v[2]=p1->z;
	
	p2v[0]=p2->x;
	p2v[1]=p2->y;
	p2v[2]=p2->z;
	
	p3v[0]=p3->x;
	p3v[1]=p3->y;
	p3v[2]=p3->z;
	
	normalv[0]=normal.x;
	normalv[1]=normal.y;
	normalv[2]=normal.z;
	
#define ABS(X) (((X)<0.f)?-(X):(X) )
#define MAX(A, B) (((A)<(B))?(B):(A))	
	max = MAX(MAX(ABS(normalv[0]), ABS(normalv[1])), ABS(normalv[2]));
#undef MAX
	if (max == ABS(normalv[0])) {i = 1; j = 2;} // y, z
	if (max == ABS(normalv[1])) {i = 0; j = 2;} // x, z
	if (max == ABS(normalv[2])) {i = 0; j = 1;} // x, y
#undef ABS
	
	u0 = pointv[i] - p1v[i];
	v0 = pointv[j] - p1v[j];
	u1 = p2v[i] - p1v[i];
	v1 = p2v[j] - p1v[j];
	u2 = p3v[i] - p1v[i];
	v2 = p3v[j] - p1v[j];

	if (u1 > -1.0e-05f && u1 < 1.0e-05f)// == 0.0f)
	{
		b = u0 / u2;
		if (0.0f <= b && b <= 1.0f)
		{
			a = (v0 - b * v2) / v1;
			if ((a >= 0.0f) && (( a + b ) <= 1.0f))
				bInter = 1;
		}
	}
	else
	{
		b = (v0 * u1 - u0 * v1) / (v2 * u1 - u2 * v1);
		if (0.0f <= b && b <= 1.0f)
		{
			a = (u0 - b * u2) / u1;
			if ((a >= 0.0f) && (( a + b ) <= 1.0f ))
				bInter = 1;
		}
	}

	return bInter;
}

bool LineFacet(XYZ p1,XYZ p2,XYZ pa,XYZ pb,XYZ pc,XYZ *p)
{
   float d;
   float denom,mu;
   XYZ n;

   //Calculate the parameters for the plane 
   n.x = (pb.y - pa.y)*(pc.z - pa.z) - (pb.z - pa.z)*(pc.y - pa.y);
   n.y = (pb.z - pa.z)*(pc.x - pa.x) - (pb.x - pa.x)*(pc.z - pa.z);
   n.z = (pb.x - pa.x)*(pc.y - pa.y) - (pb.y - pa.y)*(pc.x - pa.x);
   Normalise(&n);
   d = - n.x * pa.x - n.y * pa.y - n.z * pa.z;

   //Calculate the position on the line that intersects the plane 
   denom = n.x * (p2.x - p1.x) + n.y * (p2.y - p1.y) + n.z * (p2.z - p1.z);
   if (abs(denom) < 0.0000001)        // Line and plane don't intersect 
      return 0;
   mu = - (d + n.x * p1.x + n.y * p1.y + n.z * p1.z) / denom;
   p->x = p1.x + mu * (p2.x - p1.x);
   p->y = p1.y + mu * (p2.y - p1.y);
   p->z = p1.z + mu * (p2.z - p1.z);
   if (mu < 0 || mu > 1)   // Intersection not along line segment 
      return 0;
    
   if(!PointInTriangle( p, n, &pa, &pb, &pc)){return 0;}
   
   return 1;
}

extern float d;
extern float a1,a2,a3;
extern float total,denom,mu;
extern XYZ pa1,pa2,pa3,n;

float LineFacetd(XYZ p1,XYZ p2,XYZ pa,XYZ pb,XYZ pc,XYZ *p)
{

   //Calculate the parameters for the plane 
   n.x = (pb.y - pa.y)*(pc.z - pa.z) - (pb.z - pa.z)*(pc.y - pa.y);
   n.y = (pb.z - pa.z)*(pc.x - pa.x) - (pb.x - pa.x)*(pc.z - pa.z);
   n.z = (pb.x - pa.x)*(pc.y - pa.y) - (pb.y - pa.y)*(pc.x - pa.x);
   Normalise(&n);
   d = - n.x * pa.x - n.y * pa.y - n.z * pa.z;

   //Calculate the position on the line that intersects the plane 
   denom = n.x * (p2.x - p1.x) + n.y * (p2.y - p1.y) + n.z * (p2.z - p1.z);
   if (abs(denom) < 0.0000001)        // Line and plane don't intersect 
      return 0;
   mu = - (d + n.x * p1.x + n.y * p1.y + n.z * p1.z) / denom;
   p->x = p1.x + mu * (p2.x - p1.x);
   p->y = p1.y + mu * (p2.y - p1.y);
   p->z = p1.z + mu * (p2.z - p1.z);
   if (mu < 0 || mu > 1)   // Intersection not along line segment 
      return 0;
    
   if(!PointInTriangle( p, n, &pa, &pb, &pc)){return 0;}
   
   return 1;
}

float LineFacetd(XYZ p1,XYZ p2,XYZ pa,XYZ pb,XYZ pc, XYZ n, XYZ *p)
{

   //Calculate the parameters for the plane 
   d = - n.x * pa.x - n.y * pa.y - n.z * pa.z;

   //Calculate the position on the line that intersects the plane 
   denom = n.x * (p2.x - p1.x) + n.y * (p2.y - p1.y) + n.z * (p2.z - p1.z);
   if (abs(denom) < 0.0000001)        // Line and plane don't intersect 
      return 0;
   mu = - (d + n.x * p1.x + n.y * p1.y + n.z * p1.z) / denom;
   p->x = p1.x + mu * (p2.x - p1.x);
   p->y = p1.y + mu * (p2.y - p1.y);
   p->z = p1.z + mu * (p2.z - p1.z);
   if (mu < 0 || mu > 1)   // Intersection not along line segment 
      return 0;
    
   if(!PointInTriangle( p, n, &pa, &pb, &pc)){return 0;}
   return 1;
}

float LineFacetd(XYZ *p1,XYZ *p2,XYZ *pa,XYZ *pb,XYZ *pc, XYZ *n, XYZ *p)
{

   //Calculate the parameters for the plane 
   d = - n->x * pa->x - n->y * pa->y - n->z * pa->z;

   //Calculate the position on the line that intersects the plane 
   denom = n->x * (p2->x - p1->x) + n->y * (p2->y - p1->y) + n->z * (p2->z - p1->z);
   if (abs(denom) < 0.0000001)        // Line and plane don't intersect 
      return 0;
   mu = - (d + n->x * p1->x + n->y * p1->y + n->z * p1->z) / denom;
   p->x = p1->x + mu * (p2->x - p1->x);
   p->y = p1->y + mu * (p2->y - p1->y);
   p->z = p1->z + mu * (p2->z - p1->z);
   if (mu < 0 || mu > 1)   // Intersection not along line segment 
      return 0;
    
   if(!PointInTriangle( p, *n, pa, pb, pc)){return 0;}
   return 1;
}

void ReflectVector(XYZ *vel, XYZ *n)
{
   XYZ vn;
   XYZ vt;
   float dotprod;
   
   dotprod=dotproduct(*n,*vel);
   vn.x=n->x*dotprod;
   vn.y=n->y*dotprod;
   vn.z=n->z*dotprod;

   vt.x=vel->x-vn.x;
   vt.y=vel->y-vn.y;
   vt.z=vel->z-vn.z;
	
   vel->x = vt.x - vn.x;
   vel->y = vt.y - vn.y;
   vel->z = vt.z - vn.z;
}

float dotproduct(XYZ point1, XYZ point2){
	GLfloat returnvalue;
	returnvalue=(point1.x*point2.x+point1.y*point2.y+point1.z*point2.z);
	return returnvalue;
}

float findDistance(XYZ point1, XYZ point2){
	return(fast_sqrt((point1.x-point2.x)*(point1.x-point2.x)+(point1.y-point2.y)*(point1.y-point2.y)+(point1.z-point2.z)*(point1.z-point2.z)));
}

float findLength(XYZ point1){
	return(fast_sqrt((point1.x)*(point1.x)+(point1.y)*(point1.y)+(point1.z)*(point1.z)));
}


float findLengthfast(XYZ point1){
	return((point1.x)*(point1.x)+(point1.y)*(point1.y)+(point1.z)*(point1.z));
}

float findDistancefast(XYZ point1, XYZ point2){
	return((point1.x-point2.x)*(point1.x-point2.x)+(point1.y-point2.y)*(point1.y-point2.y)+(point1.z-point2.z)*(point1.z-point2.z));
}

XYZ DoRotation(XYZ thePoint, float xang, float yang, float zang){
	XYZ newpoint;
	if(xang){
		xang*=6.283185;
		xang/=360;
	}
	if(yang){
		yang*=6.283185;
		yang/=360;
	}
	if(zang){
		zang*=6.283185;
		zang/=360;
	}
	
	
	if(yang){
	newpoint.z=thePoint.z*cos(yang)-thePoint.x*sin(yang);
	newpoint.x=thePoint.z*sin(yang)+thePoint.x*cos(yang);
	thePoint.z=newpoint.z;
	thePoint.x=newpoint.x;
	}
	
	if(zang){
	newpoint.x=thePoint.x*cos(zang)-thePoint.y*sin(zang);
	newpoint.y=thePoint.y*cos(zang)+thePoint.x*sin(zang);
	thePoint.x=newpoint.x;
	thePoint.y=newpoint.y;
	}
	
	if(xang){
	newpoint.y=thePoint.y*cos(xang)-thePoint.z*sin(xang);
	newpoint.z=thePoint.y*sin(xang)+thePoint.z*cos(xang);
	thePoint.z=newpoint.z;
	thePoint.y=newpoint.y;	
	}
	
	return thePoint;
}

float square( float f ) { return (f*f) ;}

bool sphere_line_intersection (
    float x1, float y1 , float z1,
    float x2, float y2 , float z2,
    float x3, float y3 , float z3, float r )
{

	 // x1,y1,z1  P1 coordinates (point of line)
	 // x2,y2,z2  P2 coordinates (point of line)
	 // x3,y3,z3, r  P3 coordinates and radius (sphere)
	 //
	 // This function returns a pointer array which first index indicates
	 // the number of intersection point, followed by coordinate pairs.

	 float a, b, c, i ;
	
	if(x1>x3+r&&x2>x3+r)return(0);
	if(x1<x3-r&&x2<x3-r)return(0);
	if(y1>y3+r&&y2>y3+r)return(0);
	if(y1<y3-r&&y2<y3-r)return(0);
	if(z1>z3+r&&z2>z3+r)return(0);
	if(z1<z3-r&&z2<z3-r)return(0);
	 a =  square(x2 - x1) + square(y2 - y1) + square(z2 - z1);
	 b =  2* ( (x2 - x1)*(x1 - x3)
	      + (y2 - y1)*(y1 - y3)
	      + (z2 - z1)*(z1 - z3) ) ;
	 c =  square(x3) + square(y3) +
	      square(z3) + square(x1) +
	      square(y1) + square(z1) -
	      2* ( x3*x1 + y3*y1 + z3*z1 ) - square(r) ;
	 i =   b * b - 4 * a * c ;

	return (i >= 0.0);
}

XYZ DoRotationRadian(XYZ thePoint, float xang, float yang, float zang){
	XYZ newpoint;
	XYZ oldpoint;
	
	oldpoint=thePoint;
	
	if(yang!=0){
	newpoint.z=oldpoint.z*cos(yang)-oldpoint.x*sin(yang);
	newpoint.x=oldpoint.z*sin(yang)+oldpoint.x*cos(yang);
	oldpoint.z=newpoint.z;
	oldpoint.x=newpoint.x;
	}
	
	if(zang!=0){
	newpoint.x=oldpoint.x*cos(zang)-oldpoint.y*sin(zang);
	newpoint.y=oldpoint.y*cos(zang)+oldpoint.x*sin(zang);
	oldpoint.x=newpoint.x;
	oldpoint.y=newpoint.y;
	}
	
	if(xang!=0){
	newpoint.y=oldpoint.y*cos(xang)-oldpoint.z*sin(xang);
	newpoint.z=oldpoint.y*sin(xang)+oldpoint.z*cos(xang);
	oldpoint.z=newpoint.z;
	oldpoint.y=newpoint.y;	
	}
	
	return oldpoint;
	
}

