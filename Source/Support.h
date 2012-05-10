#ifndef SUPPORT_H
#define SUPPORT_H

#include <stdio.h>
#include <unistd.h>

#ifdef __APPLE__
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif
#include <AL/alut.h>

#define STUB_FUNCTION fprintf(stderr,"STUB: %s at " __FILE__ ", line %d, thread %d\n",__FUNCTION__,__LINE__,getpid())

#define fsFromStart SEEK_SET

typedef unsigned char * Str255;
typedef int OSErr;
typedef short int SInt16;

typedef bool Boolean;
#define TRUE true
#define FALSE false

typedef struct UnsignedWide
{
	unsigned int lo;
	unsigned int hi;
} UnsignedWide;

typedef struct Point
{
	int h;
	int v;
} Point;

#define SetFPos(fildes, whence, offset) lseek(fildes, offset, whence)
#define FSClose(fildes) close(fildes)

int Random();
void Microseconds(UnsignedWide *microTickCount);
void GetMouse(Point *p);
void GetMouseRel(Point *p);
void GetKeys(unsigned long *keys);
int Button(void);
#ifdef NOOGG
void alutLoadWAVFile_CFH(char *filename, ALenum *format, void **wave, 
	unsigned int *size, ALsizei *freq);
void alutUnloadWAV_CFH(ALenum format, void *wave, unsigned int size, 
	ALsizei freq);
#define alutLoadWAVFile alutLoadWAVFile_CFH
#define alutUnloadWAV alutUnloadWAV_CFH 
#else
void LoadOGG_CFH(char *filename, ALenum *format, void **wave, 
	unsigned int *size, ALsizei *freq);
void FreeOGG(ALenum format, void *wave, unsigned int size, 
	ALsizei freq);
#endif


FILE *cfh_fopen(const char *filename, const char *mode);

#endif
