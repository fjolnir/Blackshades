/**> HEADER FILES <**/
#include "Skeleton.h"

#include "Serialize.h"

extern double multiplier;
extern unsigned int gSourceID[100];
extern unsigned int gSampleSet[100];
extern int visions;
extern float rad2deg;
extern Camera camera;
extern float soundscalefactor;

extern XYZ vel;
extern XYZ midp;
extern XYZ newpoint1,newpoint2;

extern float oldlength;
extern float relaxlength;

extern float friction;
extern int numrepeats;
extern float groundlevel;
extern float offset;
extern XYZ impact;
extern XYZ overpoint;
extern XYZ underpoint;
extern int whichtri;
extern XYZ normalrotated;
extern bool groundish;


void Joint::DoConstraint()
{
	
	if(hasparent){
		//Find midpoint
		midp=(position+parent->position)/2;
		//Find vector from midpoint to second vector
		vel=parent->position-midp;
		//Change to unit vector
		Normalise(&vel);
		//Apply velocity change
		velocity+=((midp-vel*length/2)-position);
		parent->velocity+=((midp+vel*length/2)-parent->position);
		//Move child point to within certain distance of parent point
		if(!locked)position=midp-vel*length/2;
		if(!parent->locked)parent->position=midp+vel*length/2;
	}
}

void Muscle::DoConstraint(int broken)
{
	oldlength=length;
	relaxlength=findDistance(parent1->position,parent2->position);
	
	if(type==boneconnect)strength=1;
	if(type==constraint)strength=0;
	
	if(strength<0)strength=0;
	if(strength>1)strength=1;
	
	length-=(length-relaxlength)*(1-strength)*multiplier*multiplier*10000;
	length-=(length-targetlength)*(strength)*multiplier*multiplier*10000;
	if(strength==0)length=relaxlength;
	
	if((relaxlength-length>0&&relaxlength-oldlength<0)||(relaxlength-length<0&&relaxlength-oldlength>0))length=relaxlength;
	
	if(length<minlength)length=minlength;
	if(length>maxlength&&!broken)length=maxlength;
	
	//Find midpoint
	midp=(parent1->position+parent2->position)/2;
	//Find vector from midpoint to second vector
	vel=parent2->position-midp;
	//Change to unit vector
	Normalise(&vel);
	//Apply velocity change
	newpoint1=midp-vel*length/2;
	newpoint2=midp+vel*length/2;
	parent1->velocity+=(newpoint1-parent1->position);
	parent2->velocity+=(newpoint2-parent2->position);
	//Move child point to within certain distance of parent point
	if(!parent1->locked)parent1->position=newpoint1;
	if(!parent2->locked)parent2->position=newpoint2;
}

void Skeleton::DoConstraints()
{
	numrepeats=3;
	
	for(int j=0; j<numrepeats; j++){
		for(int i=0; i<num_joints; i++){
			joints[i].DoConstraint();
		}
	}
}

void Skeleton::DoConstraints(Model *collide, XYZ *move, float rotation)
{
	friction=20;
	numrepeats=2;
	groundlevel=0;
	
	move->y+=.35;
	
	for(int j=0; j<numrepeats; j++){
		for(int i=0; i<num_joints; i++){
			if(joints[i].existing||i==jointlabels[lefthand]||i==jointlabels[righthand]){
			//Length constraints
			joints[i].DoConstraint();
			//Ground constraint
			offset=0;
			overpoint=joints[i].position;
			overpoint.y+=10;
			underpoint=joints[i].position;
			underpoint.y-=offset;
			whichtri=collide->LineCheck2(overpoint,underpoint,&impact,*move,rotation);
			if(collide->normals[whichtri].y<=.8)whichtri=collide->LineCheck2(joints[i].realoldposition,joints[i].position,&impact,*move,rotation);
			if(joints[i].position.y<=groundlevel+offset||whichtri!=-1){
				if(whichtri==-1||(whichtri!=-1&&collide->normals[whichtri].y>.8)){
					if(whichtri==-1)joints[i].position.y=groundlevel+offset;
					if(whichtri!=-1){
						joints[i].position=impact;
						joints[i].position.y+=offset;
					}
					joints[i].velocity.y*=-.3;
					joints[i].velocity.x*=.3;
					joints[i].velocity.z*=.3;
				}
				offset=.2;
				if(whichtri!=-1&&collide->normals[whichtri].y<=.8){
					normalrotated=DoRotation(collide->normals[whichtri],0,rotation,0);
					joints[i].position=impact+normalrotated*offset;
					ReflectVector(&joints[i].velocity,&normalrotated);
					joints[i].velocity*=.3;
				}
				if(broken<=1){
				XYZ avgvelocity;
				avgvelocity=0;
				float gLoc[3];
				ALint tempint;
				for(int k=0; k<num_joints; k++){
					avgvelocity+=joints[k].velocity;
				}
				avgvelocity/=num_joints;
				if(joints[i].label==head&&(findLengthfast(joints[i].velocity)>2||findLengthfast(avgvelocity)>2)){
					avgvelocity+=joints[i].velocity;
					gLoc[0]=joints[i].position.x/soundscalefactor;
					gLoc[1]=joints[i].position.y/soundscalefactor;
					gLoc[2]=joints[i].position.z/soundscalefactor;
#ifdef DEBIAN_NEEDS_TO_UPDATE_THEIR_OPENAL					
					alGetSourceiv(gSourceID[headlandsound], AL_SOURCE_STATE, &tempint);
#else
					alGetSourcei(gSourceID[headlandsound], AL_SOURCE_STATE, &tempint);
#endif
					if (tempint != AL_PLAYING){
						alSourcef(gSourceID[headlandsound], AL_MIN_GAIN, ALfloat(findLengthfast(avgvelocity)*2/findDistancefast(joints[i].position,camera.position)*soundscalefactor*2));
						alSourcef(gSourceID[headlandsound], AL_MAX_GAIN, ALfloat(findLengthfast(avgvelocity)*2/findDistancefast(joints[i].position,camera.position)*soundscalefactor*2));
						alSourcefv(gSourceID[headlandsound], AL_POSITION, gLoc);
						alSourcePlay(gSourceID[headlandsound]);}
				}
				avgvelocity=0;
				for(int k=0; k<num_joints; k++){
					avgvelocity+=joints[k].velocity;
				}
				avgvelocity/=num_joints;
				if((joints[i].label==abdomen)&&(findLengthfast(joints[i].velocity)>2||findLengthfast(avgvelocity)>2)){
					avgvelocity+=joints[i].velocity;
					gLoc[0]=joints[i].position.x/soundscalefactor;
					gLoc[1]=joints[i].position.y/soundscalefactor;
					gLoc[2]=joints[i].position.z/soundscalefactor;
#ifdef DEBIAN_NEEDS_TO_UPDATE_THEIR_OPENAL
					alGetSourceiv(gSourceID[bodylandsound], AL_SOURCE_STATE, &tempint);
#else
					alGetSourcei(gSourceID[bodylandsound], AL_SOURCE_STATE, &tempint);
#endif					
					if (tempint != AL_PLAYING){
						alSourcef(gSourceID[bodylandsound], AL_MIN_GAIN, ALfloat(findLengthfast(joints[i].velocity)*1/findDistancefast(joints[i].position,camera.position)*soundscalefactor*2));
						alSourcef(gSourceID[bodylandsound], AL_MAX_GAIN, ALfloat(findLengthfast(joints[i].velocity)*1/findDistancefast(joints[i].position,camera.position)*soundscalefactor*2));
						alSourcefv(gSourceID[bodylandsound], AL_POSITION, gLoc);
						alSourcePlay(gSourceID[bodylandsound]);}
				}
				}
			}}
		}
		if(num_muscles)
		for(int i=0; i<num_muscles; i++){
			//Length constraints
			muscles[i].DoConstraint(broken);
		}
	}
	
	for(int i=0; i<num_joints; i++){
		joints[i].realoldposition=joints[i].position;
	}
	
	//Add velocity
	for(int i=0; i<num_joints; i++){
		if(joints[i].existing||i==jointlabels[lefthand]||i==jointlabels[righthand])joints[i].position=joints[i].position+joints[i].velocity*multiplier;
	}
}

void Skeleton::DoGravity()
{
	for(int i=0; i<num_joints; i++){
		joints[i].velocity.y+=gravity*multiplier;
	}
}

void Skeleton::Draw(int  muscleview)
{
	float jointcolor[4];

	if(muscleview!=2){
		jointcolor[0]=0;
		jointcolor[1]=0;
		jointcolor[2]=.5;
		jointcolor[3]=1;
	}
	
	if(muscleview==2){
		jointcolor[0]=0;
		jointcolor[1]=0;
		jointcolor[2]=0;
		jointcolor[3]=.5;
	}
	//Calc motionblur-ness
	for(int i=0; i<num_joints; i++){
		joints[i].oldposition=joints[i].position;
		joints[i].blurred=findDistance(joints[i].position,joints[i].oldposition)*100;
		if(joints[i].blurred<1)joints[i].blurred=1;
	}

	//Do Motionblur
	glDepthMask(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
		for(int i=0; i<num_joints; i++){
			if(joints[i].hasparent){
				glColor4f(jointcolor[0],jointcolor[1],jointcolor[2],jointcolor[3]/joints[i].blurred);
				glVertex3f(joints[i].position.x,joints[i].position.y,joints[i].position.z);
				glColor4f(jointcolor[0],jointcolor[1],jointcolor[2],jointcolor[3]/joints[i].parent->blurred);
				glVertex3f(joints[i].parent->position.x,joints[i].parent->position.y,joints[i].parent->position.z);
				glColor4f(jointcolor[0],jointcolor[1],jointcolor[2],jointcolor[3]/joints[i].parent->blurred);
				glVertex3f(joints[i].parent->oldposition.x,joints[i].parent->oldposition.y,joints[i].parent->oldposition.z);
				glColor4f(jointcolor[0],jointcolor[1],jointcolor[2],jointcolor[3]/joints[i].blurred);
				glVertex3f(joints[i].oldposition.x,joints[i].oldposition.y,joints[i].oldposition.z);
			}
		}
		for(int i=0; i<num_muscles; i++){
			if(muscles[i].type==boneconnect){
				glColor4f(jointcolor[0],jointcolor[1],jointcolor[2],jointcolor[3]/muscles[i].parent2->blurred);
				glVertex3f(muscles[i].parent1->position.x,muscles[i].parent1->position.y,muscles[i].parent1->position.z);
				glColor4f(jointcolor[0],jointcolor[1],jointcolor[2],jointcolor[3]/muscles[i].parent2->blurred);
				glVertex3f(muscles[i].parent2->position.x,muscles[i].parent2->position.y,muscles[i].parent2->position.z);
				glColor4f(jointcolor[0],jointcolor[1],jointcolor[2],jointcolor[3]/muscles[i].parent2->blurred);
				glVertex3f(muscles[i].parent2->oldposition.x,muscles[i].parent2->oldposition.y,muscles[i].parent2->oldposition.z);
				glColor4f(jointcolor[0],jointcolor[1],jointcolor[2],jointcolor[3]/muscles[i].parent1->blurred);
				glVertex3f(muscles[i].parent1->oldposition.x,muscles[i].parent1->oldposition.y,muscles[i].parent1->oldposition.z);
			}
		}
	glEnd();

	glBegin(GL_LINES);
		for(int i=0; i<num_joints; i++){
			if(joints[i].hasparent){
				glColor4f(jointcolor[0],jointcolor[1],jointcolor[2],jointcolor[3]/joints[i].blurred);
				glVertex3f(joints[i].position.x,joints[i].position.y,joints[i].position.z);
				glColor4f(jointcolor[0],jointcolor[1],jointcolor[2],jointcolor[3]/joints[i].parent->blurred);
				glVertex3f(joints[i].parent->position.x,joints[i].parent->position.y,joints[i].parent->position.z);
			}
		}
		for(int i=0; i<num_muscles; i++){
			if(muscles[i].type==boneconnect){
				glColor4f(jointcolor[0],jointcolor[1],jointcolor[2],jointcolor[3]/muscles[i].parent1->blurred);
				glVertex3f(muscles[i].parent1->position.x,muscles[i].parent1->position.y,muscles[i].parent1->position.z);
				glColor4f(jointcolor[0],jointcolor[1],jointcolor[2],jointcolor[3]/muscles[i].parent2->blurred);
				glVertex3f(muscles[i].parent2->position.x,muscles[i].parent2->position.y,muscles[i].parent2->position.z);
			}
		}
		glColor3f(.6,.6,0);
		if(muscleview==1)
		for(int i=0; i<num_muscles; i++){
			if(muscles[i].type!=boneconnect){
				glVertex3f(muscles[i].parent1->position.x,muscles[i].parent1->position.y,muscles[i].parent1->position.z);
				glVertex3f(muscles[i].parent2->position.x,muscles[i].parent2->position.y,muscles[i].parent2->position.z);
			}
		}	
	glEnd();
	
	if(muscleview!=2){
		glPointSize(3);
		glBegin(GL_POINTS);
			for(int i=0; i<num_joints; i++){
				if(i!=selected)glColor4f(0,0,.5,1);
				if(i==selected)glColor4f(1,1,0,1);
				if(joints[i].locked&&i!=selected)glColor4f(1,0,0,1);
				glVertex3f(joints[i].position.x,joints[i].position.y,joints[i].position.z);
			}
		glEnd();
	}
		
	//Set old position to current position
	if(muscleview==2)
	for(int i=0; i<num_joints; i++){
		joints[i].oldposition=joints[i].position;
	}
	glDepthMask(1);
}

void Skeleton::AddJoint(float x, float y, float z, int which)
{
	if(num_joints<max_joints-1){
		joints[num_joints].velocity=0;
		joints[num_joints].position.x=x;
		joints[num_joints].position.y=y;
		joints[num_joints].position.z=z;
		joints[num_joints].locked=0;
		
		if(which>=num_joints||which<0)joints[num_joints].hasparent=0;
		if(which<num_joints&&which>=0){
			joints[num_joints].parent=&joints[which];
			joints[num_joints].hasparent=1;
			joints[num_joints].length=findDistance(joints[num_joints].position,joints[num_joints].parent->position);
		}	
		num_joints++;
	}
}

void Skeleton::DeleteJoint(int whichjoint)
{
	if(whichjoint<num_joints&&whichjoint>=0){
		for(int i=0;i<num_muscles;i++){
			while(muscles[i].parent1==&joints[whichjoint]&&i<num_muscles)DeleteMuscle(i);
			while(muscles[i].parent2==&joints[whichjoint]&&i<num_muscles)DeleteMuscle(i);
		}
		for(int i=0;i<num_joints;i++){
			if(joints[i].parent==&joints[whichjoint])joints[i].hasparent=0;
		}
		joints[whichjoint].hasparent=0;
	}
}

void Skeleton::DeleteMuscle(int whichmuscle)
{
	if(whichmuscle<num_muscles){
		muscles[whichmuscle].minlength=muscles[num_muscles-1].minlength;
		muscles[whichmuscle].maxlength=muscles[num_muscles-1].maxlength;
		muscles[whichmuscle].strength=muscles[num_muscles-1].strength;
		muscles[whichmuscle].parent1=muscles[num_muscles-1].parent1;
		muscles[whichmuscle].parent2=muscles[num_muscles-1].parent2;
		muscles[whichmuscle].length=muscles[num_muscles-1].length;
		muscles[whichmuscle].visible=muscles[num_muscles-1].visible;
		muscles[whichmuscle].type=muscles[num_muscles-1].type;
		muscles[whichmuscle].targetlength=muscles[num_muscles-1].targetlength;
		
		num_muscles--;
	}
}

void Skeleton::SetJoint(float x, float y, float z, int which, int whichjoint)
{
	if(whichjoint<num_joints){
		joints[whichjoint].velocity=0;
		joints[whichjoint].position.x=x;
		joints[whichjoint].position.y=y;
		joints[whichjoint].position.z=z;
		
		if(which>=num_joints||which<0)joints[whichjoint].hasparent=0;
		if(which<num_joints&&which>=0){
			joints[whichjoint].parent=&joints[which];
			joints[whichjoint].hasparent=1;
			joints[whichjoint].length=findDistance(joints[whichjoint].position,joints[whichjoint].parent->position);
		}	
	}
}

void Skeleton::AddMuscle(int attach1,int attach2,float minlength,float maxlength,int type)
{
	if(num_muscles<max_muscles-1&&attach1<num_joints&&attach1>=0&attach2<num_joints&&attach2>=0&&attach1!=attach2){
		muscles[num_muscles].parent1=&joints[attach1];
		muscles[num_muscles].parent2=&joints[attach2];
		muscles[num_muscles].length=findDistance(muscles[num_muscles].parent1->position,muscles[num_muscles].parent2->position);
		muscles[num_muscles].targetlength=findDistance(muscles[num_muscles].parent1->position,muscles[num_muscles].parent2->position);
		muscles[num_muscles].strength=.7;
		muscles[num_muscles].type=type;
		muscles[num_muscles].minlength=minlength;
		muscles[num_muscles].maxlength=maxlength;
		
		num_muscles++;
	}
}

void Skeleton::MusclesSet()
{
	for(int i=0;i<num_muscles;i++){
		muscles[i].length=findDistance(muscles[i].parent1->position,muscles[i].parent2->position);
	}
}

void Skeleton::FindRotationJoint(int which)
{
	XYZ temppoint1,temppoint2,tempforward;
	float distance;
	
	temppoint1=joints[which].position;
	temppoint2=joints[which].parent->position;
	distance=findDistance(temppoint1,temppoint2);
	joints[which].rotate2=asin((temppoint1.y-temppoint2.y)/distance)*rad2deg;
	temppoint1.y=0;
	temppoint2.y=0;
	joints[which].rotate1=acos((temppoint1.z-temppoint2.z)/findDistance(temppoint1,temppoint2))*rad2deg;
	if(temppoint1.x>temppoint2.x)joints[which].rotate1=360-joints[which].rotate1;
	if(joints[which].label==head)tempforward=specialforward[0];
	else if(joints[which].label==rightshoulder||joints[which].label==rightelbow||joints[which].label==rightwrist||joints[which].label==righthand)tempforward=specialforward[1];
	else if(joints[which].label==leftshoulder||joints[which].label==leftelbow||joints[which].label==leftwrist||joints[which].label==lefthand)tempforward=specialforward[2];
	else if(joints[which].label==righthip||joints[which].label==rightknee||joints[which].label==rightankle)tempforward=specialforward[3];
	else if(joints[which].label==lefthip||joints[which].label==leftknee||joints[which].label==leftankle)tempforward=specialforward[4];
	else if(!joints[which].lower)tempforward=forward;
	else if(joints[which].lower)tempforward=lowforward;
	tempforward=DoRotation(tempforward,0,joints[which].rotate1-90,0);
	tempforward=DoRotation(tempforward,0,0,joints[which].rotate2-90);
	tempforward.y=0;
	Normalise(&tempforward);
	joints[which].rotate3=acos(0-tempforward.z)*rad2deg;
	if(0>tempforward.x)joints[which].rotate3=360-joints[which].rotate3;
}

void Skeleton::FindRotationMuscle(int which)
{
	XYZ temppoint1,temppoint2,tempforward;
	float distance;
	
	temppoint1=muscles[which].parent1->position;
	temppoint2=muscles[which].parent2->position;
	distance=findDistance(temppoint1,temppoint2);
	muscles[which].rotate2=asin((temppoint1.y-temppoint2.y)/distance)*rad2deg;
	temppoint1.y=0;
	temppoint2.y=0;
	muscles[which].rotate1=acos((temppoint1.z-temppoint2.z)/findDistance(temppoint1,temppoint2));
	muscles[which].rotate1*=360/6.28;
	if(temppoint1.x>temppoint2.x)muscles[which].rotate1=360-muscles[which].rotate1;
	if(muscles[which].parent1->label==head)tempforward=specialforward[0];
	else if(muscles[which].parent1->label==rightshoulder||muscles[which].parent1->label==rightelbow||muscles[which].parent1->label==rightwrist)tempforward=specialforward[1];
	else if(muscles[which].parent1->label==leftshoulder||muscles[which].parent1->label==leftelbow||muscles[which].parent1->label==leftwrist)tempforward=specialforward[2];
	else if(muscles[which].parent1->label==righthip||muscles[which].parent1->label==rightknee||muscles[which].parent1->label==rightankle)tempforward=specialforward[3];
	else if(muscles[which].parent1->label==lefthip||muscles[which].parent1->label==leftknee||muscles[which].parent1->label==leftankle)tempforward=specialforward[4];
	else if(!muscles[which].parent1->lower)tempforward=forward;
	else if(muscles[which].parent1->lower)tempforward=lowforward;
	tempforward=DoRotation(tempforward,0,muscles[which].rotate1-90,0);
	tempforward=DoRotation(tempforward,0,0,muscles[which].rotate2-90);
	tempforward.y=0;
	Normalise(&tempforward);
	muscles[which].rotate3=acos(0-tempforward.z)*rad2deg;
	for(int i=0;i<num_joints;i++){
		if(&joints[i]==muscles[which].parent1){
			joints[i].rotate1=muscles[which].rotate1;
			joints[i].rotate2=muscles[which].rotate2;
			joints[i].rotate3=muscles[which].rotate3;
		}
	}
	if(0>tempforward.x)muscles[which].rotate3=360-muscles[which].rotate3;
}

extern Skeleton testskeleton;

void Animation::Load(char *fileName)
{
	float placeholder[3];
	files.OpenFile((unsigned char*)fileName);
	if(files.sFile){
		ReadInt(files.sFile, 1, &numframes);
		for(int i=0;i<numframes;i++){
			for(int j=0;j<max_joints;j++){
				ReadXYZ(files.sFile, 1, &position[j][i]);
			}
			for(int j=0;j<max_joints;j++){
				ReadFloat(files.sFile, 1, &twist[j][i]);
			}
			for(int j=0;j<max_joints;j++){
				ReadBool(files.sFile, 1, &onground[j][i]);
			}
			ReadFloat(files.sFile, 1, &speed[i]);
			ReadFloat(files.sFile, 1, &gunrotation[i]);
		}
		for(int i=0;i<numframes;i++){
			for(int j=0;j<max_joints;j++){
				ReadFloat(files.sFile, 1, &twist2[j][i]);
			}
		}
	}
	
	files.EndLoad();
	
	for(int j=0;j<numframes;j++){
		for(int i=0;i<testskeleton.num_joints;i++){
			testskeleton.joints[i].position=position[i][j];	
		}
		//Find forward vectors
		CrossProduct(testskeleton.joints[testskeleton.forwardjoints[1]].position-testskeleton.joints[testskeleton.forwardjoints[0]].position,testskeleton.joints[testskeleton.forwardjoints[2]].position-testskeleton.joints[testskeleton.forwardjoints[0]].position,&testskeleton.forward);
		Normalise(&testskeleton.forward);
		
		CrossProduct(testskeleton.joints[testskeleton.lowforwardjoints[1]].position-testskeleton.joints[testskeleton.lowforwardjoints[0]].position,testskeleton.joints[testskeleton.lowforwardjoints[2]].position-testskeleton.joints[testskeleton.lowforwardjoints[0]].position,&testskeleton.lowforward);
		Normalise(&testskeleton.lowforward);
		
		//Special forwards
		testskeleton.specialforward[0]=testskeleton.forward;
		
		testskeleton.specialforward[1]=testskeleton.joints[testskeleton.jointlabels[rightshoulder]].position+testskeleton.joints[testskeleton.jointlabels[rightwrist]].position;
		testskeleton.specialforward[1]=testskeleton.joints[testskeleton.jointlabels[rightelbow]].position-testskeleton.specialforward[1]/2;
		testskeleton.specialforward[1]+=testskeleton.forward*.2;
		Normalise(&testskeleton.specialforward[1]);
		testskeleton.specialforward[2]=testskeleton.joints[testskeleton.jointlabels[leftshoulder]].position+testskeleton.joints[testskeleton.jointlabels[leftwrist]].position;
		testskeleton.specialforward[2]=testskeleton.joints[testskeleton.jointlabels[leftelbow]].position-testskeleton.specialforward[2]/2;
		testskeleton.specialforward[2]+=testskeleton.forward*.2;
		Normalise(&testskeleton.specialforward[2]);
		
		testskeleton.specialforward[3]=testskeleton.joints[testskeleton.jointlabels[righthip]].position+testskeleton.joints[testskeleton.jointlabels[rightankle]].position;
		testskeleton.specialforward[3]=testskeleton.specialforward[3]/2-testskeleton.joints[testskeleton.jointlabels[rightknee]].position;
		testskeleton.specialforward[3]+=testskeleton.lowforward*.2;
		Normalise(&testskeleton.specialforward[3]);
		testskeleton.specialforward[4]=testskeleton.joints[testskeleton.jointlabels[lefthip]].position+testskeleton.joints[testskeleton.jointlabels[leftankle]].position;
		testskeleton.specialforward[4]=testskeleton.specialforward[4]/2-testskeleton.joints[testskeleton.jointlabels[leftknee]].position;
		testskeleton.specialforward[4]+=testskeleton.lowforward*.2;
		Normalise(&testskeleton.specialforward[4]);

		//Find joint rotations
		for(int i=0;i<testskeleton.num_joints;i++){
			if(testskeleton.joints[i].hasparent&&testskeleton.joints[i].visible)
			{
				testskeleton.FindRotationJoint(i);
			}
		}
		for(int i=0;i<testskeleton.num_muscles;i++){
			if(testskeleton.muscles[i].visible)
			{
				testskeleton.FindRotationMuscle(i);
			}
		}
		for(int i=0;i<testskeleton.num_muscles;i++){
			if(testskeleton.muscles[i].visible)
			{
				mrotate1[i][j]=testskeleton.muscles[i].rotate1;
				mrotate2[i][j]=testskeleton.muscles[i].rotate2;
				mrotate3[i][j]=testskeleton.muscles[i].rotate3;
				if(j!=0&&mrotate3[i][j]>mrotate3[i][j-1]+180)mrotate3[i][j]-=360;
				if(j!=0&&mrotate3[i][j]<mrotate3[i][j-1]-180)mrotate3[i][j]+=360;
				if(j!=0&&mrotate2[i][j]>mrotate2[i][j-1]+180)mrotate2[i][j]-=360;
				if(j!=0&&mrotate2[i][j]<mrotate2[i][j-1]-180)mrotate2[i][j]+=360;
				if(j!=0&&mrotate1[i][j]>mrotate1[i][j-1]+180)mrotate1[i][j]-=360;
				if(j!=0&&mrotate1[i][j]<mrotate1[i][j-1]-180)mrotate1[i][j]+=360;
			}
		}
		for(int i=0;i<testskeleton.num_joints;i++){
			if(testskeleton.joints[i].hasparent&&testskeleton.joints[i].visible)
			{
				rotate1[i][j]=testskeleton.joints[i].rotate1;
				rotate2[i][j]=testskeleton.joints[i].rotate2;
				rotate3[i][j]=testskeleton.joints[i].rotate3;
				if(j!=0&&rotate3[i][j]>rotate3[i][j-1]+180)rotate3[i][j]-=360;
				if(j!=0&&rotate3[i][j]<rotate3[i][j-1]-180)rotate3[i][j]+=360;
				if(j!=0&&rotate2[i][j]>rotate2[i][j-1]+180)rotate2[i][j]-=360;
				if(j!=0&&rotate2[i][j]<rotate2[i][j-1]-180)rotate2[i][j]+=360;
				if(j!=0&&rotate1[i][j]>rotate1[i][j-1]+180)rotate1[i][j]-=360;
				if(j!=0&&rotate1[i][j]<rotate1[i][j-1]-180)rotate1[i][j]+=360;
			}
		}
	}
	
	for(int k=0;k<2;k++)
	for(int j=0;j<numframes;j++){
		for(int i=0;i<testskeleton.num_muscles;i++){
			if(testskeleton.muscles[i].visible)
			{
				if(j!=0&&mrotate3[i][j]>mrotate3[i][j-1]+180)mrotate3[i][j]-=360;
				if(j!=0&&mrotate3[i][j]<mrotate3[i][j-1]-180)mrotate3[i][j]+=360;
				if(j!=0&&mrotate2[i][j]>mrotate2[i][j-1]+180)mrotate2[i][j]-=360;
				if(j!=0&&mrotate2[i][j]<mrotate2[i][j-1]-180)mrotate2[i][j]+=360;
				if(j!=0&&mrotate1[i][j]>mrotate1[i][j-1]+180)mrotate1[i][j]-=360;
				if(j!=0&&mrotate1[i][j]<mrotate1[i][j-1]-180)mrotate1[i][j]+=360;
				
				if(j==0&&mrotate3[i][j]>mrotate3[i][numframes-1]+180)mrotate3[i][j]-=360;
				if(j==0&&mrotate3[i][j]<mrotate3[i][numframes-1]-180)mrotate3[i][j]+=360;
				if(j==0&&mrotate2[i][j]>mrotate2[i][numframes-1]+180)mrotate2[i][j]-=360;
				if(j==0&&mrotate2[i][j]<mrotate2[i][numframes-1]-180)mrotate2[i][j]+=360;
				if(j==0&&mrotate1[i][j]>mrotate1[i][numframes-1]+180)mrotate1[i][j]-=360;
				if(j==0&&mrotate1[i][j]<mrotate1[i][numframes-1]-180)mrotate1[i][j]+=360;
			}
		}
		for(int i=0;i<testskeleton.num_joints;i++){
			if(testskeleton.joints[i].hasparent&&testskeleton.joints[i].visible)
			{
				if(j!=0&&rotate3[i][j]>rotate3[i][j-1]+180)rotate3[i][j]-=360;
				if(j!=0&&rotate3[i][j]<rotate3[i][j-1]-180)rotate3[i][j]+=360;
				if(j!=0&&rotate2[i][j]>rotate2[i][j-1]+180)rotate2[i][j]-=360;
				if(j!=0&&rotate2[i][j]<rotate2[i][j-1]-180)rotate2[i][j]+=360;
				if(j!=0&&rotate1[i][j]>rotate1[i][j-1]+180)rotate1[i][j]-=360;
				if(j!=0&&rotate1[i][j]<rotate1[i][j-1]-180)rotate1[i][j]+=360;
				
				if(j==0&&rotate3[i][j]>rotate3[i][numframes-1]+180)rotate3[i][j]-=360;
				if(j==0&&rotate3[i][j]<rotate3[i][numframes-1]-180)rotate3[i][j]+=360;
				if(j==0&&rotate2[i][j]>rotate2[i][numframes-1]+180)rotate2[i][j]-=360;
				if(j==0&&rotate2[i][j]<rotate2[i][numframes-1]-180)rotate2[i][j]+=360;
				if(j==0&&rotate1[i][j]>rotate1[i][numframes-1]+180)rotate1[i][j]-=360;
				if(j==0&&rotate1[i][j]<rotate1[i][numframes-1]-180)rotate1[i][j]+=360;
			}
		}
	}
}

void Animation::Load(char *fileName, float rotate)
{
	float placeholder[3];
	files.OpenFile((unsigned char*)fileName);
	if(files.sFile){
		ReadInt(files.sFile, 1, &numframes);
		for(int i=0;i<numframes;i++){
			for(int j=0;j<max_joints;j++){
				ReadXYZ(files.sFile, 1, &position[j][i]);
				position[j][i]=DoRotation(position[j][i],0,rotate,0);
			}
			for(int j=0;j<max_joints;j++){
				ReadFloat(files.sFile, 1, &twist[j][i]);
			}
			for(int j=0;j<max_joints;j++){
				ReadBool(files.sFile, 1, &onground[j][i]);
			}
			ReadFloat(files.sFile, 1, &speed[i]);
			ReadFloat(files.sFile, 1, &gunrotation[i]);
		}
		for(int i=0;i<numframes;i++){
			for(int j=0;j<max_joints;j++){
				ReadFloat(files.sFile, 1, &twist2[j][i]);
			}
		}
	}
	files.EndLoad();
	
	for(int j=0;j<numframes;j++){
		for(int i=0;i<testskeleton.num_joints;i++){
			testskeleton.joints[i].position=position[i][j];	
		}
		//Find forward vectors
		CrossProduct(testskeleton.joints[testskeleton.forwardjoints[1]].position-testskeleton.joints[testskeleton.forwardjoints[0]].position,testskeleton.joints[testskeleton.forwardjoints[2]].position-testskeleton.joints[testskeleton.forwardjoints[0]].position,&testskeleton.forward);
		Normalise(&testskeleton.forward);
		
		CrossProduct(testskeleton.joints[testskeleton.lowforwardjoints[1]].position-testskeleton.joints[testskeleton.lowforwardjoints[0]].position,testskeleton.joints[testskeleton.lowforwardjoints[2]].position-testskeleton.joints[testskeleton.lowforwardjoints[0]].position,&testskeleton.lowforward);
		Normalise(&testskeleton.lowforward);
		
		//Special forwards
		testskeleton.specialforward[0]=testskeleton.forward;
		
		testskeleton.specialforward[1]=testskeleton.joints[testskeleton.jointlabels[rightshoulder]].position+testskeleton.joints[testskeleton.jointlabels[rightwrist]].position;
		testskeleton.specialforward[1]=testskeleton.joints[testskeleton.jointlabels[rightelbow]].position-testskeleton.specialforward[1]/2;
		testskeleton.specialforward[1]+=testskeleton.forward*.2;
		Normalise(&testskeleton.specialforward[1]);
		testskeleton.specialforward[2]=testskeleton.joints[testskeleton.jointlabels[leftshoulder]].position+testskeleton.joints[testskeleton.jointlabels[leftwrist]].position;
		testskeleton.specialforward[2]=testskeleton.joints[testskeleton.jointlabels[leftelbow]].position-testskeleton.specialforward[2]/2;
		testskeleton.specialforward[2]+=testskeleton.forward*.2;
		Normalise(&testskeleton.specialforward[2]);
		
		testskeleton.specialforward[3]=testskeleton.joints[testskeleton.jointlabels[righthip]].position+testskeleton.joints[testskeleton.jointlabels[rightankle]].position;
		testskeleton.specialforward[3]=testskeleton.specialforward[3]/2-testskeleton.joints[testskeleton.jointlabels[rightknee]].position;
		testskeleton.specialforward[3]+=testskeleton.lowforward*.2;
		Normalise(&testskeleton.specialforward[3]);
		testskeleton.specialforward[4]=testskeleton.joints[testskeleton.jointlabels[lefthip]].position+testskeleton.joints[testskeleton.jointlabels[leftankle]].position;
		testskeleton.specialforward[4]=testskeleton.specialforward[4]/2-testskeleton.joints[testskeleton.jointlabels[leftknee]].position;
		testskeleton.specialforward[4]+=testskeleton.lowforward*.2;
		Normalise(&testskeleton.specialforward[4]);

		//Find joint rotations
		for(int i=0;i<testskeleton.num_joints;i++){
			if(testskeleton.joints[i].hasparent&&testskeleton.joints[i].visible)
			{
				testskeleton.FindRotationJoint(i);
			}
		}
		for(int i=0;i<testskeleton.num_muscles;i++){
			if(testskeleton.muscles[i].visible)
			{
				testskeleton.FindRotationMuscle(i);
			}
		}
		for(int i=0;i<testskeleton.num_muscles;i++){
			if(testskeleton.muscles[i].visible)
			{
				mrotate1[i][j]=testskeleton.muscles[i].rotate1;
				mrotate2[i][j]=testskeleton.muscles[i].rotate2;
				mrotate3[i][j]=testskeleton.muscles[i].rotate3;
				if(j!=0&&mrotate3[i][j]>mrotate3[i][j-1]+180)mrotate3[i][j]-=360;
				if(j!=0&&mrotate3[i][j]<mrotate3[i][j-1]-180)mrotate3[i][j]+=360;
				if(j!=0&&mrotate2[i][j]>mrotate2[i][j-1]+180)mrotate2[i][j]-=360;
				if(j!=0&&mrotate2[i][j]<mrotate2[i][j-1]-180)mrotate2[i][j]+=360;
				if(j!=0&&mrotate1[i][j]>mrotate1[i][j-1]+180)mrotate1[i][j]-=360;
				if(j!=0&&mrotate1[i][j]<mrotate1[i][j-1]-180)mrotate1[i][j]+=360;
			}
		}
		for(int i=0;i<testskeleton.num_joints;i++){
			if(testskeleton.joints[i].hasparent&&testskeleton.joints[i].visible)
			{
				rotate1[i][j]=testskeleton.joints[i].rotate1;
				rotate2[i][j]=testskeleton.joints[i].rotate2;
				rotate3[i][j]=testskeleton.joints[i].rotate3;
				if(j!=0&&rotate3[i][j]>rotate3[i][j-1]+180)rotate3[i][j]-=360;
				if(j!=0&&rotate3[i][j]<rotate3[i][j-1]-180)rotate3[i][j]+=360;
				if(j!=0&&rotate2[i][j]>rotate2[i][j-1]+180)rotate2[i][j]-=360;
				if(j!=0&&rotate2[i][j]<rotate2[i][j-1]-180)rotate2[i][j]+=360;
				if(j!=0&&rotate1[i][j]>rotate1[i][j-1]+180)rotate1[i][j]-=360;
				if(j!=0&&rotate1[i][j]<rotate1[i][j-1]-180)rotate1[i][j]+=360;
			}
		}
	}
	
	for(int k=0;k<2;k++)
	for(int j=0;j<numframes;j++){
		for(int i=0;i<testskeleton.num_muscles;i++){
			if(testskeleton.muscles[i].visible)
			{
				if(j!=0&&mrotate3[i][j]>mrotate3[i][j-1]+180)mrotate3[i][j]-=360;
				if(j!=0&&mrotate3[i][j]<mrotate3[i][j-1]-180)mrotate3[i][j]+=360;
				if(j!=0&&mrotate2[i][j]>mrotate2[i][j-1]+180)mrotate2[i][j]-=360;
				if(j!=0&&mrotate2[i][j]<mrotate2[i][j-1]-180)mrotate2[i][j]+=360;
				if(j!=0&&mrotate1[i][j]>mrotate1[i][j-1]+180)mrotate1[i][j]-=360;
				if(j!=0&&mrotate1[i][j]<mrotate1[i][j-1]-180)mrotate1[i][j]+=360;
				
				if(j==0&&mrotate3[i][j]>mrotate3[i][numframes-1]+180)mrotate3[i][j]-=360;
				if(j==0&&mrotate3[i][j]<mrotate3[i][numframes-1]-180)mrotate3[i][j]+=360;
				if(j==0&&mrotate2[i][j]>mrotate2[i][numframes-1]+180)mrotate2[i][j]-=360;
				if(j==0&&mrotate2[i][j]<mrotate2[i][numframes-1]-180)mrotate2[i][j]+=360;
				if(j==0&&mrotate1[i][j]>mrotate1[i][numframes-1]+180)mrotate1[i][j]-=360;
				if(j==0&&mrotate1[i][j]<mrotate1[i][numframes-1]-180)mrotate1[i][j]+=360;
			}
		}
		for(int i=0;i<testskeleton.num_joints;i++){
			if(testskeleton.joints[i].hasparent&&testskeleton.joints[i].visible)
			{
				if(j!=0&&rotate3[i][j]>rotate3[i][j-1]+180)rotate3[i][j]-=360;
				if(j!=0&&rotate3[i][j]<rotate3[i][j-1]-180)rotate3[i][j]+=360;
				if(j!=0&&rotate2[i][j]>rotate2[i][j-1]+180)rotate2[i][j]-=360;
				if(j!=0&&rotate2[i][j]<rotate2[i][j-1]-180)rotate2[i][j]+=360;
				if(j!=0&&rotate1[i][j]>rotate1[i][j-1]+180)rotate1[i][j]-=360;
				if(j!=0&&rotate1[i][j]<rotate1[i][j-1]-180)rotate1[i][j]+=360;
				
				if(j==0&&rotate3[i][j]>rotate3[i][numframes-1]+180)rotate3[i][j]-=360;
				if(j==0&&rotate3[i][j]<rotate3[i][numframes-1]-180)rotate3[i][j]+=360;
				if(j==0&&rotate2[i][j]>rotate2[i][numframes-1]+180)rotate2[i][j]-=360;
				if(j==0&&rotate2[i][j]<rotate2[i][numframes-1]-180)rotate2[i][j]+=360;
				if(j==0&&rotate1[i][j]>rotate1[i][numframes-1]+180)rotate1[i][j]-=360;
				if(j==0&&rotate1[i][j]<rotate1[i][numframes-1]-180)rotate1[i][j]+=360;
			}
		}
	}
}

void Skeleton::Load(char *fileName)
{
	int parentID;
	bool what;
	bool lock;
	float placeholder[3];
	files.OpenFile((unsigned char*)fileName);
	if(files.sFile){
		ReadInt(files.sFile, 1, &num_joints);
		for(int i=0;i<num_joints;i++){
			ReadXYZ(files.sFile, 1, &joints[i].position);
			ReadFloat(files.sFile, 1, &joints[i].length);
			ReadFloat(files.sFile, 1, &joints[i].mass);
			ReadBool(files.sFile, 1, &joints[i].hasparent);
			ReadBool(files.sFile, 1, &joints[i].locked);
			ReadInt(files.sFile, 1, &joints[i].modelnum);
			ReadBool(files.sFile, 1, &joints[i].visible);
			ReadBool(files.sFile, 1, &what);
			ReadInt(files.sFile, 1, &joints[i].label);
			ReadInt(files.sFile, 1, &joints[i].hasgun);
			ReadBool(files.sFile, 1, &joints[i].lower);
			ReadInt(files.sFile, 1, &parentID);
			if(joints[i].hasparent)joints[i].parent=&joints[parentID];
			joints[i].velocity=0;
			joints[i].oldposition=joints[i].position;
			joints[i].existing=1;
		}

		ReadInt(files.sFile, 1, &num_muscles);
		for(int i=0;i<num_muscles;i++){
			ReadFloat(files.sFile, 1, &muscles[i].length);
			ReadFloat(files.sFile, 1, &muscles[i].targetlength);
			ReadFloat(files.sFile, 1, &muscles[i].minlength);
			ReadFloat(files.sFile, 1, &muscles[i].maxlength);
			ReadFloat(files.sFile, 1, &muscles[i].strength);
			ReadInt(files.sFile, 1, &muscles[i].type);
			ReadBool(files.sFile, 1, &muscles[i].visible);
			ReadInt(files.sFile, 1, &parentID);
			muscles[i].parent1=&joints[parentID];
			ReadInt(files.sFile, 1, &parentID);
			muscles[i].parent2=&joints[parentID];
		}

		for(int j=0;j<3;j++){
			ReadInt(files.sFile, 1, &forwardjoints[j]);
		}
		for(int j=0;j<3;j++){
			ReadInt(files.sFile, 1, &lowforwardjoints[j]);
		}
	}
	files.EndLoad();
	
	for(int i=0;i<num_joints;i++){
		for(int j=0;j<num_joints;j++){
			if(joints[i].label==j)jointlabels[j]=i;
		}
	}
	
	broken=0;	
}

