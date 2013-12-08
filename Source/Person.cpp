/**> HEADER FILES <**/
#include "Person.h"

extern double multiplier;
extern unsigned int gSourceID[100];
extern unsigned int gSampleSet[100];
extern Animation animation[30];
extern int thirdperson;
extern int visions;
extern Camera camera;
extern float rad2deg;
extern Model gunmodels[10];
extern Model skeletonmodels[10];
extern Sprites sprites;
extern float soundscalefactor;
extern int slomo;

extern int forwardskey;
extern int backwardskey;
extern int leftkey;
extern int rightkey;
extern int aimkey;
extern int psychicaimkey;
extern int psychickey;

HitStruct 	Person::BulletCollideWithPlayer(int who, XYZ start, XYZ end){
	XYZ tempbulletloc[2];
	XYZ collisionpoint;
	GLfloat M[16];
	int collide;
	XYZ average;
	int howmany;
	float distancemax;
	HitStruct hitstruct;
	hitstruct.collision=0;
	//Make bounding sphere
	average=0;
	howmany=0;
	for(int j=0;j<skeleton.num_joints;j++){
		average.x=average.x+skeleton.joints[j].position.x;
		average.y=average.y+skeleton.joints[j].position.y;
		average.z=average.z+skeleton.joints[j].position.z;
		howmany++;
	}
	average=average/howmany;
	distancemax=0;
	for(int j=0;j<skeleton.num_joints;j++){
		if(findDistancefast(average,skeleton.joints[j].position)>distancemax){
			distancemax=findDistancefast(average,skeleton.joints[j].position);
		}
	}
	distancemax=fast_sqrt(distancemax);
	//Collide with player
	if(skeleton.free<1){
		start=start-playercoords;
		end=end-playercoords;
		if(playerrotation)start=DoRotation(start,0,-playerrotation,0);
		if(playerrotation)end=DoRotation(end,0,-playerrotation,0);
	}
	tempbulletloc[0]=start;
	tempbulletloc[1]=end;
	if(sphere_line_intersection(tempbulletloc[0].x,tempbulletloc[0].y,tempbulletloc[0].z,
								tempbulletloc[1].x,tempbulletloc[1].y,tempbulletloc[1].z,
								average.x, average.y, average.z, distancemax)){
	for(int j=0;j<skeleton.num_joints;j++){
		if(skeleton.joints[j].hasparent&&skeleton.joints[j].visible){
			tempbulletloc[0]=start;
			tempbulletloc[1]=end;
			glPushMatrix();
				glLoadIdentity();
				glScalef(1,1/skeleton.joints[j].length,1);
				glRotatef(skeleton.joints[j].rotate2-90,0,0,1);
				glRotatef(skeleton.joints[j].rotate1-90,0,1,0);
				glTranslatef(	(-(skeleton.joints[j].position.x+skeleton.joints[j].parent->position.x)/2),
								(-(skeleton.joints[j].position.y+skeleton.joints[j].parent->position.y)/2),
								(-(skeleton.joints[j].position.z+skeleton.joints[j].parent->position.z)/2));
				glTranslatef(tempbulletloc[0].x,tempbulletloc[0].y,tempbulletloc[0].z);
				glGetFloatv(GL_MODELVIEW_MATRIX,M);
				tempbulletloc[0].x=M[12];
				tempbulletloc[0].y=M[13];
				tempbulletloc[0].z=M[14];
			glPopMatrix();
			glPushMatrix();
				glLoadIdentity();
				glScalef(1,1/skeleton.joints[j].length,1);
				glRotatef(skeleton.joints[j].rotate2-90,0,0,1);
				glRotatef(skeleton.joints[j].rotate1-90,0,1,0);
				glTranslatef(	(-(skeleton.joints[j].position.x+skeleton.joints[j].parent->position.x)/2),
								(-(skeleton.joints[j].position.y+skeleton.joints[j].parent->position.y)/2),
								(-(skeleton.joints[j].position.z+skeleton.joints[j].parent->position.z)/2));
				glTranslatef(tempbulletloc[1].x,tempbulletloc[1].y,tempbulletloc[1].z);
				glGetFloatv(GL_MODELVIEW_MATRIX,M);
				tempbulletloc[1].x=M[12];
				tempbulletloc[1].y=M[13];
				tempbulletloc[1].z=M[14];
			glPopMatrix();
			collide=skeletonmodels[skeleton.joints[j].modelnum].LineCheck(tempbulletloc[0],tempbulletloc[1],&collisionpoint);
			if(collide!=-1)
			{
				glPushMatrix();
					glLoadIdentity();
					glTranslatef(	(skeleton.joints[j].position.x+skeleton.joints[j].parent->position.x)/2,
									(skeleton.joints[j].position.y+skeleton.joints[j].parent->position.y)/2,
									(skeleton.joints[j].position.z+skeleton.joints[j].parent->position.z)/2);
					glRotatef(-skeleton.joints[j].rotate1+90,0,1,0);
					glRotatef(-skeleton.joints[j].rotate2+90,0,0,1);
					glScalef(1,skeleton.joints[j].length,1);
					glTranslatef(collisionpoint.x,collisionpoint.y,collisionpoint.z);
					glGetFloatv(GL_MODELVIEW_MATRIX,M);
					collisionpoint.x=M[12];
					collisionpoint.y=M[13];
					collisionpoint.z=M[14];
				glPopMatrix();
				hitstruct.collision=1;
				hitstruct.hitlocation=collisionpoint;
				hitstruct.joint1=&skeleton.joints[j];
				hitstruct.joint2=skeleton.joints[j].parent;
			}
		}
	}
	for(int j=0;j<skeleton.num_muscles;j++){
		if(skeleton.muscles[j].visible){
			tempbulletloc[0]=start;
			tempbulletloc[1]=end;
			glPushMatrix();
				glLoadIdentity();
				glScalef(1,1/skeleton.muscles[j].length,1);
				glRotatef(skeleton.muscles[j].rotate3,0,1,0);
				glRotatef(skeleton.muscles[j].rotate2-90,0,0,1);
				glRotatef(skeleton.muscles[j].rotate1-90,0,1,0);
				glTranslatef(	(-(skeleton.muscles[j].parent1->position.x+skeleton.muscles[j].parent2->position.x)/2),
								(-(skeleton.muscles[j].parent1->position.y+skeleton.muscles[j].parent2->position.y)/2),
								(-(skeleton.muscles[j].parent1->position.z+skeleton.muscles[j].parent2->position.z)/2));

				glTranslatef(tempbulletloc[0].x,tempbulletloc[0].y,tempbulletloc[0].z);
				glGetFloatv(GL_MODELVIEW_MATRIX,M);
				tempbulletloc[0].x=M[12];
				tempbulletloc[0].y=M[13];
				tempbulletloc[0].z=M[14];
			glPopMatrix();
			glPushMatrix();
				glLoadIdentity();
				glScalef(1,1/skeleton.muscles[j].length,1);
				glRotatef(skeleton.muscles[j].rotate3,0,1,0);
				glRotatef(skeleton.muscles[j].rotate2-90,0,0,1);
				glRotatef(skeleton.muscles[j].rotate1-90,0,1,0);
				
				glTranslatef(	(-(skeleton.muscles[j].parent1->position.x+skeleton.muscles[j].parent2->position.x)/2),
								(-(skeleton.muscles[j].parent1->position.y+skeleton.muscles[j].parent2->position.y)/2),
								(-(skeleton.muscles[j].parent1->position.z+skeleton.muscles[j].parent2->position.z)/2));
				glTranslatef(tempbulletloc[1].x,tempbulletloc[1].y,tempbulletloc[1].z);
				glGetFloatv(GL_MODELVIEW_MATRIX,M);
				tempbulletloc[1].x=M[12];
				tempbulletloc[1].y=M[13];
				tempbulletloc[1].z=M[14];
			glPopMatrix();
			collide=skeletonmodels[skeleton.muscles[j].parent1->modelnum].LineCheck(tempbulletloc[0],tempbulletloc[1],&collisionpoint);
			if(collide!=-1)
			{
				glPushMatrix();
					glLoadIdentity();
					glTranslatef(	(skeleton.muscles[j].parent1->position.x+skeleton.muscles[j].parent2->position.x)/2,
									(skeleton.muscles[j].parent1->position.y+skeleton.muscles[j].parent2->position.y)/2,
									(skeleton.muscles[j].parent1->position.z+skeleton.muscles[j].parent2->position.z)/2);
					glRotatef(-skeleton.muscles[j].rotate1+90,0,1,0);
					glRotatef(-skeleton.muscles[j].rotate2+90,0,0,1);
					glRotatef(-skeleton.muscles[j].rotate3,0,1,0);
					glScalef(1,findDistance(skeleton.muscles[j].parent1->position,skeleton.muscles[j].parent2->position),1);
					glTranslatef(collisionpoint.x,collisionpoint.y,collisionpoint.z);
					glGetFloatv(GL_MODELVIEW_MATRIX,M);
					collisionpoint.x=M[12];
					collisionpoint.y=M[13];
					collisionpoint.z=M[14];
				glPopMatrix();
				hitstruct.collision=1;
				hitstruct.hitlocation=collisionpoint;
				hitstruct.joint1=skeleton.muscles[j].parent1;
				hitstruct.joint2=skeleton.muscles[j].parent2;
			}
		}
	}
	}
	if(skeleton.free<1){
		if(playerrotation)hitstruct.hitlocation=DoRotation(hitstruct.hitlocation,0,playerrotation,0);
		hitstruct.hitlocation=hitstruct.hitlocation+playercoords;
	}
	return hitstruct;
}

extern float camerashake;
extern int cycle;
void 	Person::DoAnimations(int who){	
	
	if(target>1&&!skeleton.free){
		//Footstep sounds
		if(who==0&&slomo==0&&(targetanimation==joganim||targetanimation==walkanim)&&(targetframe==0||targetframe==8)&&visions==0&&(onground||abs(velocity.y)<1)){
			int whichsound;
			float gLoc[3];
			gLoc[0]=playercoords.x/soundscalefactor;
			gLoc[1]=playercoords.y/soundscalefactor;
			gLoc[2]=playercoords.z/soundscalefactor;
			whichsound=footstepsound+abs(Random())%5;
			alSourcefv(gSourceID[whichsound], AL_POSITION, gLoc);
			alSourcePlay(gSourceID[whichsound]);
		}
		if(targetanimation==zombieeatanim&&(targetframe==3)){
			float gLoc[3];
			XYZ soundpoint=(DoRotation(skeleton.joints[skeleton.jointlabels[head]].position,0,playerrotation,0)+playercoords);
			gLoc[0]=soundpoint.x/soundscalefactor;
			gLoc[1]=soundpoint.y/soundscalefactor;
			gLoc[2]=soundpoint.z/soundscalefactor;
			alSourcefv(gSourceID[bodyhitsound], AL_POSITION, gLoc);
			alSourcePlay(gSourceID[bodyhitsound]);
			bleeding=1;
			bleeddelay=1;
			bjoint1=&skeleton.joints[skeleton.jointlabels[head]];
			bjoint2=&skeleton.joints[skeleton.jointlabels[neck]];
		}
		targetframe=currentframe;
		currentanimation=targetanimation;
		if(!backwardsanim){targetframe++;
		if(targetframe>animation[currentanimation].numframes-1)targetframe=0;}
		if(backwardsanim){targetframe--;
		if(targetframe<0)targetframe=animation[currentanimation].numframes-1;}
		target=0;
		if((currentanimation==getupfrontanim||currentanimation==getupbackanim)&&targetframe==0){
			targetanimation=idleanim;
		}
		if(targetanimation==diveanim&&currentanimation==diveanim&&targetframe==0){
			targetanimation=getupfrontanim;
			float gLoc[3];
			XYZ soundpoint=(DoRotation(skeleton.joints[skeleton.jointlabels[head]].position,0,playerrotation,0)+playercoords);
			gLoc[0]=soundpoint.x/soundscalefactor;
			gLoc[1]=soundpoint.y/soundscalefactor;
			gLoc[2]=soundpoint.z/soundscalefactor;
			alSourcefv(gSourceID[bodywhacksound], AL_POSITION, gLoc);
			alSourcePlay(gSourceID[bodywhacksound]);
		}
		if(currentanimation==throwanim&&targetframe==0){
			targetanimation=idleanim;
		}
		if(currentanimation==thrownanim&&targetframe==0){
			skeleton.offset=0;
			skeleton.free=1;
			longdead=1;
			for(int j=0;j<skeleton.num_joints;j++){
				skeleton.joints[j].position+=skeleton.joints[j].offset;
				skeleton.joints[j].position=DoRotation(skeleton.joints[j].position,0,playerrotation,0);
				skeleton.joints[j].position+=playercoords;
				skeleton.joints[j].realoldposition=skeleton.joints[j].position;
				skeleton.joints[j].velocity=DoRotation(skeleton.joints[j].velocity,0,playerrotation,0);
			}
		}
	}
	
	if(!skeleton.free){
		if(currentanimation!=lyinganim){
			if(animation[targetanimation].speed[currentframe]>animation[currentanimation].speed[currentframe])
				target+=multiplier*animation[targetanimation].speed[currentframe]*speed;
			if(animation[targetanimation].speed[currentframe]<=animation[currentanimation].speed[currentframe])
				target+=multiplier*animation[currentanimation].speed[currentframe]*speed;
		}
		if(currentanimation==lyinganim){
			target+=multiplier*animation[targetanimation].speed[targetframe]*speed;
		}		
		if(((currentanimation==crouchanim)&&(targetanimation!=crouchanim))||((currentanimation!=crouchanim)&&(targetanimation==crouchanim)))target+=multiplier*animation[crouchanim].speed[0]*2;
		if(currentanimation==idleanim&&targetanimation==idleanim)target-=multiplier*animation[idleanim].speed[0]/2;
		
		if(target>1)currentframe=targetframe;
		
		for(int i=0;i<skeleton.num_joints;i++){
			if(currentanimation!=lyinganim){
				skeleton.joints[i].velocity=((animation[currentanimation].position[i][currentframe]*(1-target)+animation[targetanimation].position[i][targetframe]*(target))-(skeleton.joints[i].position))/multiplier;
				skeleton.joints[i].position=animation[currentanimation].position[i][currentframe]*(1-target)+animation[targetanimation].position[i][targetframe]*(target);
				if(skeleton.joints[i].hasparent&&skeleton.joints[i].visible)
				{
					skeleton.joints[i].rotate1=animation[currentanimation].rotate1[i][currentframe]*(1-target)+animation[targetanimation].rotate1[i][targetframe]*(target);
					skeleton.joints[i].rotate2=animation[currentanimation].rotate2[i][currentframe]*(1-target)+animation[targetanimation].rotate2[i][targetframe]*(target);
					skeleton.joints[i].rotate3=animation[currentanimation].rotate3[i][currentframe]*(1-target)+animation[targetanimation].rotate3[i][targetframe]*(target);
				}
				if(skeleton.muscles[i].visible)
				{
					skeleton.muscles[i].rotate1=animation[currentanimation].mrotate1[i][currentframe]*(1-target)+animation[targetanimation].mrotate1[i][targetframe]*(target);
					skeleton.muscles[i].rotate2=animation[currentanimation].mrotate2[i][currentframe]*(1-target)+animation[targetanimation].mrotate2[i][targetframe]*(target);
					skeleton.muscles[i].rotate3=animation[currentanimation].mrotate3[i][currentframe]*(1-target)+animation[targetanimation].mrotate3[i][targetframe]*(target);
				}
			}
			if(currentanimation==lyinganim){
				skeleton.joints[i].velocity=((tempanimation.position[i][0]*(1-target)+animation[targetanimation].position[i][targetframe]*(target))-(skeleton.joints[i].position))/multiplier;
				skeleton.joints[i].position=tempanimation.position[i][0]*(1-target)+animation[targetanimation].position[i][targetframe]*(target);
			}
		}
	}
	
	//Look up+down
	if(!skeleton.free&&(whichgun!=nogun||who==0)&&health==100&&currentanimation!=lyinganim&&currentanimation!=getupfrontanim&&currentanimation!=getupbackanim&&currentanimation!=diveanim&&targetanimation!=diveanim&&targetanimation!=throwanim&&targetanimation!=thrownanim){		
		XYZ facing;
		XYZ facingdown;
		XYZ facinghalf;
		XYZ facingright;
		if(who==0){
			playerrotation2=camera.rotation2;
			//Facing
			facing=0;
			facing.z=1;
			
			facinghalf=DoRotation(facing,playerrotation2/2,0,0);
			facinghalf=DoRotation(facinghalf,0,-7,0);
			facing=DoRotation(facing,playerrotation2,0,0);
			facingright=DoRotation(facing,0,-90,0);
			facingdown=DoRotation(facing,90,0,0);
		}
		if(who!=0){
			//Facing
			facing=0;
			facing.z=1;
			
			facinghalf=DoRotation(facing,playerrotation2/2,0,0);
			facinghalf=DoRotation(facinghalf,0,-7,0);
			facing=DoRotation(facing,playerrotation2,0,0);
			facingright=DoRotation(facing,0,-90,0);
			facingdown=DoRotation(facing,90,0,0);
		}
		XYZ rotatearound;
		XYZ oldpos;	
		if(whichgun==sniperrifle){
			for(int i=0;i<skeleton.num_joints;i++){
				if(skeleton.joints[i].label!=righthand&&skeleton.joints[i].label!=rightelbow&&skeleton.joints[i].label!=rightwrist&&skeleton.joints[i].label!=lefthand&&skeleton.joints[i].label!=leftelbow&&skeleton.joints[i].label!=leftwrist){
				}else{
					skeleton.joints[i].position=animation[rifleholdanim].position[i][0];
					if(currentanimation==crouchanim||targetanimation==crouchanim)skeleton.joints[i].position-=(animation[idleanim].position[skeleton.jointlabels[neck]][0]-skeleton.joints[skeleton.jointlabels[neck]].position);
				}
			}
		}
		if(whichgun==shotgun){
			for(int i=0;i<skeleton.num_joints;i++){
				if(skeleton.joints[i].label!=righthand&&skeleton.joints[i].label!=rightelbow&&skeleton.joints[i].label!=rightwrist&&skeleton.joints[i].label!=lefthand&&skeleton.joints[i].label!=leftelbow&&skeleton.joints[i].label!=leftwrist){
				}else{
					skeleton.joints[i].position=animation[rifleholdanim].position[i][0];
					if(currentanimation==crouchanim||targetanimation==crouchanim)skeleton.joints[i].position-=(animation[idleanim].position[skeleton.jointlabels[neck]][0]-skeleton.joints[skeleton.jointlabels[neck]].position);
				}
			}
		}	
		if(whichgun==assaultrifle){
			for(int i=0;i<skeleton.num_joints;i++){
				if(skeleton.joints[i].label!=righthand&&skeleton.joints[i].label!=rightelbow&&skeleton.joints[i].label!=rightwrist&&skeleton.joints[i].label!=lefthand&&skeleton.joints[i].label!=leftelbow&&skeleton.joints[i].label!=leftwrist){
				}else{
					skeleton.joints[i].position=animation[rifleholdanim].position[i][0];
					if(currentanimation==crouchanim||targetanimation==crouchanim)skeleton.joints[i].position-=(animation[idleanim].position[skeleton.jointlabels[neck]][0]-skeleton.joints[skeleton.jointlabels[neck]].position);
				}
			}
		}	
		if((aiming||aimamount>0||whichgun==grenade)&&whichgun!=nogun){
			if(aiming&&targetanimation!=joganim){
				if(aimamount<1)aimamount+=multiplier*4;
				if(aimamount>1)aimamount=1;
			}
			if(!aiming||targetanimation==joganim){
				if(aimamount>0)aimamount-=multiplier*4;
				if(aimamount<0)aimamount=0;
			}
			if(grenphase==1){
				if(grenamount<1)grenamount+=multiplier*4;
				if(grenamount>1)grenamount=1;
			}
			if(grenphase==0){
				if(grenamount>0)grenamount-=multiplier*4;
				if(grenamount<0)grenamount=0;
			}
			rotatearound=skeleton.joints[skeleton.jointlabels[neck]].position;
			for(int i=0;i<skeleton.num_joints;i++){
				if(skeleton.joints[i].label!=righthand&&skeleton.joints[i].label!=rightelbow&&skeleton.joints[i].label!=rightwrist&&skeleton.joints[i].label!=lefthand&&skeleton.joints[i].label!=leftelbow&&skeleton.joints[i].label!=leftwrist){
				}else{
					if(whichgun==sniperrifle){
						oldpos=skeleton.joints[i].position;
						skeleton.joints[i].position=animation[rifleaimanim].position[i][0];
						if(currentanimation==crouchanim||targetanimation==crouchanim){
							skeleton.joints[i].position-=(animation[idleanim].position[skeleton.jointlabels[neck]][0]-skeleton.joints[skeleton.jointlabels[neck]].position);
						}
						skeleton.joints[i].position+=facingright*0.1;
						skeleton.joints[i].position=rotatearound+DoRotation(skeleton.joints[i].position-rotatearound,(playerrotation2/2-10)*aimamount,0,0);
						skeleton.joints[i].position=skeleton.joints[i].position*(aimamount)+oldpos*(1-aimamount);
					}
					if(whichgun==shotgun){
						oldpos=skeleton.joints[i].position;
						skeleton.joints[i].position=animation[assaultrifleaimanim].position[i][0];
						if(currentanimation==crouchanim||targetanimation==crouchanim)skeleton.joints[i].position-=(animation[idleanim].position[skeleton.jointlabels[neck]][0]-skeleton.joints[skeleton.jointlabels[neck]].position);
						skeleton.joints[i].position=rotatearound+DoRotation(skeleton.joints[i].position-rotatearound,(playerrotation2/2)*aimamount,0,0);
						if(currentanimation==crouchanim||targetanimation==crouchanim)skeleton.joints[i].position+=facingdown*.2;
						else skeleton.joints[i].position-=facingdown*.02;
						if(skeleton.joints[i].label==righthand)skeleton.joints[i].position=skeleton.joints[skeleton.jointlabels[lefthand]].position-facinghalf*2;
						skeleton.joints[i].position=skeleton.joints[i].position*(aimamount)+oldpos*(1-aimamount);
					}
					if(whichgun==assaultrifle){
						oldpos=skeleton.joints[i].position;
						skeleton.joints[i].position=animation[assaultrifleaimanim].position[i][0];
						if(currentanimation==crouchanim||targetanimation==crouchanim)skeleton.joints[i].position-=(animation[idleanim].position[skeleton.jointlabels[neck]][0]-skeleton.joints[skeleton.jointlabels[neck]].position);
						skeleton.joints[i].position=rotatearound+DoRotation(skeleton.joints[i].position-rotatearound,(playerrotation2/2)*aimamount,0,0);
						if(currentanimation==crouchanim||targetanimation==crouchanim)skeleton.joints[i].position+=facingdown*.2;
						else skeleton.joints[i].position-=facingdown*.02;
						if(skeleton.joints[i].label==righthand)skeleton.joints[i].position=skeleton.joints[skeleton.jointlabels[lefthand]].position-facinghalf*2;
						skeleton.joints[i].position=skeleton.joints[i].position*(aimamount)+oldpos*(1-aimamount);
					}
					if(whichgun==handgun1){
						oldpos=skeleton.joints[i].position;
						skeleton.joints[i].position=animation[pistolaimanim].position[i][0];
						if(currentanimation==crouchanim||targetanimation==crouchanim)skeleton.joints[i].position-=(animation[idleanim].position[skeleton.jointlabels[neck]][0]-skeleton.joints[skeleton.jointlabels[neck]].position);
						skeleton.joints[i].position=rotatearound+DoRotation(skeleton.joints[i].position-rotatearound,(playerrotation2/2)*aimamount,0,0);
						if(currentanimation==crouchanim||targetanimation==crouchanim){skeleton.joints[i].position+=facingright*.15;skeleton.joints[i].position+=facingdown*.3;}
						else skeleton.joints[i].position-=facingdown*.1;
						skeleton.joints[i].position=skeleton.joints[i].position*(aimamount)+oldpos*(1-aimamount);
					}
					if(whichgun==handgun2){
						oldpos=skeleton.joints[i].position;
						skeleton.joints[i].position=animation[pistolaimanim].position[i][0];
						if(currentanimation==crouchanim||targetanimation==crouchanim)skeleton.joints[i].position-=(animation[idleanim].position[skeleton.jointlabels[neck]][0]-skeleton.joints[skeleton.jointlabels[neck]].position);
						skeleton.joints[i].position=rotatearound+DoRotation(skeleton.joints[i].position-rotatearound,(playerrotation2/2)*aimamount,0,0);
						if(currentanimation==crouchanim||targetanimation==crouchanim){skeleton.joints[i].position+=facingright*.15;skeleton.joints[i].position+=facingdown*.3;}
						else skeleton.joints[i].position-=facingdown*.1;
						skeleton.joints[i].position=skeleton.joints[i].position*(aimamount)+oldpos*(1-aimamount);
					}
					if(whichgun==grenade){
						aimamount=1;
						oldpos=skeleton.joints[i].position;
						skeleton.joints[i].position=animation[grenadeaimanim].position[i][0];
						if(currentanimation==crouchanim||targetanimation==crouchanim)skeleton.joints[i].position-=(animation[idleanim].position[skeleton.jointlabels[neck]][0]-skeleton.joints[skeleton.jointlabels[neck]].position);
						skeleton.joints[i].position=skeleton.joints[i].position*(aimamount)+oldpos*(1-aimamount);
					}
					if(whichgun==grenade&&grenamount>0){
						oldpos=skeleton.joints[i].position;
						skeleton.joints[i].position=animation[grenadechargeanim].position[i][0];
						if(currentanimation==crouchanim||targetanimation==crouchanim)skeleton.joints[i].position-=(animation[idleanim].position[skeleton.jointlabels[neck]][0]-skeleton.joints[skeleton.jointlabels[neck]].position);
						skeleton.joints[i].position=skeleton.joints[i].position*(grenamount)+oldpos*(1-grenamount);
					}
					if(thirdperson||who!=0)skeleton.joints[i].position+=facingdown*.4;
					if(currentanimation!=crouchanim)skeleton.joints[i].position-=facinghalf*recoil*.35;
					if(currentanimation==crouchanim)skeleton.joints[i].position-=facinghalf*recoil*.1;
				}
			}
		}
		//Whack
		if(attackframe>-1&&whichgun!=grenade){
			for(int i=0;i<skeleton.num_joints;i++){
				if(!skeleton.joints[i].lower){
					if(attackframe==animation[riflehitanim].numframes)skeleton.joints[i].position=skeleton.joints[i].position*(attacktarget)+animation[riflehitanim].position[i][attackframe-1]*(1-attacktarget);
					if(attackframe>0&&attackframe<animation[riflehitanim].numframes)skeleton.joints[i].position=animation[riflehitanim].position[i][attackframe-1]*(1-attacktarget)+animation[riflehitanim].position[i][attackframe]*(attacktarget);
					if(attackframe==0)skeleton.joints[i].position=skeleton.joints[i].position*(1-attacktarget)+animation[riflehitanim].position[i][attackframe]*(attacktarget);
				}
			}
			if(attackframe==0)attacktarget+=multiplier*animation[riflehitanim].speed[attackframe]*2;
			if(attackframe>0)attacktarget+=multiplier*animation[riflehitanim].speed[attackframe-1]*2;
			if(attacktarget>1){
				attacktarget=0;
				attackframe++;
				if(attackframe>animation[riflehitanim].numframes)attackframe=-1;
			}
		}
		//Throw grenade
		if(attackframe>-1&&whichgun==grenade&&ammo>0){
			for(int i=0;i<skeleton.num_joints;i++){
				if(!skeleton.joints[i].lower){
					if(attackframe==animation[grenadethrowanim].numframes)skeleton.joints[i].position=skeleton.joints[i].position*(attacktarget)+animation[grenadethrowanim].position[i][attackframe-1]*(1-attacktarget);
					if(attackframe>0&&attackframe<animation[grenadethrowanim].numframes)skeleton.joints[i].position=animation[grenadethrowanim].position[i][attackframe-1]*(1-attacktarget)+animation[grenadethrowanim].position[i][attackframe]*(attacktarget);
					if(attackframe==0)skeleton.joints[i].position=skeleton.joints[i].position*(1-attacktarget)+animation[grenadethrowanim].position[i][attackframe]*(attacktarget);
				}
			}
			if(attackframe==0)attacktarget+=multiplier*animation[grenadethrowanim].speed[attackframe]*2;
			if(attackframe>0)attacktarget+=multiplier*animation[grenadethrowanim].speed[attackframe-1]*2;
			if(attacktarget>1){
				attacktarget=0;
				attackframe++;
				if(attackframe>animation[grenadethrowanim].numframes){
					attackframe=-1;
				}
				if(attackframe==animation[grenadethrowanim].numframes-1&&ammo>0){
					ammo=-1;
					sprites.MakeSprite(grenadesprite, 1, 1, 1, 1, DoRotation(skeleton.joints[skeleton.jointlabels[righthand]].position,0,playerrotation,0)+playercoords, DoRotation(facing,0,playerrotation,0)*30+velocity, 1);
					sprites.MakeSprite(spoonsprite, 1, 1, 1, 1, DoRotation(skeleton.joints[skeleton.jointlabels[righthand]].position,0,playerrotation,0)+playercoords, DoRotation(facing,0,playerrotation,0)*10+velocity, 1);
					sprites.MakeSprite(pinsprite, 1, 1, 1, 1, DoRotation(skeleton.joints[skeleton.jointlabels[lefthand]].position,0,playerrotation,0)+playercoords, facing*.1+velocity, 1);
					XYZ soundsource=DoRotation(skeleton.joints[skeleton.jointlabels[righthand]].position,0,playerrotation,0)+playercoords;
					float gLoc[3];
					gLoc[0]=soundsource.x/soundscalefactor;
					gLoc[1]=soundsource.y/soundscalefactor;
					gLoc[2]=soundsource.z/soundscalefactor;
					alSourcefv(gSourceID[grenadethrowsound], AL_POSITION, gLoc);
					alSourcePlay(gSourceID[grenadethrowsound]);
				}
			}
		}
		
		rotatearound=skeleton.joints[skeleton.jointlabels[abdomen]].position;
		if(who==0)
		for(int i=0;i<skeleton.num_joints;i++){
		 	if(skeleton.joints[i].lower==0)
		 		skeleton.joints[i].position=rotatearound+DoRotation(skeleton.joints[i].position-rotatearound,playerrotation2/2,0,0);
		}
		if(who==0)
		for(int i=0;i<skeleton.num_joints;i++){
		 	if(skeleton.joints[i].lower==1&&skeleton.joints[i].label!=groin)
		 		skeleton.joints[i].position=DoRotation(skeleton.joints[i].position,0,playerlowrotation-playerrotation,0);
		}
		//head facing
		if(who==0){
		rotatearound=skeleton.joints[skeleton.jointlabels[neck]].position;
		skeleton.joints[skeleton.jointlabels[head]].position=rotatearound+DoRotation(skeleton.joints[skeleton.jointlabels[head]].position-rotatearound,playerrotation2/2,0,0);
		}
		
		skeleton.DoConstraints();
		
		//Reload
		if(whichgun!=nogun&&whichgun!=knife){
			if(reloading>0){
				aiming=0;
				reloading-=multiplier;
			}
			if(ammo<0&&reloads[whichgun]>0&&reloading<=0){
				if(whichgun!=grenade){
					float gLoc[3];
					ALint tempint;
					gLoc[0]=playercoords.x/soundscalefactor;
					gLoc[1]=playercoords.y/soundscalefactor;
					gLoc[2]=playercoords.z/soundscalefactor;
#ifdef DEBIAN_NEEDS_TO_UPDATE_THEIR_OPENAL					
					alGetSourceiv(gSourceID[reloadsound], AL_SOURCE_STATE, &tempint);
#else					
					alGetSourcei(gSourceID[reloadsound], AL_SOURCE_STATE, &tempint);
#endif

					if (tempint != AL_PLAYING){
						alSourcefv(gSourceID[reloadsound], AL_POSITION, gLoc);
						alSourcePlay(gSourceID[reloadsound]);
					}
				}
				reloading=3;
				aiming=0;
				if(whichgun==sniperrifle)ammo=5;
				if(whichgun==assaultrifle)ammo=25;
				if(whichgun==handgun1)ammo=12;
				if(whichgun==handgun2)ammo=16;
				if(whichgun==grenade){ammo=1; reloading=1;}
				if(whichgun==shotgun)ammo=6;
				reloads[whichgun]--;
			}
			if(reloads[whichgun]==0&&whichgun==grenade&&ammo<=0){
				whichgun=nogun;	
			}
			if(reloading<0){
				reloading=0;
				aiming=1;
			}
		}
	}
}

void 	Person::DoAnimationslite(int who){
	if(target>1&&!skeleton.free){
		//Footstep sounds
		if(who==0&&(targetanimation==joganim||targetanimation==walkanim)&&(targetframe==0||targetframe==8)&&visions==0&&(onground||abs(velocity.y)<1)){
			int whichsound;
			float gLoc[3];
			gLoc[0]=playercoords.x/soundscalefactor;
			gLoc[1]=playercoords.y/soundscalefactor;
			gLoc[2]=playercoords.z/soundscalefactor;
			whichsound=footstepsound+abs(Random())%5;
			alSourcefv(gSourceID[whichsound], AL_POSITION, gLoc);
			alSourcePlay(gSourceID[whichsound]);
		}
		if(targetanimation==zombieeatanim&&(targetframe==3)){
			float gLoc[3];
			XYZ soundpoint=(DoRotation(skeleton.joints[skeleton.jointlabels[head]].position,0,playerrotation,0)+playercoords);
			gLoc[0]=soundpoint.x/soundscalefactor;
			gLoc[1]=soundpoint.y/soundscalefactor;
			gLoc[2]=soundpoint.z/soundscalefactor;
			alSourcefv(gSourceID[bodyhitsound], AL_POSITION, gLoc);
			alSourcePlay(gSourceID[bodyhitsound]);
			bleeding=1;
			bleeddelay=1;
			bjoint1=&skeleton.joints[skeleton.jointlabels[head]];
			bjoint2=&skeleton.joints[skeleton.jointlabels[neck]];
		}
		targetframe=currentframe;
		currentanimation=targetanimation;
		if(!backwardsanim){targetframe++;
		if(targetframe>animation[currentanimation].numframes-1)targetframe=0;}
		if(backwardsanim){targetframe--;
		if(targetframe<0)targetframe=animation[currentanimation].numframes-1;}
		target=0;
		if((currentanimation==getupfrontanim||currentanimation==getupbackanim)&&targetframe==0){
			targetanimation=idleanim;
		}
	}
	
	if(!skeleton.free){
		if(currentanimation!=lyinganim){
			if(animation[targetanimation].speed[currentframe]>animation[currentanimation].speed[currentframe])
				target+=multiplier*animation[targetanimation].speed[currentframe]*speed;
			if(animation[targetanimation].speed[currentframe]<=animation[currentanimation].speed[currentframe])
				target+=multiplier*animation[currentanimation].speed[currentframe]*speed;
		}
		if(currentanimation==lyinganim){
			target+=multiplier*animation[targetanimation].speed[targetframe]*speed;
		}		
		if(((currentanimation==crouchanim)&&(targetanimation!=crouchanim))||((currentanimation!=crouchanim)&&(targetanimation==crouchanim)))target+=multiplier*animation[crouchanim].speed[0]*2;
		if(currentanimation==idleanim&&targetanimation==idleanim)target-=multiplier*animation[idleanim].speed[0]/2;
		
		if(target>1)currentframe=targetframe;
	}
}

void 	Person::DoStuff(int who){
	int moveanim;

	if((targetanimation!=idleanim&&targetanimation!=crouchanim)||visions==1)playerlowrotation=playerrotation;
	if(targetanimation!=crouchanim&&currentanimation!=crouchanim){
		if(playerrotation>playerlowrotation+50){playerlowrotation=playerrotation-50; targetanimation=walkanim; targetframe=0; target=0;}
		if(playerrotation<playerlowrotation-50){playerlowrotation=playerrotation+50; targetanimation=walkanim; targetframe=0; target=0;}
	}
	if(targetanimation==crouchanim||currentanimation==crouchanim){
		if(playerrotation>playerlowrotation+70){playerrotation=playerlowrotation+70;}
		if(playerrotation<playerlowrotation-70){playerrotation=playerlowrotation-70;}
	}
	if(who==0)camera.rotation=180-playerrotation;
	
	if(who!=0&&visions==0){
		if(targetanimation!=walkanim&&targetanimation!=zombiewalkanim)speed=1.0*speedmult;
		if(targetanimation==walkanim||targetanimation==zombiewalkanim)speed=1.8*speedmult;
		playerlowrotation=playerrotation;
	}
	
	//Do controls
	if(who==0&&targetanimation!=diveanim&&targetanimation!=throwanim&&targetanimation!=thrownanim&&currentanimation!=diveanim&&currentanimation!=getupfrontanim){
		backwardsanim=0;
		if(visions==1)speed=40;
		if(visions==0&&targetanimation==joganim)speed=2.2;
		if(visions==0&&targetanimation!=joganim)speed=1.3;
		if(visions==0&&targetanimation==walkanim)speed=2.5;
		unsigned char	theKeyMap[16];
		GetKeys( ( unsigned long * )theKeyMap );
		
		if(IsKeyDown( theKeyMap, MAC_SHIFT_KEY )||visions==1){
			moveanim=joganim;
		}else{
			moveanim=walkanim;
		}
		if(IsKeyDown( theKeyMap, MAC_CONTROL_KEY )&&currentanimation==idleanim&&targetanimation==idleanim){
			targetanimation=crouchanim;
			target=0;
		}
		if(!IsKeyDown( theKeyMap, MAC_CONTROL_KEY )&&currentanimation==crouchanim&&targetanimation==crouchanim){
			targetanimation=idleanim;
			target=0;
		}
		if((onground||(who==0&&visions==1))&&currentanimation!=crouchanim){
			if ( IsKeyDown( theKeyMap, forwardskey )&&!( IsKeyDown( theKeyMap, backwardskey ))){
				if(targetanimation!=moveanim)targetframe=0;
				targetanimation=moveanim;		
			}
			if ( IsKeyDown( theKeyMap, rightkey )&&!( IsKeyDown( theKeyMap, leftkey ))){
				if(targetanimation!=moveanim)targetframe=0;
				targetanimation=moveanim;
				playerlowrotation-=90;
				if(IsKeyDown( theKeyMap, forwardskey ))playerlowrotation+=45;
				if(IsKeyDown( theKeyMap, backwardskey ))playerlowrotation-=235;
			}
			if ( IsKeyDown( theKeyMap, leftkey )&&!( IsKeyDown( theKeyMap, rightkey ))){
				if(targetanimation!=moveanim)targetframe=0;
				targetanimation=moveanim;
				playerlowrotation+=90;
				if(IsKeyDown( theKeyMap, forwardskey ))playerlowrotation-=45;
				if(IsKeyDown( theKeyMap, backwardskey ))playerlowrotation+=235;
			}
			if ( IsKeyDown( theKeyMap, backwardskey )){
				if(targetanimation!=moveanim)targetframe=0;
				targetanimation=moveanim;
				backwardsanim=1;
			}
		}
		//air control
		if(!onground){
			float oldplayerrotation=playerrotation;
			if ( IsKeyDown( theKeyMap, forwardskey )&&!( IsKeyDown( theKeyMap, backwardskey ))){
				facing=0;
				facing.z=1;		
				facing=DoRotation(facing,0,playerrotation,0);
				velocity=velocity+facing*multiplier*4;
			}
			if ( IsKeyDown( theKeyMap, rightkey )&&!( IsKeyDown( theKeyMap, leftkey ))){
				playerrotation-=90;
				if(IsKeyDown( theKeyMap, forwardskey ))playerrotation+=45;
				if(IsKeyDown( theKeyMap, backwardskey ))playerrotation-=45;
				facing=0;
				facing.z=1;		
				facing=DoRotation(facing,0,playerrotation,0);
				velocity=velocity+facing*multiplier*4;
			}
			if ( IsKeyDown( theKeyMap, leftkey )&&!( IsKeyDown( theKeyMap, rightkey ))){
				playerrotation+=90;
				if(IsKeyDown( theKeyMap, forwardskey ))playerrotation-=45;
				if(IsKeyDown( theKeyMap, backwardskey ))playerrotation+=45;
				facing=0;
				facing.z=1;		
				facing=DoRotation(facing,0,playerrotation,0);
				velocity=velocity+facing*multiplier*4;
			}
			if ( IsKeyDown( theKeyMap, backwardskey )&&!IsKeyDown( theKeyMap, forwardskey )&&!IsKeyDown( theKeyMap, leftkey )&&!IsKeyDown( theKeyMap, rightkey )){
				playerrotation+=180;
				facing=0;
				facing.z=1;		
				facing=DoRotation(facing,0,playerrotation,0);
				velocity=velocity+facing*multiplier*4;
			}
			playerrotation=oldplayerrotation;
		}
		if(!IsKeyDown( theKeyMap, forwardskey )&&!IsKeyDown( theKeyMap, leftkey )&&!IsKeyDown( theKeyMap, rightkey )&&!IsKeyDown( theKeyMap, backwardskey )&&(targetanimation==joganim||targetanimation==walkanim)){
			if(!(targetanimation==walkanim&&(targetframe==0||targetframe==2))){
				targetanimation=idleanim;
				targetframe=0;
				target=0;
			}
		}
	}
	
	
	facing=0;
	facing.z=1;
	
	facing=DoRotation(facing,0,playerlowrotation,0);
	if(backwardsanim)facing*=-1;
	
	if(onground){
		velocity=0;
	}
	if(((currentanimation==joganim||currentanimation==zombiejoganim||currentanimation==diveanim)&&onground)||(who==0&&visions==1&&((currentanimation==joganim||currentanimation==walkanim||currentanimation==diveanim)||(currentanimation==zombiejoganim||currentanimation==zombiewalkanim)))){
		playercoords+=facing*multiplier*15*speed;
		velocity.x=facing.x*15*speed;
		velocity.z=facing.z*15*speed;
	}
	if((currentanimation==walkanim||currentanimation==zombiewalkanim)&&onground&&(who!=0||visions==0)){
		playercoords+=facing*multiplier*4*speed;
		velocity.x=facing.x*4*speed;
		velocity.z=facing.z*4*speed;
	}
}

void Person::FindRotationGun(XYZ start, XYZ target)
{
	XYZ temppoint1,temppoint2,tempforward;
	float distance;
	
	temppoint1=start;
	temppoint2=target;
	distance=findDistance(temppoint1,temppoint2);
	gunrotate2=asin((temppoint1.y-temppoint2.y)/distance)*rad2deg;
	temppoint1.y=0;
	temppoint2.y=0;
	gunrotate1=acos((temppoint1.z-temppoint2.z)/findDistance(temppoint1,temppoint2))*rad2deg;
	if(temppoint1.x>temppoint2.x)gunrotate1=360-gunrotate1;
	tempforward=target-start;
	tempforward=DoRotation(tempforward,-90,0,0);
	tempforward=DoRotation(tempforward,0,gunrotate1-90,0);
	tempforward=DoRotation(tempforward,0,0,gunrotate2-90);
	tempforward.y=0;
	Normalise(&tempforward);
	gunrotate3=acos(0-tempforward.z)*rad2deg;
	if(0>tempforward.x)gunrotate3=360-gunrotate3;
}

extern Model skeletonmodels[10];
extern Costume costume[2];
int Person::DrawSkeleton(int who){
	//Guns
	if(whichgun==sniperrifle){
		FindRotationGun(skeleton.joints[skeleton.jointlabels[righthand]].position,skeleton.joints[skeleton.jointlabels[lefthand]].position);
		glPushMatrix();
				glTranslatef(	skeleton.joints[skeleton.jointlabels[righthand]].position.x,
								skeleton.joints[skeleton.jointlabels[righthand]].position.y,
								skeleton.joints[skeleton.jointlabels[righthand]].position.z);
				glRotatef(-gunrotate1+90,0,1,0);
				glRotatef(-gunrotate2+90,0,0,1);
				glRotatef(-gunrotate3,0,1,0);
				gunmodels[sniperriflemodel].draw();
			glPopMatrix();
	}
	
	if(whichgun==shotgun){
		FindRotationGun(skeleton.joints[skeleton.jointlabels[righthand]].position,skeleton.joints[skeleton.jointlabels[lefthand]].position);
		glPushMatrix();
				glTranslatef(	skeleton.joints[skeleton.jointlabels[righthand]].position.x,
								skeleton.joints[skeleton.jointlabels[righthand]].position.y,
								skeleton.joints[skeleton.jointlabels[righthand]].position.z);
				glRotatef(2,1,0,0);
				glRotatef(-gunrotate1+90+1,0,1,0);
				glRotatef(-gunrotate2+90,0,0,1);
				glRotatef(-gunrotate3,0,1,0);
				glTranslatef(0,-.4,0);
				gunmodels[shotgunmodel].draw();
			glPopMatrix();
	}
	
	if(whichgun==assaultrifle){
		FindRotationGun(skeleton.joints[skeleton.jointlabels[righthand]].position,skeleton.joints[skeleton.jointlabels[lefthand]].position);
		glPushMatrix();
				glTranslatef(	skeleton.joints[skeleton.jointlabels[righthand]].position.x,
								skeleton.joints[skeleton.jointlabels[righthand]].position.y,
								skeleton.joints[skeleton.jointlabels[righthand]].position.z);
				glRotatef(-gunrotate1+90,0,1,0);
				glRotatef(-gunrotate2+90,0,0,1);
				glRotatef(-gunrotate3,0,1,0);
				gunmodels[assaultriflemodel].draw();
			glPopMatrix();
	}
	
	if(whichgun==handgun1){
		if(!thirdperson&&who==0)FindRotationGun(skeleton.joints[skeleton.jointlabels[righthand]].position,(skeleton.joints[skeleton.jointlabels[head]].position*.65+skeleton.joints[skeleton.jointlabels[neck]].position*.35));
		if(thirdperson||who!=0)FindRotationGun(skeleton.joints[skeleton.jointlabels[righthand]].position,(skeleton.joints[skeleton.jointlabels[head]].position*.35+skeleton.joints[skeleton.jointlabels[neck]].position*.65));
		glPushMatrix();
				glTranslatef(	skeleton.joints[skeleton.jointlabels[righthand]].position.x,
								skeleton.joints[skeleton.jointlabels[righthand]].position.y,
								skeleton.joints[skeleton.jointlabels[righthand]].position.z);
				glRotatef(-gunrotate1+90-1.5,0,1,0);
				glRotatef(-gunrotate2+90,0,0,1);
				glRotatef(-gunrotate3,0,1,0);
				glTranslatef(0,0,.15);
				gunmodels[handgunbasemodel].draw();
				glTranslatef(0,-recoil*.3,0);
				gunmodels[handgunslidemodel].draw();
			glPopMatrix();
	}
	
	if(whichgun==handgun2){
		if(!thirdperson&&who==0)FindRotationGun(skeleton.joints[skeleton.jointlabels[righthand]].position,(skeleton.joints[skeleton.jointlabels[head]].position*.65+skeleton.joints[skeleton.jointlabels[neck]].position*.35));
		if(thirdperson||who!=0)FindRotationGun(skeleton.joints[skeleton.jointlabels[righthand]].position,(skeleton.joints[skeleton.jointlabels[head]].position*.35+skeleton.joints[skeleton.jointlabels[neck]].position*.65));
		glPushMatrix();
				glTranslatef(	skeleton.joints[skeleton.jointlabels[righthand]].position.x,
								skeleton.joints[skeleton.jointlabels[righthand]].position.y,
								skeleton.joints[skeleton.jointlabels[righthand]].position.z);
				glRotatef(-gunrotate1+90-1.5,0,1,0);
				glRotatef(-gunrotate2+90,0,0,1);
				glRotatef(-gunrotate3,0,1,0);
				glTranslatef(0,0,.15);
				gunmodels[handgun2basemodel].draw();
				glTranslatef(0,-recoil*.3,0);
				gunmodels[handgun2slidemodel].draw();
			glPopMatrix();
	}
	
	if(whichgun==grenade){
		glPushMatrix();
				glTranslatef(	skeleton.joints[skeleton.jointlabels[righthand]].position.x,
								skeleton.joints[skeleton.jointlabels[righthand]].position.y,
								skeleton.joints[skeleton.jointlabels[righthand]].position.z);
				glRotatef(-90,1,0,0);
				glTranslatef(0,0,.05);
				if(reloading<=0){
					gunmodels[grenadebasemodel].draw();
					if(grenphase==0)gunmodels[grenadepinmodel].draw();
					glTranslatef(0,0,.005);
					gunmodels[grenadespoonmodel].draw();
				}
			glPopMatrix();
	}
	
	if(whichgun==grenade){
		glPushMatrix();
				glTranslatef(	skeleton.joints[skeleton.jointlabels[lefthand]].position.x,
								skeleton.joints[skeleton.jointlabels[lefthand]].position.y,
								skeleton.joints[skeleton.jointlabels[lefthand]].position.z);
				glRotatef(-90,1,0,0);
				glTranslatef(0,0,-.15);
				if(reloading<=0){
					if(grenphase==1)gunmodels[grenadepinmodel].draw();
				}
			glPopMatrix();
	}
	if(!litup){
		glDisable(GL_LIGHT1);
	}
	if(litup){
		GLfloat LightAmbient[]=	{ 0,0,0, 1.0f};
		GLfloat LightDiffuse[]=	{ 1,1,1, 1.0f };
		XYZ lightpoint;
		lightpoint=skeleton.joints[skeleton.jointlabels[lefthand]].position;
		GLfloat LightPosition[]=	{lightpoint.x,lightpoint.y,lightpoint.z,0};
		glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
		glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		
		glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);	
		glEnable(GL_LIGHT1);
		
		litup=0;
	}
	
	//Find forward vectors
	if(who==0||skeleton.free!=0||skeleton.offset!=0||whichgun!=nogun||currentanimation==lyinganim||((currentanimation==getupfrontanim||currentanimation==getupbackanim)&&targetanimation==idleanim)){
	if(!(skeleton.free==1&&longdead<=0)){
	if(skeleton.offset&&skeleton.free<1){
		XYZ normal;
		skeleton.offset=0;
		for(int i=0;i<skeleton.num_joints;i++){
			skeleton.joints[i].oldposition=skeleton.joints[i].position;
			skeleton.joints[i].position+=skeleton.joints[i].offset;
			if(findLengthfast(skeleton.joints[i].offset)<multiplier*multiplier*25)skeleton.joints[i].offset=0;
			else{
				normal=skeleton.joints[i].offset;
				Normalise(&normal);
				skeleton.offset=1;
				skeleton.joints[i].offset-=normal*multiplier*5;
			}
		}
		skeleton.DoConstraints();
	}
	CrossProduct(skeleton.joints[skeleton.forwardjoints[1]].position-skeleton.joints[skeleton.forwardjoints[0]].position,skeleton.joints[skeleton.forwardjoints[2]].position-skeleton.joints[skeleton.forwardjoints[0]].position,&skeleton.forward);
	Normalise(&skeleton.forward);
	
	CrossProduct(skeleton.joints[skeleton.lowforwardjoints[1]].position-skeleton.joints[skeleton.lowforwardjoints[0]].position,skeleton.joints[skeleton.lowforwardjoints[2]].position-skeleton.joints[skeleton.lowforwardjoints[0]].position,&skeleton.lowforward);
	Normalise(&skeleton.lowforward);
	
	//Special forwards
	skeleton.specialforward[0]=skeleton.forward;
	
	skeleton.specialforward[1]=skeleton.joints[skeleton.jointlabels[rightshoulder]].position+skeleton.joints[skeleton.jointlabels[rightwrist]].position;
	skeleton.specialforward[1]=skeleton.joints[skeleton.jointlabels[rightelbow]].position-skeleton.specialforward[1]/2;
	skeleton.specialforward[1]+=skeleton.forward*.2;
	Normalise(&skeleton.specialforward[1]);
	skeleton.specialforward[2]=skeleton.joints[skeleton.jointlabels[leftshoulder]].position+skeleton.joints[skeleton.jointlabels[leftwrist]].position;
	skeleton.specialforward[2]=skeleton.joints[skeleton.jointlabels[leftelbow]].position-skeleton.specialforward[2]/2;
	skeleton.specialforward[2]+=skeleton.forward*.2;
	Normalise(&skeleton.specialforward[2]);

	if(who==0&&aimamount>0&&health==100&&whichgun!=nogun){
		//Facing
		XYZ facing;
		XYZ facingdown;
		//Facing
		facing=0;
		facing.z=1;
		
		facing=DoRotation(facing,camera.rotation2,0,0);
		facingdown=DoRotation(facing,90,0,0);
		skeleton.specialforward[1]=skeleton.specialforward[1]*(1-aimamount)+facingdown*aimamount;
		skeleton.specialforward[2]=skeleton.specialforward[2]*(1-aimamount)+facingdown*aimamount;
	}
	
	skeleton.specialforward[3]=skeleton.joints[skeleton.jointlabels[righthip]].position+skeleton.joints[skeleton.jointlabels[rightankle]].position;
	skeleton.specialforward[3]=skeleton.specialforward[3]/2-skeleton.joints[skeleton.jointlabels[rightknee]].position;
	skeleton.specialforward[3]+=skeleton.lowforward*.2;
	Normalise(&skeleton.specialforward[3]);
	skeleton.specialforward[4]=skeleton.joints[skeleton.jointlabels[lefthip]].position+skeleton.joints[skeleton.jointlabels[leftankle]].position;
	skeleton.specialforward[4]=skeleton.specialforward[4]/2-skeleton.joints[skeleton.jointlabels[leftknee]].position;
	skeleton.specialforward[4]+=skeleton.lowforward*.2;
	Normalise(&skeleton.specialforward[4]);

	//Find joint rotations
	for(int i=0;i<skeleton.num_joints;i++){
		if(skeleton.joints[i].hasparent&&skeleton.joints[i].visible)
		{
			skeleton.FindRotationJoint(i);
		}
	}
	for(int i=0;i<skeleton.num_muscles;i++){
		if(skeleton.muscles[i].visible)
		{
			skeleton.FindRotationMuscle(i);
		}
	}}}
	for(int i=0;i<skeleton.num_joints;i++){
		if(skeleton.joints[i].hasparent&&skeleton.joints[i].visible)
		{
			glPushMatrix();
				glTranslatef(	(skeleton.joints[i].position.x+skeleton.joints[i].parent->position.x)/2,
								(skeleton.joints[i].position.y+skeleton.joints[i].parent->position.y)/2,
								(skeleton.joints[i].position.z+skeleton.joints[i].parent->position.z)/2);
				glRotatef(-skeleton.joints[i].rotate1+90,0,1,0);
				glRotatef(-skeleton.joints[i].rotate2+90,0,0,1);
				glRotatef(-skeleton.joints[i].rotate3,0,1,0);
				if(skeleton.joints[i].modelnum==0&&(who!=0||thirdperson))skeletonmodels[skeleton.joints[i].modelnum].draw(costume[whichcostume].headcolor[0],costume[whichcostume].headcolor[1],costume[whichcostume].headcolor[2]);
				if(skeleton.joints[i].modelnum==0&&(who==0&&thirdperson))skeletonmodels[9].draw();
				if(skeleton.joints[i].modelnum==1&&(who!=0||thirdperson))skeletonmodels[skeleton.joints[i].modelnum].draw(costume[whichcostume].chestcolor[0],costume[whichcostume].chestcolor[1],costume[whichcostume].chestcolor[2]);
				if(skeleton.joints[i].modelnum==2)skeletonmodels[skeleton.joints[i].modelnum].draw(costume[whichcostume].abdomencolor[0],costume[whichcostume].abdomencolor[1],costume[whichcostume].abdomencolor[2]);
				if(skeleton.joints[i].modelnum==3)skeletonmodels[skeleton.joints[i].modelnum].draw(costume[whichcostume].upperarmcolor[0],costume[whichcostume].upperarmcolor[1],costume[whichcostume].upperarmcolor[2]);
				if(skeleton.joints[i].modelnum==4)skeletonmodels[skeleton.joints[i].modelnum].draw(costume[whichcostume].lowerarmcolor[0],costume[whichcostume].lowerarmcolor[1],costume[whichcostume].lowerarmcolor[2]);
				if(skeleton.joints[i].modelnum==5)skeletonmodels[skeleton.joints[i].modelnum].draw(costume[whichcostume].handcolor[0],costume[whichcostume].handcolor[1],costume[whichcostume].handcolor[2]);
				if(skeleton.joints[i].modelnum==6)skeletonmodels[skeleton.joints[i].modelnum].draw(costume[whichcostume].upperlegcolor[0],costume[whichcostume].upperlegcolor[1],costume[whichcostume].upperlegcolor[2]);
				if(skeleton.joints[i].modelnum==7)skeletonmodels[skeleton.joints[i].modelnum].draw(costume[whichcostume].lowerlegcolor[0],costume[whichcostume].lowerlegcolor[1],costume[whichcostume].lowerlegcolor[2]);
				if(skeleton.joints[i].modelnum==8)skeletonmodels[skeleton.joints[i].modelnum].draw(costume[whichcostume].footcolor[0],costume[whichcostume].footcolor[1],costume[whichcostume].footcolor[2]);
			glPopMatrix();
		}
	}
	
	for(int i=0;i<skeleton.num_muscles;i++){
		if(skeleton.muscles[i].visible)
		{
			glPushMatrix();
				glTranslatef(	(skeleton.muscles[i].parent1->position.x+skeleton.muscles[i].parent2->position.x)/2,
								(skeleton.muscles[i].parent1->position.y+skeleton.muscles[i].parent2->position.y)/2,
								(skeleton.muscles[i].parent1->position.z+skeleton.muscles[i].parent2->position.z)/2);
				glRotatef(-skeleton.muscles[i].rotate1+90,0,1,0);
				glRotatef(-skeleton.muscles[i].rotate2+90,0,0,1);
				glRotatef(-skeleton.muscles[i].rotate3,0,1,0);
				if(skeleton.muscles[i].parent1->modelnum==0&&(who!=0||thirdperson))skeletonmodels[skeleton.muscles[i].parent1->modelnum].draw(costume[whichcostume].headcolor[0],costume[whichcostume].headcolor[1],costume[whichcostume].headcolor[2]);
				if(skeleton.muscles[i].parent1->modelnum==0&&(who==0&&thirdperson))skeletonmodels[9].draw();
				if(skeleton.muscles[i].parent1->modelnum==1&&(who!=0||thirdperson))skeletonmodels[skeleton.muscles[i].parent1->modelnum].draw(costume[whichcostume].chestcolor[0],costume[whichcostume].chestcolor[1],costume[whichcostume].chestcolor[2]);
				if(skeleton.muscles[i].parent1->modelnum==2)skeletonmodels[skeleton.muscles[i].parent1->modelnum].draw(costume[whichcostume].abdomencolor[0],costume[whichcostume].abdomencolor[1],costume[whichcostume].abdomencolor[2]);
				if(skeleton.muscles[i].parent1->modelnum==3)skeletonmodels[skeleton.muscles[i].parent1->modelnum].draw(costume[whichcostume].upperarmcolor[0],costume[whichcostume].upperarmcolor[1],costume[whichcostume].upperarmcolor[2]);
				if(skeleton.muscles[i].parent1->modelnum==4)skeletonmodels[skeleton.muscles[i].parent1->modelnum].draw(costume[whichcostume].lowerarmcolor[0],costume[whichcostume].lowerarmcolor[1],costume[whichcostume].lowerarmcolor[2]);
				if(skeleton.muscles[i].parent1->modelnum==5)skeletonmodels[skeleton.muscles[i].parent1->modelnum].draw(costume[whichcostume].handcolor[0],costume[whichcostume].handcolor[1],costume[whichcostume].handcolor[2]);
				if(skeleton.muscles[i].parent1->modelnum==6)skeletonmodels[skeleton.muscles[i].parent1->modelnum].draw(costume[whichcostume].upperlegcolor[0],costume[whichcostume].upperlegcolor[1],costume[whichcostume].upperlegcolor[2]);
				if(skeleton.muscles[i].parent1->modelnum==7)skeletonmodels[skeleton.muscles[i].parent1->modelnum].draw(costume[whichcostume].lowerlegcolor[0],costume[whichcostume].lowerlegcolor[1],costume[whichcostume].lowerlegcolor[2]);
				if(skeleton.muscles[i].parent1->modelnum==8)skeletonmodels[skeleton.muscles[i].parent1->modelnum].draw(costume[whichcostume].footcolor[0],costume[whichcostume].footcolor[1],costume[whichcostume].footcolor[2]);
				
		glPopMatrix();
		}
	}
	if(whichgun==knife){
		glPushMatrix();
				glTranslatef(	skeleton.joints[skeleton.jointlabels[righthand]].position.x,
								skeleton.joints[skeleton.jointlabels[righthand]].position.y,
								skeleton.joints[skeleton.jointlabels[righthand]].position.z);
				glRotatef(-skeleton.joints[skeleton.jointlabels[righthand]].rotate1+90-1.5,0,1,0);
				glRotatef(-skeleton.joints[skeleton.jointlabels[righthand]].rotate2+90,0,0,1);
				glRotatef(-skeleton.joints[skeleton.jointlabels[righthand]].rotate3,0,1,0);
				glTranslatef(0,-.2,0);
				gunmodels[knifemodel].draw();
			glPopMatrix();
	}
	if(skeleton.offset&&skeleton.free<1){
		for(int i=0;i<skeleton.num_joints;i++){
			skeleton.joints[i].position=skeleton.joints[i].oldposition;
		}
	}
	
	glDisable(GL_LIGHT1);
	
	return 0;
}

