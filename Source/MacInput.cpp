/**> HEADER FILES <**/
#include "MacInput.h"

/**> Mouse Stuff <**/
#ifdef OS9 
CursorDevicePtr	theCursor;
#endif

/********************> IsKeyDown() <*****/
Boolean	IsKeyDown( unsigned char *keyMap, unsigned short theKey )
{
	long	keyMapIndex;
	Boolean	isKeyDown;
	short	bitToCheck;
	
	// Calculate the key map index
	keyMapIndex = keyMap[theKey/8];
	
	// Calculate the individual bit to check
	bitToCheck = theKey%8;
	
	// Check the status of the key
	isKeyDown = ( keyMapIndex >> bitToCheck ) & 0x01;
	
	// Return the status of the key
	return isKeyDown;
	
}

#ifdef OS9 
void 	InitMouse()
{
	CursorDeviceNewDevice( &theCursor ); //Mouse
}

void 	MoveMouse(int xcoord, int ycoord, Point *mouseloc)
{
	CursorDeviceMoveTo( theCursor, xcoord, ycoord);
	GetMouse(mouseloc);
}

void DisposeMouse()
{
	CursorDeviceDisposeDevice( theCursor );//Mouse
}
#endif
