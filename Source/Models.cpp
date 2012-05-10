#include "Models.h"

#include "Serialize.h"

//Functions
void Model::UpdateVertexArray(){
	int i;
	for(i=0;i<TriangleNum;i++){
		vArray[i*27+0]=vertex[Triangles[i].vertex[0]].x;
		vArray[i*27+1]=vertex[Triangles[i].vertex[0]].y;
		vArray[i*27+2]=vertex[Triangles[i].vertex[0]].z;
		vArray[i*27+3]=normals[i].x;
		vArray[i*27+4]=normals[i].y;
		vArray[i*27+5]=normals[i].z;
		vArray[i*27+6]=Triangles[i].r;
		vArray[i*27+7]=Triangles[i].g;
		vArray[i*27+8]=Triangles[i].b;
		
		vArray[i*27+9]=vertex[Triangles[i].vertex[1]].x;
		vArray[i*27+10]=vertex[Triangles[i].vertex[1]].y;
		vArray[i*27+11]=vertex[Triangles[i].vertex[1]].z;
		vArray[i*27+12]=normals[i].x;
		vArray[i*27+13]=normals[i].y;
		vArray[i*27+14]=normals[i].z;
		vArray[i*27+15]=Triangles[i].r;
		vArray[i*27+16]=Triangles[i].g;
		vArray[i*27+17]=Triangles[i].b;
		
		vArray[i*27+18]=vertex[Triangles[i].vertex[2]].x;
		vArray[i*27+19]=vertex[Triangles[i].vertex[2]].y;
		vArray[i*27+20]=vertex[Triangles[i].vertex[2]].z;
		vArray[i*27+21]=normals[i].x;
		vArray[i*27+22]=normals[i].y;
		vArray[i*27+23]=normals[i].z;
		vArray[i*27+24]=Triangles[i].r;
		vArray[i*27+25]=Triangles[i].g;
		vArray[i*27+26]=Triangles[i].b;
	}
	
	XYZ average;
	int howmany;
	average=0;
	howmany=0;
	boundingboxmin=20000;
	boundingboxmax=-20000;
	for(int i=0;i<vertexNum;i++){
		howmany++;
		average=average+vertex[i];
		if(vertex[i].x<boundingboxmin.x)boundingboxmin.x=vertex[i].x;
		if(vertex[i].y<boundingboxmin.y)boundingboxmin.y=vertex[i].y;
		if(vertex[i].z<boundingboxmin.z)boundingboxmin.z=vertex[i].z;
		if(vertex[i].x>boundingboxmax.x)boundingboxmax.x=vertex[i].x;
		if(vertex[i].y>boundingboxmax.y)boundingboxmax.y=vertex[i].y;
		if(vertex[i].z>boundingboxmax.z)boundingboxmax.z=vertex[i].z;
	}
	average=average/howmany;
	boundingspherecenter=average;
	boundingsphereradius=0;
	for(int i=0;i<vertexNum;i++){
		if(findDistancefast(average,vertex[i])>boundingsphereradius)boundingsphereradius=findDistancefast(average,vertex[i]);
	}
	boundingsphereradius=fast_sqrt(boundingsphereradius);
}

bool Model::load(Str255 Name)
{
	short				tfile;
	long				err;
	Files file;
	
	tfile=file.OpenFile(Name);
	SetFPos(tfile,fsFromStart,0);

		// read model settings
	
	err=ReadShort(tfile,1,&vertexNum);
	err=ReadShort(tfile,1,&TriangleNum);
	
		// read the model data
	
	err=ReadXYZ(tfile,vertexNum,vertex);
	err=ReadTexturedTriangle(tfile,TriangleNum,Triangles);

	FSClose(tfile);
		
	UpdateVertexArray();
	
	XYZ average;
	int howmany;
	average=0;
	howmany=0;
	for(int i=0;i<vertexNum;i++){
		howmany++;
		average=average+vertex[i];
	}
	average=average/howmany;
	boundingspherecenter=average;
	boundingsphereradius=0;
	for(int i=0;i<vertexNum;i++){
		if(findDistancefast(average,vertex[i])>boundingsphereradius)boundingsphereradius=findDistancefast(average,vertex[i]);
	}
	boundingsphereradius=fast_sqrt(boundingsphereradius);
	
	return 1;
}

void Model::Scale(float xscale,float yscale,float zscale)
{
	int i;
	for(i=0; i<vertexNum; i++){
		vertex[i].x*=xscale;
		vertex[i].y*=yscale;
		vertex[i].z*=zscale;
	}
	UpdateVertexArray();
}

void Model::MultColor(float howmuch)
{
	int i;
	for(i=0; i<TriangleNum; i++){
		Triangles[i].r*=howmuch;
		Triangles[i].g*=howmuch;
		Triangles[i].b*=howmuch;
	}
	UpdateVertexArray();
}

void Model::ScaleNormals(float xscale,float yscale,float zscale)
{
	int i;
	for(i=0; i<vertexNum; i++){
		normals[i].x*=xscale;
		normals[i].y*=yscale;
		normals[i].z*=zscale;
	}
	UpdateVertexArray();
}

void Model::Translate(float xtrans,float ytrans,float ztrans)
{
	int i;
	for(i=0; i<vertexNum; i++){
		vertex[i].x+=xtrans;
		vertex[i].y+=ytrans;
		vertex[i].z+=ztrans;
	}
	UpdateVertexArray();
}

void Model::Rotate(float xang,float yang,float zang)
{
	int i;
	for(i=0; i<vertexNum; i++){
		vertex[i]=DoRotation(vertex[i],xang,yang,zang);
	}
	UpdateVertexArray();
}


void Model::CalculateNormals()
{
	int i;
	for(i=0;i<TriangleNum;i++){
		CrossProduct(vertex[Triangles[i].vertex[1]]-vertex[Triangles[i].vertex[0]],vertex[Triangles[i].vertex[2]]-vertex[Triangles[i].vertex[0]],&normals[i]);
		Normalise(&normals[i]);
	}
	UpdateVertexArray();
}

extern int nocolors;
void Model::draw()
{
	if(!nocolors){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 9*sizeof(GLfloat),&vArray[0]);
	glNormalPointer(GL_FLOAT, 9*sizeof(GLfloat),&vArray[3]);
	glColorPointer(3,GL_FLOAT, 9*sizeof(GLfloat),&vArray[6]);
	glDrawArrays(GL_TRIANGLES, 0, TriangleNum*3);
	}
	if(nocolors){
		glColor4f(0,0,0,1);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, 9*sizeof(GLfloat),&vArray[0]);
		glNormalPointer(GL_FLOAT, 9*sizeof(GLfloat),&vArray[3]);
		glDrawArrays(GL_TRIANGLES, 0, TriangleNum*3);
	}
}


void Model::draw(float r, float g, float b)
{
	if(!nocolors)glColor4f(r,g,b,1);
	if(nocolors==1)glColor4f(0,0,0,1);
	if(nocolors==2)glColor4f(1,0,0,1);
	if(nocolors==3)glColor4f(0,0,1,1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 9*sizeof(GLfloat),&vArray[0]);
	glNormalPointer(GL_FLOAT, 9*sizeof(GLfloat),&vArray[3]);
	glDrawArrays(GL_TRIANGLES, 0, TriangleNum*3);
}

void Model::draw(float r, float g, float b, float o)
{
	if(!nocolors)glColor4f(r,g,b,o);
	if(nocolors==1)glColor4f(0,0,0,1);
	if(nocolors==2)glColor4f(1,0,0,1);
	if(nocolors==3)glColor4f(1,1,1,1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 9*sizeof(GLfloat),&vArray[0]);
	glNormalPointer(GL_FLOAT, 9*sizeof(GLfloat),&vArray[3]);
	glDrawArrays(GL_TRIANGLES, 0, TriangleNum*3);
}

void Model::draw(float r, float g, float b, float x, float y, float z)
{
	if(!nocolors)glColor4f(r,g,b,1);
	if(nocolors==1)glColor4f(0,0,0,1);
	if(nocolors==2)glColor4f(1,0,0,1);
	if(nocolors==3)glColor4f(1,1,1,1);
	glNormal3f(x,y,z);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 9*sizeof(GLfloat),&vArray[0]);
	glDrawArrays(GL_TRIANGLES, 0, TriangleNum*3);
}


int Model::LineCheck(XYZ p1,XYZ p2, XYZ *p)
{
  	int j;
	float distance;
	float olddistance=9999999.0;
	int intersecting=0;
	int firstintersecting=-1;
	XYZ point;
	if(sphere_line_intersection(p1.x,p1.y,p1.z,
								p2.x,p2.y,p2.z,
								boundingspherecenter.x,boundingspherecenter.y,boundingspherecenter.z,
								boundingsphereradius))
	for (j=0;j<TriangleNum;j++){
		intersecting=LineFacetd(p1,p2,vertex[Triangles[j].vertex[0]],vertex[Triangles[j].vertex[1]],vertex[Triangles[j].vertex[2]],normals[j],&point);
		if (intersecting == 0) continue;
		distance=(point.x-p1.x)*(point.x-p1.x)+(point.y-p1.y)*(point.y-p1.y)+(point.z-p1.z)*(point.z-p1.z);
		if((distance<olddistance||firstintersecting==-1)&&intersecting){olddistance=distance; firstintersecting=j; *p=point;}
	}
	return firstintersecting;
}

int Model::LineCheck2(XYZ p1,XYZ p2, XYZ *p, XYZ move, float rotate)
{
  	int j;
	float distance;
	float olddistance=9999999.0;
	int intersecting=0;
	int firstintersecting=-1;
	XYZ point;
	p1=p1-move;
	p2=p2-move;
	if(rotate)p1=DoRotation(p1,0,-rotate,0);
	if(rotate)p2=DoRotation(p2,0,-rotate,0);
	if(sphere_line_intersection(p1.x,p1.y,p1.z,
								p2.x,p2.y,p2.z,
								boundingspherecenter.x,boundingspherecenter.y,boundingspherecenter.z,
								boundingsphereradius))
	for (j=0;j<TriangleNum;j++){
		intersecting=LineFacetd(p1,p2,vertex[Triangles[j].vertex[0]],vertex[Triangles[j].vertex[1]],vertex[Triangles[j].vertex[2]],normals[j],&point);
		if (intersecting == 0) continue;
		distance=(point.x-p1.x)*(point.x-p1.x)+(point.y-p1.y)*(point.y-p1.y)+(point.z-p1.z)*(point.z-p1.z);
		if((distance<olddistance||firstintersecting==-1)&&intersecting){olddistance=distance; firstintersecting=j; *p=point;}
	}
	
	if(rotate)*p=DoRotation(*p,0,rotate,0);
	*p=*p+move;
	return firstintersecting;
}

int Model::LineCheck2(XYZ *p1,XYZ *p2, XYZ *p, XYZ *move, float *rotate)
{
  	int j;
	float distance;
	float olddistance=9999999.0;
	int intersecting=0;
	int firstintersecting=-1;
	XYZ point;
	*p1=*p1-*move;
	*p2=*p2-*move;
	if(*rotate)*p1=DoRotation(*p1,0,-*rotate,0);
	if(*rotate)*p2=DoRotation(*p2,0,-*rotate,0);
	if(sphere_line_intersection(p1->x,p1->y,p1->z,
								p2->x,p2->y,p2->z,
								boundingspherecenter.x,boundingspherecenter.y,boundingspherecenter.z,
								boundingsphereradius))
	for (j=0;j<TriangleNum;j++){
		intersecting=LineFacetd(p1,p2,&vertex[Triangles[j].vertex[0]],&vertex[Triangles[j].vertex[1]],&vertex[Triangles[j].vertex[2]],&normals[j],&point);
		if (intersecting == 0) continue;
		distance=(point.x-p1->x)*(point.x-p1->x)+(point.y-p1->y)*(point.y-p1->y)+(point.z-p1->z)*(point.z-p1->z);
		if((distance<olddistance||firstintersecting==-1)&&intersecting){olddistance=distance; firstintersecting=j; *p=point;}
	}
	
	if(*rotate)*p=DoRotation(*p,0,*rotate,0);
	*p=*p+*move;
	return firstintersecting;
}

int Model::LineCheck3(XYZ p1,XYZ p2, XYZ *p, XYZ move, float rotate, float *d)
{
  	int j;
	float distance;
	float olddistance=9999999.0;
	int intersecting=0;
	int firstintersecting=-1;
	XYZ point;
	p1=p1-move;
	p2=p2-move;
	p1=DoRotation(p1,0,-rotate,0);
	p2=DoRotation(p2,0,-rotate,0);
	if(sphere_line_intersection(p1.x,p1.y,p1.z,
								p2.x,p2.y,p2.z,
								boundingspherecenter.x,boundingspherecenter.y,boundingspherecenter.z,
								boundingsphereradius))
	for (j=0;j<TriangleNum;j++){
		intersecting=LineFacetd(p1,p2,vertex[Triangles[j].vertex[0]],vertex[Triangles[j].vertex[1]],vertex[Triangles[j].vertex[2]],normals[j],&point);
		if (intersecting == 0) continue;
		distance=(point.x-p1.x)*(point.x-p1.x)+(point.y-p1.y)*(point.y-p1.y)+(point.z-p1.z)*(point.z-p1.z);
		if((distance<olddistance||firstintersecting==-1)&&intersecting){olddistance=distance; firstintersecting=j; *p=point;}
	}
	*d=intersecting;
	*p=DoRotation(*p,0,rotate,0);
	*p=*p+move;
	return firstintersecting;
}
