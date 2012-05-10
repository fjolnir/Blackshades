#include "Sprites.h"
#include "Textures.h"

extern double multiplier;
extern bool slomo;
extern Fog fog;
extern bool blood;
extern float fogcolorr;
extern float fogcolorg;
extern float fogcolorb;
//Functions
extern float sinefluct;
extern int environment;
extern Model gunmodels[10];
extern Camera camera;
extern float precipitationhorz;
extern float precipitationvert;
extern float precipitationdensity;
extern float snowdelay;

int Sprites::MakeSprite(int atype, float abrightness, float acolor1, float acolor2, float acolor3, XYZ alocation, XYZ avelocity, float asize){
	type[howmanysprites]=atype;
	rotation[howmanysprites]=Random()%360;
	brightness[howmanysprites]=abrightness;
	color1[howmanysprites]=acolor1;
	color2[howmanysprites]=acolor2;
	color3[howmanysprites]=acolor3;
	location[howmanysprites]=alocation;
	oldlocation[howmanysprites]=alocation;
	velocity[howmanysprites]=avelocity;
	size[howmanysprites]=asize;
	initialsize[howmanysprites]=asize;
	initialbrightness[howmanysprites]=abrightness;
	initialvelocity[howmanysprites]=avelocity;
	alivetime[howmanysprites]=0;
	owner[howmanysprites]=0;
	if(howmanysprites<maxsprites){howmanysprites++;}
	
	return 0;
}

int Sprites::MakeSprite(int atype, float abrightness, float acolor1, float acolor2, float acolor3, XYZ alocation, XYZ avelocity, float asize, int aowner){
	type[howmanysprites]=atype;
	rotation[howmanysprites]=Random()%360;
	brightness[howmanysprites]=abrightness;
	color1[howmanysprites]=acolor1;
	color2[howmanysprites]=acolor2;
	color3[howmanysprites]=acolor3;
	location[howmanysprites]=alocation;
	oldlocation[howmanysprites]=alocation;
	velocity[howmanysprites]=avelocity;
	size[howmanysprites]=asize;
	initialsize[howmanysprites]=asize;
	initialbrightness[howmanysprites]=abrightness;
	initialvelocity[howmanysprites]=avelocity;
	alivetime[howmanysprites]=0;
	owner[howmanysprites]=aowner;
	if(howmanysprites<maxsprites){howmanysprites++;}
	
	return 0;
}

int Sprites::DeleteSprite(int which){
	if(which>=0){
		location[which]=location[howmanysprites-1];
		oldlocation[which]=oldlocation[howmanysprites-1];
		velocity[which]=velocity[howmanysprites-1];
		initialsize[which]=initialsize[howmanysprites-1];
		size[which]=size[howmanysprites-1];
		brightness[which]=brightness[howmanysprites-1];
		initialbrightness[which]=initialbrightness[howmanysprites-1];
		color1[which]=color1[howmanysprites-1];
		color2[which]=color2[howmanysprites-1];
		color3[which]=color3[howmanysprites-1];
		alivetime[which]=alivetime[howmanysprites-1];
		rotation[which]=rotation[howmanysprites-1];
		type[which]=type[howmanysprites-1];
		type[howmanysprites-1]=0;
		
		if(howmanysprites>0){howmanysprites--;}
	}
	
	return 0;
}

void Sprites::LoadFlareTexture(char *fileName)
{
	flaretextureptr = loadTexture(fileName);
}

void Sprites::LoadMuzzleFlareTexture(char *fileName)
{
	muzzleflaretextureptr = loadTexture(fileName);
}

void Sprites::LoadSmokeTexture(char *fileName)
{
	smoketextureptr = loadTexture(fileName);
}

void Sprites::LoadBloodTexture(char *fileName)
{
	bloodtextureptr = loadTexture(fileName);
}

void Sprites::LoadRainTexture(char *fileName)
{
	raintextureptr = loadTexture(fileName);
}

void Sprites::LoadSnowTexture(char *fileName)
{
	snowtextureptr = loadTexture(fileName);
}

void Sprites::LoadBulletTexture(char *fileName)
{
	bullettextureptr = loadTexture(fileName);
}


void Sprites::DoStuff()
{
	for(int i=0;i<howmanysprites;i++){
		if(type[i]!=bullet&&type[i]!=bulletinstant&&type[i]!=grenadesprite&&type[i]!=pinsprite&&type[i]!=grenadesprite)oldlocation[i]=location[i];
		if(type[i]==muzzleflashsprite)brightness[i]-=multiplier*10;
		if(type[i]==flashsprite)brightness[i]-=multiplier*10;
		if(type[i]==smokesprite||type[i]==smokespritenoup||type[i]==bloodspritedown||type[i]==particlesspritedown)brightness[i]-=multiplier*.5;
		if(type[i]==bloodspritenoup)brightness[i]-=multiplier*.9;
		if(type[i]==smokesprite)size[i]=initialsize[i]*abs(fast_sqrt(fast_sqrt((float)abs(initialbrightness[i]-brightness[i]))));//velocity[i].y+=multiplier*20;
		if(type[i]==bloodspritenoup||type[i]==particlesspritedown)size[i]=initialsize[i]*(initialbrightness[i]-brightness[i]+.4)*(initialbrightness[i]-brightness[i]+.4);//velocity[i].y+=multiplier*20;
		if(type[i]==bloodspritenoup||type[i]==particlesspritedown)velocity[i]=initialvelocity[i]*brightness[i];//velocity[i].y+=multiplier*20;
		if(type[i]==bullet)brightness[i]-=multiplier;
		if(type[i]==snowsprite)rotation[i]+=multiplier*50;
		if(type[i]==smokesprite)rotation[i]+=multiplier*30;
		if(type[i]==smokespritenoup)rotation[i]+=multiplier*30;
		if(type[i]==bulletinstant){
			brightness[i]=0;
		}
		if(type[i]==grenadesprite||type[i]==spoonsprite||type[i]==pinsprite){
			if(findLengthfast(velocity[i])>0){
				velocity[i].y+=gravity*multiplier;
				rotation[i]+=multiplier*2;
			}
		}
		if(type[i]==bloodspritedown||type[i]==particlesspritedown){
			velocity[i].y+=gravity*multiplier;
		}
		
		if(type[i]!=bulletinstant&&type[i]!=bullet)location[i]=location[i]+velocity[i]*multiplier;
		
		alivetime[i]+=multiplier;
		if(brightness[i]<=0)DeleteSprite(i);
		if(type[i]==snowsprite&&(location[i].y<-.1||environment!=snowy_environment))DeleteSprite(i);
		if(type[i]==rainsprite&&(location[i].y<-.1||environment!=rainy_environment))DeleteSprite(i);
		
		if(type[i]==snowsprite){
			if(location[i].x<camera.position.x-precipitationhorz){location[i].x+=precipitationhorz*2;}
			if(location[i].z<camera.position.z-precipitationhorz){location[i].z+=precipitationhorz*2;}
			if(location[i].y<camera.position.y-precipitationvert){location[i].y+=precipitationvert*2;snowdelay+=1/precipitationdensity*2;}
			if(location[i].x>camera.position.x+precipitationhorz){location[i].x-=precipitationhorz*2;}
			if(location[i].z>camera.position.z+precipitationhorz){location[i].z-=precipitationhorz*2;}
			if(location[i].y>camera.position.y+precipitationvert){location[i].y-=precipitationvert*2;}
		}
		if(type[i]==rainsprite){
			if(location[i].x<camera.position.x-precipitationhorz*.5){location[i].x+=precipitationhorz;}
			if(location[i].z<camera.position.z-precipitationhorz*.5){location[i].z+=precipitationhorz;}
			if(location[i].y<camera.position.y-precipitationvert){location[i].y+=precipitationvert*2;snowdelay+=1/precipitationdensity/4;}
			if(location[i].x>camera.position.x+precipitationhorz*.5){location[i].x-=precipitationhorz;}
			if(location[i].z>camera.position.z+precipitationhorz*.5){location[i].z-=precipitationhorz;}
			if(location[i].y>camera.position.y+precipitationvert){location[i].y-=precipitationvert*2;}
		}
	}
}

void Sprites::draw()
{
	float M[16];
	XYZ begProj,endProj,avgProj,persp;
	XYZ between;
	float oolen,dx,dy;
	float bulletsize;
	XYZ point;
	glAlphaFunc(GL_GREATER, 0.01);
	
	//glEnable(GL_POLYGON_OFFSET_FILL);
	
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDepthMask(0);
	glAlphaFunc(GL_GREATER, 0.01);
	for(int i=0;i<howmanysprites;i++){
		if(type[i]==muzzleflashsprite){fog.TempFog(0,0,0); glBindTexture(GL_TEXTURE_2D, muzzleflaretextureptr);glBlendFunc(GL_SRC_ALPHA,GL_ONE);}
		if(type[i]==flashsprite){fog.TempFog(0,0,0); glBindTexture(GL_TEXTURE_2D, flaretextureptr);glBlendFunc(GL_SRC_ALPHA,GL_ONE);}
		if(type[i]==smokesprite||type[i]==smokespritenoup){fog.ResetFog(); glBindTexture(GL_TEXTURE_2D,  smoketextureptr);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);}
		if(type[i]==bloodspritedown||type[i]==bloodspritenoup){fog.ResetFog(); glBindTexture(GL_TEXTURE_2D,  smoketextureptr);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);}
		if(type[i]==particlesspritedown){fog.ResetFog(); glBindTexture(GL_TEXTURE_2D,  bloodtextureptr);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);}
		if(type[i]==snowsprite){fog.ResetFog(); glBindTexture(GL_TEXTURE_2D,  snowtextureptr);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);}
		if(type[i]==rainsprite){fog.ResetFog(); glBindTexture(GL_TEXTURE_2D,  raintextureptr);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);}
		if(type[i]==bullet||type[i]==bulletinstant){fog.ResetFog(); glBindTexture(GL_TEXTURE_2D,  bullettextureptr);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);}
		
		glPushMatrix();
		if(type[i]==muzzleflashsprite||type[i]==flashsprite||type[i]==smokesprite||type[i]==smokespritenoup||type[i]==snowsprite||type[i]==particlesspritedown||((type[i]==bloodspritenoup||type[i]==bloodspritedown)&&blood)){
			glColor4f(color1[i]*fogcolorr*1.6,color2[i]*fogcolorg*1.6,color3[i]*fogcolorb*1.6,brightness[i]);
			glTranslatef(location[i].x,location[i].y,location[i].z);
			if(type[i]==snowsprite){
				glTranslatef(sinefluct*rotation[i]/400,0,sinefluct*rotation[i]/400);
			}
			glGetFloatv(GL_MODELVIEW_MATRIX,M);
			point.x=M[12];
			point.y=M[13];
			point.z=M[14];
			glLoadIdentity();
			glTranslatef(point.x, point.y, point.z);
			glRotatef(rotation[i],0,0,1);
			
			glBegin(GL_TRIANGLES);
					glTexCoord2f(1.0f, 1.0f); glVertex3f( .3f*size[i], .3f*size[i], 0.0f);
					glTexCoord2f(0.0f, 1.0f); glVertex3f(-.3f*size[i], .3f*size[i], 0.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex3f( .3f*size[i],-.3f*size[i], 0.0f);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(-.3f*size[i],-.3f*size[i], 0.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex3f( .3f*size[i], -.3f*size[i], 0.0f);
					glTexCoord2f(0.0f, 1.0f); glVertex3f(-.3f*size[i], .3f*size[i], 0.0f);
				glEnd();
			
		}
		
		if(type[i]==rainsprite){
			glColor4f(color1[i]*fogcolorr*1.6,color2[i]*fogcolorg*1.6,color3[i]*fogcolorb*1.6,brightness[i]);
			glTranslatef(location[i].x,location[i].y,location[i].z);
			glGetFloatv(GL_MODELVIEW_MATRIX,M);
			for( int k=0; k<3; k+=2 ) 
				for( int j=0; j<3; j++ ) {
					if ( k==j )
						M[k*4+j] = 1.0;
					else
						M[k*4+j] = 0.0;
				}
			
			
			
			glLoadMatrixf(M);
			glScalef(.2,abs(velocity[i].y)*multiplier,.2);
			glBegin(GL_TRIANGLES);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( .3f*size[i], .3f*size[i], 0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-.3f*size[i], .3f*size[i], 0.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( .3f*size[i],-.3f*size[i], 0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-.3f*size[i],-.3f*size[i], 0.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( .3f*size[i], -.3f*size[i], 0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-.3f*size[i], .3f*size[i], 0.0f);
			glEnd();
		}
		
		if(type[i]==grenadesprite||type[i]==spoonsprite||type[i]==pinsprite){
			glTranslatef(location[i].x,location[i].y+.2,location[i].z);
			glDisable(GL_BLEND);
			glEnable(GL_FOG);
			glEnable(GL_CULL_FACE);
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_LIGHTING);
			glEnable(GL_COLOR_MATERIAL);
			glDepthMask(1);
			glRotatef(rotation[i],1,.6,.3);
			
			if(type[i]==grenadesprite)gunmodels[grenadebasemodel].draw();
			if(type[i]==spoonsprite)gunmodels[grenadespoonmodel].draw();
			if(type[i]==pinsprite)gunmodels[grenadepinmodel].draw();
			
			glEnable(GL_BLEND);
			glDisable(GL_FOG);
			glDisable(GL_CULL_FACE);
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glDisable(GL_COLOR_MATERIAL);
			glDepthMask(0);
		}
		
		if(type[i]==bullet||type[i]==bulletinstant){
			bulletsize=size[i];
			glPushMatrix();
				begProj=location[i];
				endProj=velocity[i];
				glPushMatrix();
					glTranslatef(begProj.x,begProj.y,begProj.z);
					glGetFloatv(GL_MODELVIEW_MATRIX,M);
					begProj.x=M[12];
					begProj.y=M[13];
					begProj.z=M[14];
				glPopMatrix();
				glPushMatrix();
					glTranslatef(endProj.x,endProj.y,endProj.z);
					glGetFloatv(GL_MODELVIEW_MATRIX,M);
					endProj.x=M[12];
					endProj.y=M[13];
					endProj.z=M[14];
				glPopMatrix();
				avgProj=(endProj+begProj)/2;
				
				dx	= endProj.x - begProj.x;
				dy	= endProj.y - begProj.y;
				oolen= 1/fast_sqrt(dx*dx+dy*dy)*0.5;
				persp=0;
				persp.x=-dy*oolen; persp.y=dx*oolen;
				
				glColor4f(color1[i]*fogcolorr*1.6,color2[i]*fogcolorg*1.6,color3[i]*fogcolorb*1.6,brightness[i]);
				
				glPushMatrix();
					glLoadIdentity();
					glBegin(GL_TRIANGLES);
						glTexCoord2f(0.0f, 1.0f); glVertex3f(endProj.x-persp.x*bulletsize/2, endProj.y-persp.y*bulletsize/2, endProj.z);
						glTexCoord2f(1.0f, 0.0f); glVertex3f(begProj.x+persp.x*bulletsize/2, begProj.y+persp.y*bulletsize/2, begProj.z);
						glTexCoord2f(0.0f, 0.0f); glVertex3f(begProj.x-persp.x*bulletsize/2, begProj.y-persp.y*bulletsize/2, begProj.z);
						glTexCoord2f(1.0f, 0.0f); glVertex3f(begProj.x+persp.x*bulletsize/2, begProj.y+persp.y*bulletsize/2, begProj.z);
						glTexCoord2f(0.0f, 1.0f); glVertex3f(endProj.x-persp.x*bulletsize/2, endProj.y-persp.y*bulletsize/2, endProj.z);
						glTexCoord2f(1.0f, 1.0f); glVertex3f(endProj.x+persp.x*bulletsize/2, endProj.y+persp.y*bulletsize/2, endProj.z);
					glEnd();
				glPopMatrix();
			glPopMatrix();	
		}
		glPopMatrix();
	}
	fog.ResetFog(); 
	glDepthMask(1);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	//glDisable(GL_POLYGON_OFFSET_FILL);
	
}

