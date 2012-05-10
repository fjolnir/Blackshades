/********************> Headers <*****/

#include "Game.h"



/********************> Globals <*****/



Game game;



/********************> main() <*****/

int 	main( int argc, char *argv[] )

	{

	
#ifdef OS9 
	ToolboxInit();

	

	if ( HasAppearance() )

		RegisterAppearanceClient();
#endif
	



	game.InitGL();

	

	game.InitGame();

	

	game.EventLoop();

	

	game.Dispose();

	
#ifdef OS9 
	if ( HasAppearance() )

		UnregisterAppearanceClient();

	

	FlushEvents( everyEvent, 0 );

	ExitToShell();
#endif
	

}

