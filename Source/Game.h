#ifndef _GAME_H_
#define _GAME_H_

#ifdef OS9 
#include <gl.h>
#include <glu.h>
#include <tk.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <stdlib.h>			
#include <stdio.h>			
#include <string.h>
#include <ctype.h>
#ifdef OS9 
#include <Sound.h>
#include <Resources.h>		
#endif
#include <cstdarg>
#ifdef OS9 
#include <glm.h>
#include <TextUtils.h>
#endif
#ifndef OS9 
#include <SDL/SDL.h>
#endif
#ifdef OS9 
#include "alut.h"
#elif __APPLE__
#include <OpenAL/al.h>
#include <AL/alut.h>
#else
#include <AL/al.h>
#include <AL/alut.h>
#endif
#include "Timer.h"	
#ifdef OS9 
#include "AGL_DSp.h"	
#endif
#include "MacInput.h"
#include "Quaternions.h"
#include "Camera.h"
#include "Skeleton.h"
#include "Files.h"
#include "Models.h"
#include "Text.h"
#include "Fog.h"
#include "Frustum.h"
#include "Sprites.h"
#include "Person.h"
#include "Decals.h"

#define num_blocks 100
#define block_spacing 360
#define max_people 90
#define max_people_block 20


class Game			 							
{
	public:
		//Eventloop
  		Boolean	gQuit;
		float gamespeed;
		double multiplier2,multiplier3,multiplier4,multiplier5,end,start,timetaken,framespersecond;
		timer theTimer;	
		float sps;
		int maxfps;
#ifdef OS9 
		AGLContext gOpenGLContext;
		CGrafPtr	theScreen;
#endif
		//Graphics
		int screenwidth,screenheight;
		float viewdistance;
		
		//GL functions
		GLvoid ReSizeGLScene(float fov, float near);
		int DrawGLScene(void);
		int InitGL(void);
		void LoadingScreen(float percent);
		
		//Game Functions
		void	HandleKeyDown( char theChar );
#ifdef OS9 
		void	DoEvent( EventRecord *event );
#endif
		void	EventLoop( void );
		void 	Tick();
		void 	Splat(int k);
		void 	InitGame();
		void 	Dispose();
		
		//Mouse
		Point mouseloc;
		Point oldmouseloc;

		float mouserotation,mouserotation2;
		float oldmouserotation,oldmouserotation2;
		float mousesensitivity;
		float usermousesensitivity;
		
		//keyboard
		
		bool tabkeydown;
		
		//Project Specific
		int cityrotation[num_blocks][num_blocks];
		int citytype[num_blocks][num_blocks];
		int citypeoplenum[num_blocks][num_blocks];
		bool drawn[num_blocks][num_blocks];
		int onblockx,onblocky;
		bool cubetest;
		bool disttest;
		bool oldbutton;
		
		bool initialized;
		
		float flashamount;
		float flashr,flashg,flashb;
		
		int enemystate;
		
		int cycle;
		
		bool whacked;
		
		float losedelay;
		
		XYZ bodycoords;
		
		FRUSTUM frustum;
		Model blocks[4];
		Model blockwalls[4];
		Model blockcollide[4];
		Model blocksimplecollide[4];
		Model blockroofs[4];
		Model blockocclude;
		Model sidewalkcollide;
		Model street;
		Model Bigstreet;
		Model path;
		Model blocksimple;
		XYZ boundingpoints[8];
		Files files;
		Text text;
		int goodkills;
		int badkills;
		int civkills;
		int machinegunsoundloop;
		
		bool lasersight;
		bool debug;
		bool vblsync;
		
		bool blur;
		bool blurness;
		
		bool paused;
		
		int mainmenu;
		
		bool reloadtoggle;
		
		bool aimtoggle;

		Point olddrawmouse;
	
		XYZ vipgoal;
		
		XYZ aimer[2];
		
		double eqn[4];
		
		float oldrot,oldrot2;
		
		XYZ lastshot[2];
		bool zoom;
		bool oldzoom;
		
		int numpeople;
		float spawndelay;
		
		bool customlevels;
		
		bool musictoggle;
		
		float psychicpower;
		
		int type;
	
		bool slomokeydown;
		
		int mouseoverbutton;
		int oldmouseoverbutton;
		
		Person person[max_people];
		
		GLuint 				personspritetextureptr;
		GLuint 				deadpersonspritetextureptr;
		GLuint 				scopetextureptr;
		GLuint 				flaretextureptr;
		
		bool killedinnocent;
		bool gameinprogress;
		bool beatgame;
		bool mainmenuness;
		int murderer;
		float timeremaining;
		int whichsong;
		int oldscore;
		int highscore;
		int score;
		int mission;
		int nummissions;
		int numpossibleguns;
		int possiblegun[6];
		int evilprobability;
		float difficulty;
		bool azertykeyboard;
		bool oldvisionkey;
		
		~Game() {
			glDeleteTextures( 1, (const GLuint *)personspritetextureptr );
			glDeleteTextures( 1, (const GLuint *)deadpersonspritetextureptr );
			glDeleteTextures( 1, (const GLuint *)scopetextureptr );
			glDeleteTextures( 1, (const GLuint *)flaretextureptr );
		}
		
};

#endif
