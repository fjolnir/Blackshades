#ifndef _SKELETON_H_
#define _SKELETON_H_

/**> HEADER FILES <**/
#ifdef OS9 
#include <gl.h>
#include "alut.h"
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenAL/al.h>
#include <AL/alut.h>
#else
#include <GL/gl.h>
#include <AL/al.h>
#include <AL/alut.h>
#endif
#include "Quaternions.h"
#include "Constants.h"
#include "Files.h"
#include "Models.h"
#include "Camera.h"

#define boneconnect 0
#define constraint 1
#define muscle 2

//head, neck, left shoulder, left elbow, left wrist, left hand
		//right shoulder, right elbow, right wrist, right hand,
		//middle, left hip, right hip,groin
		//left knee,left ankle, left foot, right knee, right ankle, right foort
		
#define head 1
#define neck 2
#define leftshoulder 3
#define leftelbow 4
#define leftwrist 5
#define lefthand 6
#define rightshoulder 7
#define rightelbow 8
#define rightwrist 9
#define righthand 10
#define abdomen 11
#define lefthip 12
#define righthip 13
#define groin 14
#define leftknee 15
#define leftankle 16
#define leftfoot 17
#define rightknee 18
#define rightankle 19
#define rightfoot 20

class Joint
{
	public:
		XYZ position;
		XYZ oldposition;
		XYZ realoldposition;
		XYZ velocity;
		XYZ offset;
		float blurred;
		float length;
		float mass;
		bool lower;
		bool hasparent;
		bool locked;
		int modelnum;
		bool visible;
		bool existing;
		Joint* parent;
		int label;
		int hasgun;
		float rotate1,rotate2,rotate3;
		
		void DoConstraint();
};

class Muscle
{
	public:
		float length;
		float targetlength;
		Joint* parent1;
		Joint* parent2;
		float maxlength;
		float minlength;
		int type;
		bool visible;
		void DoConstraint(int broken);
		float rotate1,rotate2,rotate3;
		
		float strength;
};

class Animation
{
	public:
		Files files;
		int numframes;
		bool canbeoverridden;
		bool ismodified[max_joints][max_frames];
		XYZ  position[max_joints][max_frames];
		float twist[max_joints][max_frames];
		float twist2[max_joints][max_frames];
		float speed[max_frames];
		float gunrotation[max_frames];
		bool onground[max_joints][max_frames];
		XYZ forward[max_frames];
		float rotate1[max_joints][max_frames],rotate2[max_joints][max_frames],rotate3[max_joints][max_frames];
		float mrotate1[max_joints][max_frames],mrotate2[max_joints][max_frames],mrotate3[max_joints][max_frames];
		void Load(char *fileName);
		void Load(char *fileName,float rotate);
};


class Skeleton
{
	public:
		int num_joints;
		Joint joints[max_joints];
		int jointlabels[max_joints];
		
		int num_muscles;
		Muscle muscles[max_muscles];
	
		int selected;
	
		int forwardjoints[3];
		XYZ forward;
	
		int lowforwardjoints[3];
		XYZ lowforward;
		
		int broken;
		bool offset;
		
		XYZ specialforward[5];
		
		bool free;
		
		Files files;
	
		void DoConstraints();
		void DoConstraints(Model *collide, XYZ *move, float rotation);
		void DoGravity();
		void DoBalance();
		void MusclesSet();
		void Draw(int muscleview);
		void AddJoint(float x, float y, float z, int which);
		void SetJoint(float x, float y, float z, int which, int whichjoint);
		void DeleteJoint(int whichjoint);
		void AddMuscle(int attach1,int attach2,float maxlength,float minlength,int type);
		void DeleteMuscle(int whichmuscle);
		void FindRotationJoint(int which);
		void FindRotationMuscle(int which);
		void Load(char *fileName);
};

#endif

