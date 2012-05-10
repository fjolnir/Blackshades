/**> HEADER FILES <**/
#include "Fog.h"

void Fog::SetFog(float colorR, float colorG, float colorB, float fStart, float fEnd, float Density)
{
	fogColor[0]=colorR;
	fogColor[1]=colorG;
	fogColor[2]=colorB;
	fogColor[3]=1;
	fogStart=fStart;
	fogEnd=fEnd;
	fogDensity=Density;
	fogMode=GL_LINEAR;

	glFogi(GL_FOG_MODE,fogMode);
	glFogfv(GL_FOG_COLOR,fogColor);
	glFogf(GL_FOG_DENSITY,fogDensity);
	glFogi(GL_FOG_HINT,GL_DONT_CARE);
	glFogi(GL_FOG_START,fogStart);
	glFogi(GL_FOG_END,fogEnd);
	
	glEnable(GL_FOG);
}

void Fog::TempFog(float colorR, float colorG, float colorB)
{
	GLfloat tempfogColor[4];
	tempfogColor[0]=colorR;
	tempfogColor[1]=colorG;
	tempfogColor[2]=colorB;
	tempfogColor[3]=1;

	glFogi(GL_FOG_MODE,fogMode);
	glFogfv(GL_FOG_COLOR,tempfogColor);
	glFogf(GL_FOG_DENSITY,fogDensity);
	glFogi(GL_FOG_HINT,GL_DONT_CARE);
	glFogi(GL_FOG_START,fogStart);
	glFogi(GL_FOG_END,fogEnd);
	
	glEnable(GL_FOG);
}

void Fog::ResetFog()
{
	glFogi(GL_FOG_MODE,fogMode);
	glFogfv(GL_FOG_COLOR,fogColor);
	glFogf(GL_FOG_DENSITY,fogDensity);
	glFogi(GL_FOG_HINT,GL_DONT_CARE);
	glFogi(GL_FOG_START,fogStart);
	glFogi(GL_FOG_END,fogEnd);
	
	glEnable(GL_FOG);
}

