#ifndef _SPRITE_H_
#define _SPRITE_H_

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
#include "Quaternions.h"
#include "Camera.h"
#include "Models.h"
#include "Fog.h"
//
// Model Structures
//

#define maxsprites 2000

#define muzzleflashsprite 1
#define smokesprite 2
#define smokespritenoup 3
#define flashsprite 6
#define grenadesprite 7
#define pinsprite 8
#define spoonsprite 9
#define bloodspritedown 10
#define bloodspritenoup 11
#define particlesspritedown 12
#define snowsprite 13
#define rainsprite 14

#define bullet 4
#define bulletinstant 5

class Sprites{
	public:
				GLuint 				flaretextureptr;
				GLuint 				muzzleflaretextureptr;
				GLuint 				smoketextureptr;
				GLuint 				bullettextureptr;
				GLuint 				bloodtextureptr;
				GLuint 				raintextureptr;
				GLuint 				snowtextureptr;
				
				int howmanysprites;
				
				XYZ location[maxsprites];
				XYZ oldlocation[maxsprites];
				XYZ velocity[maxsprites];
				XYZ initialvelocity[maxsprites];
				float size[maxsprites];
				float initialsize[maxsprites];
				float brightness[maxsprites];
				float initialbrightness[maxsprites];
				float color1[maxsprites];
				float color2[maxsprites];
				float color3[maxsprites];
				float alivetime[maxsprites];
				float rotation[maxsprites];
				int type[maxsprites];
				int owner[maxsprites];
				
				void draw();
				
				int DeleteSprite(int which);
				int MakeSprite(int atype, float abrightness, float acolor1, float acolor2, float acolor3, XYZ alocation, XYZ avelocity, float asize);
				int MakeSprite(int atype, float abrightness, float acolor1, float acolor2, float acolor3, XYZ alocation, XYZ avelocity, float asize, int aowner);
				
				void DoStuff();
				void LoadMuzzleFlareTexture(char *fileName);
				void LoadFlareTexture(char *fileName);
				void LoadSmokeTexture(char *fileName);
				void LoadBulletTexture(char *fileName);
				void LoadBloodTexture(char *fileName);
				void LoadSnowTexture(char *fileName);
				void LoadRainTexture(char *fileName);
				
				~Sprites() {
					glDeleteTextures( 1, (const GLuint *)muzzleflaretextureptr );
					glDeleteTextures( 1, (const GLuint *)flaretextureptr );
					glDeleteTextures( 1, (const GLuint *)bullettextureptr );
					glDeleteTextures( 1, (const GLuint *)smoketextureptr );
					glDeleteTextures( 1, (const GLuint *)bloodtextureptr );
					glDeleteTextures( 1, (const GLuint *)raintextureptr );
					glDeleteTextures( 1, (const GLuint *)snowtextureptr );
				};
};

#endif

