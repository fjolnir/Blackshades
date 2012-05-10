#include "Game.h"	



extern double multiplier;

extern int visions;

extern unsigned int gSourceID[100];

extern unsigned int gSampleSet[100];

extern Camera camera;

extern float rad2deg;

extern Fog fog;

extern int environment;

extern int slomo;

/********************> HandleKeyDown() <*****/

void	Game::HandleKeyDown( char theChar )

{

	XYZ facing;

	

	if(!mainmenu){

	switch( theChar )

	{

		case 'l': 
		        if(!lasersight==1){lasersight=1;}else{lasersight=0;} 
			
			break;

		case 'k':

			if(debug)timeremaining=0;

			break;

		

		case 'b':

			if(debug){

			alSourcePlay(gSourceID[soulinsound]);

			if(!slomo)slomo=1;

			else slomo=0;}

			if(slomo){

				alSourcef(gSourceID[knifesong], AL_PITCH, (ALfloat)(.5));

				alSourcef(gSourceID[shootsong], AL_PITCH, (ALfloat)(.5));

				alSourcef(gSourceID[zombiesong], AL_PITCH, (ALfloat)(.5));

			}

			if(!slomo){

				alSourcef(gSourceID[knifesong], AL_PITCH, (ALfloat)(1));

				alSourcef(gSourceID[shootsong], AL_PITCH, (ALfloat)(1));

				alSourcef(gSourceID[zombiesong], AL_PITCH, (ALfloat)(1));

			}

			break;

		case 'B':

			if(debug){

			alSourcePlay(gSourceID[soulinsound]);

			paused=1-paused;}

			break;

		case 'f':

			if(debug){

			alSourcePlay(gSourceID[souloutsound]);

			//Facing

			facing=0;

			facing.z=-1;

			

			facing=DoRotation(facing,-camera.rotation2,0,0);

			facing=DoRotation(facing,0,0-camera.rotation,0);

			for(int i=1;i<numpeople;i++){				

				if(person[i].skeleton.free!=1){

				if(findDistancefast(person[i].playercoords,person[0].playercoords)<1000){

				person[i].skeleton.free=1;

				person[i].longdead=1;

				for(int j=0;j<person[i].skeleton.num_joints;j++){

					person[i].skeleton.joints[j].position=DoRotation(person[i].skeleton.joints[j].position,0,person[i].playerrotation,0);

					person[i].skeleton.joints[j].position+=person[i].playercoords;

					person[i].skeleton.joints[j].realoldposition=person[i].skeleton.joints[j].position;

					person[i].skeleton.joints[j].velocity=DoRotation(person[i].skeleton.joints[j].velocity,0,person[i].playerrotation,0);

					person[i].skeleton.joints[j].velocity+=person[i].velocity;

					person[i].skeleton.joints[j].velocity+=facing*50;

					person[i].skeleton.joints[j].velocity.x+=abs(Random()%20)-10;

					person[i].skeleton.joints[j].velocity.y+=abs(Random()%20)-10;

					person[i].skeleton.joints[j].velocity.z+=abs(Random()%20)-10;

				}}}

			}}

			break;

		case 'X':

			if(debug){

			if(person[0].grenphase==0){

				person[0].ammo=-1;

				person[0].whichgun++;

				person[0].grenphase=0;

				person[0].reloads[person[0].whichgun]=3;

				if(person[0].whichgun>7)person[0].whichgun=0;

			}}

			break;

	}

	}

}



/********************> DoEvent() <*****/

#ifdef OS9 
void	Game::DoEvent( EventRecord *event )

{

	

	char	theChar;

	

	switch ( event->what )

	{

		case keyDown:

		case autoKey:

			theChar = event->message & charCodeMask;	// Get the letter of the key pressed from the event message

			HandleKeyDown( theChar );					// Only some key presses are handled here because it is slower and less responsive

			break;

	}

	

	

}
#endif

#ifndef OS9 
static int mapinit = 0;
static int sdlkeymap[SDLK_LAST];

static unsigned char ourkeys[16];

static void init_sdlkeymap()
{
	int i;
	
	for (i = 0; i < SDLK_LAST; i++) {
		sdlkeymap[i] = -1;
	}
	
	sdlkeymap[SDLK_1] = MAC_1_KEY;
	sdlkeymap[SDLK_2] = MAC_2_KEY;
	sdlkeymap[SDLK_3] = MAC_3_KEY;
	sdlkeymap[SDLK_4] = MAC_4_KEY;
	sdlkeymap[SDLK_5] = MAC_5_KEY;
	sdlkeymap[SDLK_6] = MAC_6_KEY;
	sdlkeymap[SDLK_7] = MAC_7_KEY;
	sdlkeymap[SDLK_8] = MAC_8_KEY;
	sdlkeymap[SDLK_9] = MAC_9_KEY;
	sdlkeymap[SDLK_0] = MAC_0_KEY;
	sdlkeymap[SDLK_KP1] = MAC_NUMPAD_1_KEY;
	sdlkeymap[SDLK_KP2] = MAC_NUMPAD_2_KEY;
	sdlkeymap[SDLK_KP3] = MAC_NUMPAD_3_KEY;
	sdlkeymap[SDLK_KP4] = MAC_NUMPAD_4_KEY;
	sdlkeymap[SDLK_KP5] = MAC_NUMPAD_5_KEY;
	sdlkeymap[SDLK_KP6] = MAC_NUMPAD_6_KEY;
	sdlkeymap[SDLK_KP7] = MAC_NUMPAD_7_KEY;
	sdlkeymap[SDLK_KP8] = MAC_NUMPAD_8_KEY;
	sdlkeymap[SDLK_KP9] = MAC_NUMPAD_9_KEY;
	sdlkeymap[SDLK_KP0] = MAC_NUMPAD_0_KEY;
	sdlkeymap[SDLK_a] = MAC_A_KEY;
	sdlkeymap[SDLK_b] = MAC_B_KEY;
	sdlkeymap[SDLK_c] = MAC_C_KEY;
	sdlkeymap[SDLK_d] = MAC_D_KEY;
	sdlkeymap[SDLK_e] = MAC_E_KEY;
	sdlkeymap[SDLK_f] = MAC_F_KEY;
	sdlkeymap[SDLK_g] = MAC_G_KEY;
	sdlkeymap[SDLK_h] = MAC_H_KEY;
	sdlkeymap[SDLK_i] = MAC_I_KEY;
	sdlkeymap[SDLK_j] = MAC_J_KEY;
	sdlkeymap[SDLK_k] = MAC_K_KEY;
	sdlkeymap[SDLK_l] = MAC_L_KEY;
	sdlkeymap[SDLK_m] = MAC_M_KEY;
	sdlkeymap[SDLK_n] = MAC_N_KEY;
	sdlkeymap[SDLK_o] = MAC_O_KEY;
	sdlkeymap[SDLK_p] = MAC_P_KEY;
	sdlkeymap[SDLK_q] = MAC_Q_KEY;
	sdlkeymap[SDLK_r] = MAC_R_KEY;
	sdlkeymap[SDLK_s] = MAC_S_KEY;
	sdlkeymap[SDLK_t] = MAC_T_KEY;
	sdlkeymap[SDLK_u] = MAC_U_KEY;
	sdlkeymap[SDLK_v] = MAC_V_KEY;
	sdlkeymap[SDLK_w] = MAC_W_KEY;
	sdlkeymap[SDLK_x] = MAC_X_KEY;
	sdlkeymap[SDLK_y] = MAC_Y_KEY;
	sdlkeymap[SDLK_z] = MAC_Z_KEY;
	sdlkeymap[SDLK_F1] = MAC_F1_KEY;
	sdlkeymap[SDLK_F2] = MAC_F2_KEY;
	sdlkeymap[SDLK_F3] = MAC_F3_KEY;
	sdlkeymap[SDLK_F4] = MAC_F4_KEY;
	sdlkeymap[SDLK_F5] = MAC_F5_KEY;
	sdlkeymap[SDLK_F6] = MAC_F6_KEY;
	sdlkeymap[SDLK_F7] = MAC_F7_KEY;
	sdlkeymap[SDLK_F8] = MAC_F8_KEY;
	sdlkeymap[SDLK_F9] = MAC_F9_KEY;
	sdlkeymap[SDLK_F10] = MAC_F10_KEY;
	sdlkeymap[SDLK_F11] = MAC_F11_KEY;
	sdlkeymap[SDLK_F12] = MAC_F12_KEY;
	sdlkeymap[SDLK_RETURN] = MAC_RETURN_KEY;
	sdlkeymap[SDLK_KP_ENTER] = MAC_ENTER_KEY;
	sdlkeymap[SDLK_TAB] = MAC_TAB_KEY;
	sdlkeymap[SDLK_SPACE] = MAC_SPACE_KEY;
	sdlkeymap[SDLK_BACKSPACE] = MAC_DELETE_KEY;
	sdlkeymap[SDLK_ESCAPE] = MAC_ESCAPE_KEY;
	sdlkeymap[SDLK_LCTRL] = MAC_CONTROL_KEY;
	sdlkeymap[SDLK_RCTRL] = MAC_CONTROL_KEY;
	sdlkeymap[SDLK_LSHIFT] = MAC_SHIFT_KEY;
	sdlkeymap[SDLK_RSHIFT] = MAC_SHIFT_KEY;
	sdlkeymap[SDLK_CAPSLOCK] = MAC_CAPS_LOCK_KEY;
	sdlkeymap[SDLK_LALT] = MAC_OPTION_KEY;
	sdlkeymap[SDLK_RALT] = MAC_OPTION_KEY;
	sdlkeymap[SDLK_PAGEUP] = MAC_PAGE_UP_KEY;
	sdlkeymap[SDLK_PAGEDOWN] = MAC_PAGE_DOWN_KEY;
	sdlkeymap[SDLK_INSERT] = MAC_INSERT_KEY;
	sdlkeymap[SDLK_DELETE] = MAC_DEL_KEY;
	sdlkeymap[SDLK_HOME] = MAC_HOME_KEY;
	sdlkeymap[SDLK_END] = MAC_END_KEY;
	sdlkeymap[SDLK_LEFTBRACKET] = MAC_LEFT_BRACKET_KEY;
	sdlkeymap[SDLK_RIGHTBRACKET] = MAC_RIGHT_BRACKET_KEY;
	sdlkeymap[SDLK_UP] = MAC_ARROW_UP_KEY;
	sdlkeymap[SDLK_DOWN] = MAC_ARROW_DOWN_KEY;
	sdlkeymap[SDLK_LEFT] = MAC_ARROW_LEFT_KEY;
	sdlkeymap[SDLK_RIGHT] = MAC_ARROW_RIGHT_KEY;
	
	mapinit = 1;
}

void GetKeys(unsigned long *keys)
{
	/* this is just weird */
	memcpy(keys, ourkeys, sizeof(ourkeys));
}

static void DoSDLKey(Game *g, SDL_Event *event)
{
	int press = (event->type == SDL_KEYDOWN) ? 1 : 0;
	int mackey;
	int index;
	int mask;
	

	if (mapinit == 0) {
		init_sdlkeymap();
	}
	
	mackey = sdlkeymap[event->key.keysym.sym];
	
	if (mackey != -1) {
		index = mackey / 8;
		mask = 1 << (mackey % 8);
		
		if (press) {
			ourkeys[index] |= mask;
		} else {
			ourkeys[index] &= ~mask;
		}
	}
	
	if (event->key.keysym.unicode && 
		!(event->key.keysym.unicode & 0xFF80)) {
	
		/* hey, at least it was aleady public */
		g->HandleKeyDown(event->key.keysym.unicode);
	}
	
	
}

static void ProcessSDLEvents(Game *g)
{
	SDL_Event event;
	
	if (SDL_PollEvent(&event)) {
		do {
			switch(event.type) {
				case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_RETURN &&
					event.key.keysym.mod & KMOD_ALT)
				{
					SDL_WM_ToggleFullScreen (SDL_GetVideoSurface ());
					break;
				}
				if (event.key.keysym.sym == SDLK_g &&
					event.key.keysym.mod & KMOD_CTRL)
				{
					if (SDL_WM_GrabInput (SDL_GRAB_QUERY) == SDL_GRAB_OFF)
					{
						SDL_WM_GrabInput (SDL_GRAB_ON);
						SDL_ShowCursor (SDL_DISABLE);
					}
					else
					{
						SDL_WM_GrabInput (SDL_GRAB_OFF);
						SDL_ShowCursor (SDL_ENABLE);
					}
					break;
				}
				case SDL_KEYUP:
					DoSDLKey(g, &event);
					break;
				case SDL_QUIT:
					exit(0);
			}
		} while (SDL_PollEvent(&event));
	}
}	

#endif

/********************> EventLoop() <*****/

void	Game::EventLoop( void )

{

#ifdef OS9 
	EventRecord		event;
#endif

	unsigned char	theKeyMap[16];

	int colaccuracy,i;

	GLfloat oldmult;

	gQuit = false;

	while ( gQuit == false )

	{

#ifdef OS9 
		if ( GetNextEvent( everyEvent, &event ) )

			DoEvent( &event );
#else
		ProcessSDLEvents(this);
#endif
		

		start=TimerGetTime(&theTimer);

		

		colaccuracy=sps/framespersecond+1;

		if(colaccuracy>sps){colaccuracy=sps;}

		

		oldmult=multiplier;

		multiplier/=colaccuracy;

		for(i=0;i<(int)(colaccuracy+.5);i++){

			Tick();

		}

		multiplier=oldmult;

		

		if ( DrawGLScene())

#ifdef OS9 
			aglSwapBuffers( gOpenGLContext );
#else
                        SDL_GL_SwapBuffers();
#endif

		else

			gQuit = true;

		oldmult=multiplier;	

		

		end=TimerGetTime(&theTimer);

		timetaken=end-start;

		framespersecond=600000000/timetaken;

		while(framespersecond>maxfps){

			end=TimerGetTime(&theTimer);

			timetaken=end-start;

			framespersecond=600000000/timetaken;

		}

		multiplier5=multiplier4;

		multiplier4=multiplier3;

		multiplier3=multiplier2;

		multiplier2=1/framespersecond;

		multiplier=(multiplier2+multiplier3+multiplier4+multiplier5)/4;

		if(multiplier>1)multiplier=1;

		if(multiplier<.00001)multiplier=.00001;

		if(visions==1&&mainmenu==0)multiplier/=3;

		if(slomo)multiplier*=.2;

		if(paused)multiplier=0;

		GetKeys( ( unsigned long * )theKeyMap );

		if ( IsKeyDown( theKeyMap, MAC_COMMAND_KEY )&&IsKeyDown( theKeyMap, MAC_Q_KEY )){

			gQuit = true;

			if(score>highscore){

				highscore=score;

#ifdef OS9 
				ofstream opstream(":Data:Highscore"); 
#else
				/* TODO */
				ofstream opstream("Data/Highscore");
#endif

		        opstream << highscore;

				opstream << "\n";

	        	opstream << beatgame;

				opstream.close();

			}

		}

		if ( IsKeyDown( theKeyMap, MAC_ESCAPE_KEY )){

			alSourcePause(gSourceID[rainsound]);

		

			mainmenu=1;

			alSourcePlay(gSourceID[souloutsound]);

			flashamount=1;

			flashr=1;flashg=1;flashb=1;

			alSourceStop(gSourceID[visionsound]);

			whichsong=mainmenusong;

			alSourceStop(gSourceID[knifesong]);

			alSourceStop(gSourceID[shootsong]);

			alSourceStop(gSourceID[zombiesong]);

			alSourceStop(gSourceID[mainmenusong]);

			alSourcef(gSourceID[knifesong], AL_MIN_GAIN, 0);

			alSourcef(gSourceID[shootsong], AL_MIN_GAIN, 0);

			alSourcef(gSourceID[zombiesong], AL_MIN_GAIN, 0);

			alSourcef(gSourceID[mainmenusong], AL_MIN_GAIN, 0);

			alSourcePlay(gSourceID[whichsong]);

			alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 1);

		}

	}

}
