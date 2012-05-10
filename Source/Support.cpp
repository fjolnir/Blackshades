#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "SDL/SDL.h"

#include "Support.h"
#include "Files.h"

int Random()
{
#if RAND_MAX >= 65535
	return (rand() % 65535) - 32767;
#else
#error please fix this for your platform
#endif
}

void Microseconds(UnsignedWide *microTickCount)
{
	/* NOTE: hi isn't used in BS, so it's not implemented here */
	/* TODO: does game need microsecond precision? */
	microTickCount->hi = 0;
	microTickCount->lo = SDL_GetTicks() * 1000;
}

void GetMouse(Point *p)
{
	int x;
	int y;
	
	SDL_GetMouseState(&x, &y);
	
	p->h = x;
	p->v = y;
}

void GetMouseRel(Point *p)
{
	int x;
	int y;
	
	SDL_GetRelativeMouseState(&x, &y);
	
	p->h = x;
	p->v = y;
}
int Button(void)
{
	return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1));
}

void InitMouse()
{
// STUB_FUNCTION;
}

void MoveMouse(int xcoord, int ycoord, Point *mouseloc)
{
	/* TODO: mouse warp is annoying when we can just grab the mouse */
#ifdef FULLSCREEN
	SDL_WarpMouse(xcoord, ycoord);
	SDL_PumpEvents();
#endif
	GetMouse(mouseloc);
}

void DisposeMouse()
{
// STUB_FUNCTION;
}

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

static int find_filename(char *filename)
{
	char *ptr;
	char *cur;
	char *next;
	DIR *dir;
	struct dirent *dirent;
	
	if (access(filename, R_OK) == 0) {
		return 1;
	}
	
	ptr = filename;
	
	while (*ptr) {
		if (ptr == filename || *ptr == '/') {
			if (*ptr == '/') {
				cur = ptr+1;
			} else {
				cur = ptr;
			}
			
			if (*cur == 0) {
				/* hit the end */
				break;
			}
			
			next = strchr(cur, '/');
			
			if (ptr != filename) {
				*ptr = 0;
			}
			
			if (next) {
				*next = 0;
			}
			
			if (ptr == filename && *ptr == '/') {
				dir = opendir("/");
			} else {
				dir = opendir(filename);
			}
			
			if (dir == NULL) {
				if (ptr != filename) {
					*ptr = '/';
				}
				
				if (next) {
					*next = 0;
				}
				
				return 0;
			}
			
			while ((dirent = readdir(dir)) != NULL) {
				if (strcasecmp(cur, dirent->d_name) == 0) {
					strcpy(cur, dirent->d_name);
					break;
				}
			}
			
			closedir(dir);
			
			if (ptr != filename) {
				*ptr = '/';
			}
			
			if (next) {
				*next = '/';
				ptr = next;
			} else {
				ptr++;
			}
		} else {
			ptr++;
		}
	}
	
	if (access(filename, R_OK) == 0) {
		return 1;
	}
	
	return 0;
}

static void fix_filename(const char *original, char *fixed)
{
	const char *start;
	int i;
	int len;

	start = original;
	if (original[0] == ':') {
		start = &original[1];
	}

	fixed[MAX_PATH-1] = 0;
	
	strncpy(fixed, start, MAX_PATH);
	
	/* check to see if strncpy overwrote the terminator */
	if (fixed[MAX_PATH-1] != 0) {
		fixed[MAX_PATH-1] = 0;
	
		fprintf(stderr, "ERROR: file truncation error: %s -> %s\n", 
			original, fixed);
	}
	
	len = strlen(fixed);
	for (i = 0; i < len; i++) {
		if (fixed[i] == ':') {
			fixed[i] = '/';
		}
	}
	
	/* 
	   here we would try to see if the file is available (game dir),
	   else try another dir
	   
	   really, this function also needs a flag to indicate whether
	   it should only go to local (write) or both (read)
	 */

	if (find_filename(fixed) == 0) {
		fprintf(stderr, "find failed: %s\n", fixed);
	}
}

/*
Convenient Filename Hacks
*/

FILE *cfh_fopen(const char *filename, const char *mode)
{
	char filename1[MAX_PATH];
	
	fix_filename(filename, filename1);
	
	return fopen(filename1, mode);	
}

int Files::OpenFile(Str255 Name)
{
	char filename1[MAX_PATH];
	
	fix_filename((char *)Name, filename1);
	
	sFile = open(filename1, O_RDONLY | O_BINARY);
	return sFile;
}

void Files::EndLoad()
{
	if (sFile != -1) {
		FSClose( sFile );
	}
	
	sFile = -1;
}

#ifdef NOOGG    
/*
  Our own special magic version that fixes the filename.
 */
void alutLoadWAVFile_CFH(char *filename, ALenum *format, void **wave,
	unsigned int *size, ALsizei *freq)
{
	char filename1[MAX_PATH];
	ALsizei format1, size1, bits1, freq1;
	
	fix_filename(filename, filename1);
	
	alutLoadWAV(filename1, wave, &format1, &size1, &bits1, &freq1);
	
	*format = format1;
	*size = size1;
	*freq = freq1;
}

void alutUnloadWAV_CFH(ALenum format, void *wave, unsigned int size,
	ALsizei freq)
{
	free(wave);
}
#else
#include <vorbis/vorbisfile.h>

/*
Read the requested OGG file into memory, and extract the information required
by OpenAL
*/
void LoadOGG_CFH(char *filename, ALenum *format, void **wave,
	unsigned int *size, ALsizei *freq)
{
	char filename1[MAX_PATH];
	ALsizei format1, size1, freq1;
	void *wave1;
	OggVorbis_File vf;
	vorbis_info *vi;
	FILE *fp;
	int current_section;
	char *buf;
	int asize;
	int err;
	int eof;

#if BYTE_ORDER == BIG_ENDIAN
	const int endian = 1;
#else
	const int endian = 0;
#endif

	/* try to find the real file (and place it in filename1) */
	fix_filename(filename, filename1);

	/* open it for reading */
	fp = fopen(filename1, "rb");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: unable to open %s\n", filename1);
		exit(EXIT_FAILURE);
	}
	
	/* open it up */
	err = ov_open(fp, &vf, NULL, 0);	
	if (err < 0) {
		fprintf(stderr, "ERROR: vorbis error %d opening %s\n", -err, filename1);
		exit(EXIT_FAILURE);
	}

	/* get the ogg information */
	vi = ov_info(&vf, -1);
	if (vi == NULL) {
		fprintf(stderr, "ERROR: vorbis error opening %s (ov_info failed)\n", filename1);
		exit(EXIT_FAILURE);
	}
	
	/* calculate the byte size */
	size1 = vi->channels * 2 * ov_pcm_total(&vf, -1);

	/* hack around some possible ogg vorbis weirdness */
	asize = ((size1 + 2047) / 2048 + 1) * 2048;
	
	/* allocate our buffer */	
	wave1 = malloc(asize);
	
	if (wave1 == NULL) {
		fprintf(stderr, "ERROR: could not allocate %d bytes while loading %s\n", size1, filename1);
		exit(EXIT_FAILURE);
	}

	/* read it in */
	eof = 0;
	buf = (char *)wave1;
	
	while(!eof) {
		long ret = ov_read(&vf, buf, 1024, endian, 2, 1, 
			&current_section);
		
		if (ret == 0) {
			/* end of file */
			eof = 1;
		} else if (ret < 0) {
			/* some sort of error */
			
			/* TODO: is this ok to ignore? */
		} else {
			buf += ret;
		}
	}

	/* get the rest of the information */
	if (vi->channels == 1) {
		format1 = AL_FORMAT_MONO16;
	} else if (vi->channels == 2) {
		format1 = AL_FORMAT_STEREO16;
	} else {
		fprintf(stderr, "ERROR: ogg %s has %d channels\n", filename1, vi->channels);
		exit(EXIT_FAILURE);
	}
	
	freq1 = vi->rate;

	/* we are done with the ogg, so free it */
	ov_clear(&vf);
	
	/* finall, give the values to the caller */	
	*format = format1;
	*size = size1;
	*freq = freq1;
	*wave = wave1;
}

/*
Free the OGG buffer
*/
void FreeOGG(ALenum format, void *wave, unsigned int size,
	ALsizei freq)
{
	free(wave);
}
#endif
