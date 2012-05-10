#ifndef _DECALS_H_
#define _DECALS_H_

#include "Quaternions.h"
#ifdef OS9 
#include <gl.h>
#include <glu.h>
#include "glut.h"
#else
///ZWARNING
///#include <GL/glut.h>
///GLUT REMOVED.
#endif
#include "Files.h"
#include "Quaternions.h"
#include "Camera.h"
#include "Models.h"
#include "Fog.h"
//
// Model Structures
//

#define maxdecals 120

class Decals{
	public:
				GLuint 				bulletholetextureptr;
				GLuint 				cratertextureptr;
				GLuint 				bloodtextureptr[11];
				
				int howmanydecals;
				
				int type[maxdecals];
				
				XYZ points[8*maxdecals];
				int numpoints[maxdecals];
				float texcoordsx[8*maxdecals];
				float texcoordsy[8*maxdecals];
				float alivetime[maxdecals];
				
				void draw();
				
				int DeleteDecal(int which);
				int MakeDecal(int atype, XYZ location, float size, XYZ normal, int poly, Model *model, XYZ move, float rotation);
				
				void DoStuff();
				void LoadBulletHoleTexture(char *fileName);
				void LoadCraterTexture(char *fileName);
				void LoadBloodTexture(char *fileName, int which);
				
				~Decals() {
					glDeleteTextures( 1, (const GLuint *)bulletholetextureptr );
					glDeleteTextures( 1, (const GLuint *)cratertextureptr );
					glDeleteTextures( 11, (const GLuint *)bloodtextureptr );
				};
};

#endif

