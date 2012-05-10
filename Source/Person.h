#ifndef _PERSON_H_
#define _PERSON_H_

/**> HEADER FILES <**/
#include <cmath>
#ifdef OS9 
#include <gl.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
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
#include "Quaternions.h"
#include "Constants.h"
#include "Skeleton.h"
#include "Models.h"
#include "Camera.h"
#include "Sprites.h"

#define playertype 0
#define civiliantype 1
#define eviltype 2
#define viptype 3
#define evilsnipertype 4
#define evilassaultrifletype 5
#define zombietype 6

class HitStruct
{
	public:
		Joint *joint1;
		Joint *joint2;
		XYZ hitlocation;
		bool collision;
};

class Person
{
	public:
		int eaten;
		int currentframe;
		int targetframe;
		int currentanimation;
		int targetanimation;
		float target;
		float playerhealth;
		int modelnum;
		XYZ oldplayercoords;
		XYZ playercoords;
		XYZ playervelocity;
		float playerrotation;
		float playerrotation2;
		float playerlowrotation;
		float pathcheckdelay;
		bool onground;
		bool backwardsanim;
		XYZ facing;
		XYZ velocity;
		float shotdelay;
		bool litup;
		
		bool existing;
		
		int type;
		
		int whichcostume;
		
		Skeleton skeleton;
		Animation tempanimation;
		
		bool freshshootkey;
		bool freshkickkey;
		int bufferattack;
		int jump_key;
		int left_key;
		int right_key;
		int duck_key;
		int shoot_key;
		int kick_key;
		int block_key;
		
		float speed;
		bool aiming;
		int grenphase;
		float grenamount;
		float aimamount;
		float speedmult;
		float pathsize;
		
		int pathnum;
		int oldpathnum;
		int oldoldpathnum;
		int oldoldoldpathnum;
		XYZ pathtarget;
		int whichblockx;
		int whichblocky;
		
		bool dead;
		
		XYZ averageloc;
		XYZ oldaverageloc;
		
		float lastdistancevictim;
		
		bool firstlongdead;
		float longdead;
		
		Joint *bjoint1,*bjoint2;
		float bleeding;
		float bleeddelay;
		
		float attacktarget;
		int attackframe;
		int killtarget;
		bool killtargetvisible;
		
		float gunrotate1, gunrotate2, gunrotate3;
		float recoil;
		int whichgun;
		float oldhealth;
		float health;
		float maxhealth;
		float reloading;
		int ammo;
		int reloads[10];
		bool running;
		bool onpath;
		
		void FindRotationGun(XYZ start, XYZ target);
		
		int DrawSkeleton(int who);
		void DoStuff(int who);
		void DoAnimations(int who);	
		void DoAnimationslite(int who);	
		HitStruct BulletCollideWithPlayer(int who, XYZ start, XYZ end);		
};

class Costume
{
	public:
		float headcolor[3];
		float handcolor[3];
		float footcolor[3];
		float upperarmcolor[3];
		float lowerarmcolor[3];
		float upperlegcolor[3];
		float lowerlegcolor[3];
		float abdomencolor[3];
		float chestcolor[3];
};


#endif
