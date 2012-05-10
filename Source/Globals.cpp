#include "Quaternions.h"
#include "Constants.h"
#include "Models.h"
#include "Person.h"
#include "Game.h"

float sinefluct;
float sinefluctprog;

double multiplier=0;

unsigned int gSourceID[100]; // hundred source IDs
unsigned int gSampleSet[100]; // hundred sample set ID numbers

float precipitationhorz;
float precipitationvert;
float precipitationdensity;

int environment;

float soundscalefactor;
int slomo;

Animation animation[30];
Model skeletonmodels[10];
Model gunmodels[11];
Costume costume[10];
Sprites sprites;
Decals decals;
int thirdperson;
int nocolors;
int visions;
Camera camera;
float rad2deg;
Skeleton testskeleton;

float snowdelay;

float camerashake;

Fog fog;

int forwardskey;
int backwardskey;
int leftkey;
int rightkey;
int aimkey;
int psychicaimkey;
int psychickey;

bool blood;

float fogcolorr;
float fogcolorg;
float fogcolorb;

float d;
float a1,a2,a3;
float total,denom,mu;
XYZ n,pa1,pa2,pa3;

float u0, u1, u2;
float v0, v1, v2;
float a, b;
float max;
int i, j;
bool bInter;
float pointv[3];
float p1v[3];
float p2v[3];
float p3v[3];
float normalv[3];

XYZ vel;
XYZ midp;
XYZ newpoint1,newpoint2;

float oldlength;
float relaxlength;

float friction;
int numrepeats;
float groundlevel;
float offset;
XYZ impact;
XYZ overpoint;
XYZ underpoint;
int whichtri;
XYZ normalrotated;
bool groundish;
