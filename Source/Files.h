#ifndef _FILES_H_
#define _FILES_H_

#include <stdio.h>
#include <stdlib.h>			// Header File For Standard functions
#include <stdio.h>			// Header File For Standard Input/Output
#include <string.h>
#include <ctype.h>
#include <cstdarg>
#include <cmath>
#include <iostream>
#include <fstream>
#ifdef OS9 
#include "gl.h"				// Header File For The OpenGL32 Library
#include "glu.h"			// Header File For The GLu32 Library
#include "tk.h"				// Header File For The Glaux Library
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#ifdef OS9 
#include <Sound.h>
#include <Resources.h>
#include "AGL_DSp.h"		// Header for OpenGL and DrawSprocket Functions
#include "Alerts.h"			// Header for Dialogs and alerts for this application
#endif
#include "MacInput.h"		// Header for Special Mac Input functions
#ifdef OS9 
#include "glm.h"
#include <TextUtils.h>
#endif

#include "Support.h"

#define FILE_STRINGS              130
#define Pstr unsigned char *
#define Cstr char *

#define FILE_ERROR_ALERT          138
#define OLD_SAVE_VERSION          139
#define UNREADABLE_SCORES_ALERT   140

#define SAVE_WARNING_ID           132
#define PB_SAVE                     1
#define PB_CANCEL                   2
#define PB_DONT_SAVE                3

#define FILE_ERROR_STRINGS        129
#define FILE_ERROR_PREFIX           1
#define FILE_ERROR_SUFFIX           2

#define SAVE_WARNING_STRINGS      132

#define FILE_STRINGS              130
#define SAVE_GAME_STRING            1
#define SAVE_SCORES_STRING          2
#define SCORES_NAME_STRING          3
#define SAVE_JOURNAL_STRING         4
#define JOURNAL_NAME_STRING         5
#define UNTITLED_STRING             6
#define SAVE_FORM_STRING            7
#define FORM_NAME_STRING            8

#define REGISTRATION_FORM         136

#define FILE_NAME_SIZE             32
#define ERROR_LENGTH               80

/**> Files Opening <**/
class Files
{
	public:
		char          szSavedGameName[FILE_NAME_SIZE + 1];
		short         sSavedGameVolume;
#ifdef OS9 
		SFReply    sfReply;
		Boolean       bGameSaved;
#endif
#ifdef OS9 
		short sFile;
#else
		int sFile;
#endif

#ifdef OS9 
		short OpenFile(Str255 Name);
#else
		int OpenFile(Str255 Name);
		Files() : sFile(-1) { } 
#endif

#ifdef OS9 
		short PromptForSaveAS( short   sPromptID,
                              short   sNameID,
                              Str255  str255NamePrompt,
                              OSType  osTypeCreator,
                              OSType  osTypeType,
                              SFReply *psfReply );
	        short OpenNewFile( SFReply *psfReply, OSType  osTypeCreator, OSType  osTypeType );
#endif
		short OpenSavedGame(Str255 Name);
		short OpenFileDialog();
		void LoadNamedMap(Str255 Name);
		void LoadGame(Str255 Name, int animnum);

		void LoadMap();
		void StartSave();
		void EndSave();
		void StartLoad();
		void EndLoad();
};
	
#endif
