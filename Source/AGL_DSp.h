#pragma once

#ifndef _AGL_DSP_H_
#define _AGL_DSP_H_


/**> HEADER FILES <**/
#include <stdlib.h>			// ANSI C cross platform headers
#include <stdio.h>
#include <DrawSprocket.h>	// DrawSprocket
#include <agl.h>			// Apple's OpenGL
#include <glu.h>			// Used for setting perspective and making objects
#include <tk.h>				// Used for loading images


/**> CONSTANT DECLARATIONS <**/
#define	kMoveToFront		kFirstWindowOfClass

// Screen Dimensions
#define SCREEN_WIDTH		640
#define SCREEN_HEIGHT		480


/**> GLOBAL VARIABLES <**/
extern DSpContextAttributes		gDSpContextAttributes;	// Global DrawSprocket context attributes
extern DSpContextReference		gDSpContext;			// The global DrawSprocket context
extern AGLContext				gOpenGLContext;			// The global OpenGL (AGL) context
														// Note: These are actually defined in AGL_DSp.cpp


/**> FUNCTION PROTOTYPES <**/
void		ToolboxInit( void );
Boolean 	HasAppearance( void );
CGrafPtr	SetupScreen( int width, int height );
void		CreateWindow( CGrafPtr &theFrontBuffer, int width, int height );
void		ShutdownScreen( CGrafPtr theFrontBuffer );
AGLContext	SetupAGL( AGLDrawable window );
void		CleanupAGL( AGLContext context );


#endif