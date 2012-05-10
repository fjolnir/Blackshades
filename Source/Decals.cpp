#include "Decals.h"
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

int Decals::MakeDecal(int atype, XYZ location, float size, XYZ normal, int poly, Model *model, XYZ move, float rotation){
	int major=0;
	float normalv[3];
	XYZ right;
	XYZ up;
	XYZ nothing;
	XYZ axis[3];
	XYZ temp;
	
	nothing=0;
	
	axis[0].x=1;
	axis[1].y=1;
	axis[2].z=1;
	
	normalv[0]=abs(normal.x);
	normalv[1]=abs(normal.y);
	normalv[2]=abs(normal.z);
	
	
	if(normalv[1]>normalv[major])major=1;
	if(normalv[2]>normalv[major])major=2;
	
	if (normalv[0] == 1 || normalv[1] == 1 || normalv[2] == 1)
	{
	if ((major == 0 && normal.x > 0) || major == 1){
		right=0;
		right.z=-1;}
	else if (major == 0){
		right=0;
		right.z=1;}
    else {
        right=0;
		right.x=normal.z;}
   	}
   	else
    	CrossProduct(axis[major], normal, &right);
   
    CrossProduct(normal, right, &up);
    Normalise(&up); 
    Normalise(&right);
	
	float count;
	float count2;
	float countinc=1/size;
	if(countinc<.01)countinc=.01;
	if(countinc>.2)countinc=.2;
	float normaloffset=.02;
	int good;
	
	numpoints[howmanydecals]=0;
    points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing - right - up) * (size/3) /*+ normal/100*/;
    texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = 0;
    texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = 0;
    if((move.x==0&&move.z==0&&rotation==0)||
    LineFacetd(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25, model->vertex[model->Triangles[poly].vertex[0]], model->vertex[model->Triangles[poly].vertex[1]], model->vertex[model->Triangles[poly].vertex[2]],normal,&temp)
    )numpoints[howmanydecals]++;
    else {
    	good=-1;
    	count=1-countinc;
    	while(good==-1&&count>-1){
    		texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = 0;
    		texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = .5-count/2;
    		points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing - right - up*count) * (size/3);
    		count-=countinc;
    		good=model->LineCheck2(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25,&temp,move,rotation);
    	}
    	if(good!=-1)numpoints[howmanydecals]++;
    	good=-1;
    	count=1-countinc;
    	while(good==-1&&count>-1){
    		texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = .5-count/2;
    		texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = 0;
    		points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing - right*count - up) * (size/3);
    		count-=countinc;
    		good=model->LineCheck2(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25,&temp,move,rotation);
    	}
    	if(good!=-1)numpoints[howmanydecals]++;
    	if(good==-1){
    		good=-1;
	    	count2=1-countinc;
	    	while(good==-1&&count2>-1){
	    		count=1-countinc;
		    	while(good==-1&&count>-1){
		    		texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = .5-count2/2;
		    		texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = .5-count/2;
		    		points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing - right*count2 - up*count) * (size/3);
		    		count-=countinc;
		    		good=model->LineCheck2(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25,&temp,move,rotation);
		    	}
		    	count2-=countinc;
		    }
	    	if(good!=-1)numpoints[howmanydecals]++;
    	}
    }
    
    points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing + right - up) * (size/3) /*+ normal/100*/;
    texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = 1;
    texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = 0;
    if((move.x==0&&move.y==0&&move.z==0&&rotation==0)||
    LineFacetd(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25, model->vertex[model->Triangles[poly].vertex[0]], model->vertex[model->Triangles[poly].vertex[1]], model->vertex[model->Triangles[poly].vertex[2]],normal,&temp)
    )numpoints[howmanydecals]++;
    else {
    	good=-1;
    	count=1-countinc;
    	while(good==-1&&count>-1){
    		texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = .5+count/2;
    		texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = 0;
    		points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing + right*count - up) * (size/3);
    		count-=countinc;
    		good=model->LineCheck2(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25,&temp,move,rotation);
    	}
    	if(good!=-1)numpoints[howmanydecals]++;
    	good=-1;
    	count=1-countinc;
    	while(good==-1&&count>-1){
    		texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = 1;
    		texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = .5-count/2;
    		points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing + right - up*count) * (size/3);
    		count-=countinc;
    		good=model->LineCheck2(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25,&temp,move,rotation);
    	}
    	if(good!=-1)numpoints[howmanydecals]++;
    	if(good==-1){
    		good=-1;
	    	count2=1-countinc;
	    	while(good==-1&&count2>-1){
	    		count=1-countinc;
		    	while(good==-1&&count>-1){
		    		texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = .5+count2/2;
		    		texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = .5-count/2;
		    		points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing + right*count2 - up*count) * (size/3);
		    		count-=countinc;
		    		good=model->LineCheck2(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25,&temp,move,rotation);
		    	}
		    	count2-=countinc;
		    }
	    	if(good!=-1)numpoints[howmanydecals]++;
    	}
    }
    
    points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing + right + up) * (size/3) /*+ normal/100*/;
    texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = 1;
    texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = 1;
    if((move.x==0&&move.y==0&&move.z==0&&rotation==0)||
    LineFacetd(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25, model->vertex[model->Triangles[poly].vertex[0]], model->vertex[model->Triangles[poly].vertex[1]], model->vertex[model->Triangles[poly].vertex[2]],normal,&temp)
    )numpoints[howmanydecals]++;
    else {
    	good=-1;
    	count=1-countinc;
    	while(good==-1&&count>-1){
    		texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = 1;
    		texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = .5+count/2;
    		points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing + right + up*count) * (size/3);
    		count-=countinc;
    		good=model->LineCheck2(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25,&temp,move,rotation);
    	}
    	if(good!=-1)numpoints[howmanydecals]++;
    	good=-1;
    	count=1-countinc;
    	while(good==-1&&count>-1){
    		texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = .5+count/2;
    		texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = 1;
    		points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing + right*count + up) * (size/3);
    		count-=countinc;
    		good=model->LineCheck2(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25,&temp,move,rotation);
    	}
    	if(good!=-1)numpoints[howmanydecals]++;
    	if(good==-1){
    		good=-1;
	    	count2=1-countinc;
	    	while(good==-1&&count2>-1){
	    		count=1-countinc;
		    	while(good==-1&&count>-1){
		    		texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = .5+count2/2;
		    		texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = .5+count/2;
		    		points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing + right*count2 + up*count) * (size/3);
		    		count-=countinc;
		    		good=model->LineCheck2(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25,&temp,move,rotation);
		    	}
		    	count2-=countinc;
		    }
	    	if(good!=-1)numpoints[howmanydecals]++;
    	}
    }
    
    points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing - right + up) * (size/3) /*+ normal/100*/;
	texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = 0;
    texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = 1;
    if((move.x==0&&move.y==0&&move.z==0&&rotation==0)||
    LineFacetd(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25, model->vertex[model->Triangles[poly].vertex[0]], model->vertex[model->Triangles[poly].vertex[1]], model->vertex[model->Triangles[poly].vertex[2]],normal,&temp)
    )numpoints[howmanydecals]++;
    else {
    	good=-1;
    	count=1-countinc;
    	while(good==-1&&count>-1){
    		texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = .5-count/2;
    		texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = 1;
    		points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing - right*count + up) * (size/3);
    		count-=countinc;
    		good=model->LineCheck2(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25,&temp,move,rotation);
    	}
    	if(good!=-1)numpoints[howmanydecals]++;
    	good=-1;
    	count=1-countinc;
    	while(good==-1&&count>-1){
    		texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = 0;
    		texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = .5+count/2;
    		points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing - right + up*count) * (size/3);
    		count-=countinc;
    		good=model->LineCheck2(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25,&temp,move,rotation);
    	}
    	if(good!=-1)numpoints[howmanydecals]++;
    	if(good==-1){
    		good=-1;
	    	count2=1-countinc;
	    	while(good==-1&&count2>-1){
	    		count=1-countinc;
		    	while(good==-1&&count>-1){
		    		texcoordsx[howmanydecals*8+numpoints[howmanydecals]] = .5-count2/2;
		    		texcoordsy[howmanydecals*8+numpoints[howmanydecals]] = .5+count/2;
		    		points[howmanydecals*8+numpoints[howmanydecals]] = location + (nothing - right*count2 + up*count) * (size/3);
		    		count-=countinc;
		    		good=model->LineCheck2(points[howmanydecals*8+numpoints[howmanydecals]]+normal/25,points[howmanydecals*8+numpoints[howmanydecals]]-normal/25,&temp,move,rotation);
		    	}
		    	count2-=countinc;
		    }
	    	if(good!=-1)numpoints[howmanydecals]++;
    	}
    }
    for(int i=0;i<numpoints[howmanydecals];i++){
    	 points[howmanydecals*8+i] += normal*normaloffset;
    }
    
    type[howmanydecals]=atype;
	alivetime[howmanydecals]=0;
	if(howmanydecals<maxdecals){howmanydecals++;}

	return 0;
}

int Decals::DeleteDecal(int which){
	if(which>=0){
		numpoints[which]=numpoints[howmanydecals-1];
		alivetime[which]=alivetime[howmanydecals-1];
		type[which]=type[howmanydecals-1];
		for(int i=0;i<numpoints[which];i++){
			points[which*8+i] = points[howmanydecals*8-8+i];
		    texcoordsx[which*8+i] = texcoordsx[howmanydecals*8-8+i];
		    texcoordsy[which*8+i] = texcoordsy[howmanydecals*8-8+i];
	    }
		if(howmanydecals>0){howmanydecals--;}
	}
	
	return 0;
}

void Decals::LoadBulletHoleTexture(char *fileName)
{
	bulletholetextureptr = loadTexture(fileName);
}

void Decals::LoadBloodTexture(char *fileName, int which)
{
	bloodtextureptr[which] = loadTexture(fileName);
}

void Decals::LoadCraterTexture(char *fileName)
{
	cratertextureptr = loadTexture(fileName);
}

void Decals::DoStuff()
{
	for(int i=0;i<howmanydecals;i++){
		alivetime[i]+=multiplier;
		if(alivetime[i]>10&&(type[i]==bullethole||type[i]==crater))DeleteDecal(i);
		if(alivetime[i]>20&&(type[i]==bloodpool))DeleteDecal(i);
	}
}

void Decals::draw()
{
	glAlphaFunc(GL_GREATER, 0.01);
	
	float bloodpoolspeed=1;
	
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glDepthMask(0);
	glAlphaFunc(GL_GREATER, 0.01);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POLYGON_OFFSET_FILL);
	for(int i=0;i<howmanydecals;i++){
		if(type[i]==bullethole)glBindTexture(GL_TEXTURE_2D, bulletholetextureptr);
		if(type[i]==crater)glBindTexture(GL_TEXTURE_2D, cratertextureptr);
		if(type[i]!=bloodpool)glColor4f(1,1,1,10-alivetime[i]);
		
		if(type[i]==bloodpool&&alivetime[i]<bloodpoolspeed*.2)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[0]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*.2&&alivetime[i]<bloodpoolspeed*.4)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[1]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*.4&&alivetime[i]<bloodpoolspeed*.6)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[2]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*.6&&alivetime[i]<bloodpoolspeed*.8)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[3]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*.8&&alivetime[i]<bloodpoolspeed*1)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[4]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*1&&alivetime[i]<bloodpoolspeed*1.2)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[5]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*1.2&&alivetime[i]<bloodpoolspeed*1.4)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[6]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*1.4&&alivetime[i]<bloodpoolspeed*1.6)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[7]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*1.6&&alivetime[i]<bloodpoolspeed*1.8)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[8]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*1.8&&alivetime[i]<bloodpoolspeed*2.0)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[9]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*2.0)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[10]);
		if(type[i]==bloodpool&&alivetime[i]<bloodpoolspeed*2.0)glColor4f(1,1,1,1.5-(alivetime[i]*5/bloodpoolspeed-(int)(alivetime[i]*5/bloodpoolspeed)));
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*2.0)glColor4f(1,1,1,20-alivetime[i]);
		
		glPushMatrix();
		glBegin(GL_TRIANGLE_FAN);
			for(int j=0;j<numpoints[i];j++){
			 glTexCoord2f(texcoordsx[i*8+j], texcoordsy[i*8+j]); glVertex3f(points[i*8+j].x,points[i*8+j].y,points[i*8+j].z);
			}
		glEnd();
		glPopMatrix();
		
		if(type[i]==bloodpool&&alivetime[i]<bloodpoolspeed*2.0){
			if(type[i]==bloodpool&&alivetime[i]<bloodpoolspeed*.2)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[1]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*.2&&alivetime[i]<bloodpoolspeed*.4)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[2]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*.4&&alivetime[i]<bloodpoolspeed*.6)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[3]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*.6&&alivetime[i]<bloodpoolspeed*.8)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[4]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*.8&&alivetime[i]<bloodpoolspeed*1)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[5]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*1&&alivetime[i]<bloodpoolspeed*1.2)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[6]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*1.2&&alivetime[i]<bloodpoolspeed*1.4)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[7]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*1.4&&alivetime[i]<bloodpoolspeed*1.6)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[8]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*1.6&&alivetime[i]<bloodpoolspeed*1.8)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[9]);
		if(type[i]==bloodpool&&alivetime[i]>=bloodpoolspeed*1.8&&alivetime[i]<bloodpoolspeed*2.0)glBindTexture(GL_TEXTURE_2D, bloodtextureptr[10]);
		if(type[i]==bloodpool)glColor4f(1,1,1,alivetime[i]*5/bloodpoolspeed-(int)(alivetime[i]*5/bloodpoolspeed));
			
			glPushMatrix();
			glBegin(GL_TRIANGLE_FAN);
				for(int j=0;j<numpoints[i];j++){
				 glTexCoord2f(texcoordsx[i*8+j], texcoordsy[i*8+j]); glVertex3f(points[i*8+j].x,points[i*8+j].y,points[i*8+j].z);
				}
			glEnd();
			glPopMatrix();
		}
	}
	glDepthMask(1);
	glDisable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);
	glEnable(GL_CULL_FACE);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDepthFunc(GL_LEQUAL);
}

