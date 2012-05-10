/*
(c) 2008 Victor "ErV" Eremin, Voronezh, Russia.
mailto: ErV2005@rambler.ru, erv@box.vsi.ru
for non-ncommercial use only
*/
#include "Textures.h"
#if __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <SDL/SDL_image.h>
#include <stdio.h>

GLuint loadTexture(const char* filename_, GLenum minFilter, GLenum magFilter, bool mipmaps){
	char filename[1024];
	strcpy(filename, filename_+1);
	while(true){
		char *c = strchr(filename, ':');
		if (!c) break;
		*c = '/';
	}
	GLuint tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

	SDL_Surface *surface = IMG_Load(filename);
	if (!surface){
		fprintf(stderr, "couldn't load file %s!\n", filename);
		return 0;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLenum format = GL_RGBA;
	int numColors = surface->format->BytesPerPixel;
	if (surface){
		switch (numColors){
			case (4):{
				if (surface->format->Rmask == 0x000000ff)
                        format = GL_RGBA;
                else
                        format = GL_BGRA;
				break;
			};
			case (3):{
				if (surface->format->Rmask == 0x000000ff)
                        format = GL_RGB;
                else
                        format = GL_BGR;
			};
		};
		
		//well, our textures are upside down. Fixing it here.
		Uint32 bytesPerRow = surface->format->BytesPerPixel*surface->w;
		char * buf = new char[bytesPerRow];
		char* p = (char*)surface->pixels;
		for (Uint32 i = 0; i < surface->h/2; i++){
			Uint32 offset1 = i*bytesPerRow;
			Uint32 offset2 = (surface->h - i - 1)*bytesPerRow;
			memcpy(buf, &p[offset1], bytesPerRow);
			memcpy(&p[offset1], &p[offset2], bytesPerRow);
			memcpy(&p[offset2], buf, bytesPerRow);
		}
		delete[] buf;
		
		glTexImage2D(GL_TEXTURE_2D, 0, numColors, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
		if (mipmaps)
		    gluBuild2DMipmaps(GL_TEXTURE_2D, format,  surface->w, surface->h, format, GL_UNSIGNED_BYTE, surface->pixels);
	}
	
	delete surface;
	return tex;
}

