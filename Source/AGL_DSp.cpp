/**> HEADER FILES <**/
#include "AGL_DSp.h"
#include "Alerts.h"


/**> GLOBAL VARIABLES <**/
DSpContextAttributes	gDSpContextAttributes;	// Global DrawSprocket context attributes
DSpContextReference		gDSpContext;			// The global DrawSprocket context
AGLContext				gOpenGLContext;			// The global OpenGL (AGL) context


/********************> ToolboxInit() <*****/
void		ToolboxInit( void )
{
	
	MaxApplZone();
	
	InitGraf( &qd.thePort );
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs( 0L );
	InitCursor();
	
}

/********************> HasAppearance() <*****/
Boolean 	HasAppearance( void )
{
	
	OSErr    error;
	SInt32   response;
	Boolean  appearancePresent    = false;
	Boolean  appearance101present = false;
	Boolean  appearance110present = false;
	Boolean  inCompatibilityMode  = false;
	
	error = Gestalt( gestaltAppearanceAttr, &response );
	
	// If Gestalt returns no error and the bit in response represented by the constant
	// gestaltAppearanceExists is set, proceed, otherwise exit with an error message.
	
	if ( error == noErr && ( BitTst( &response, 31 - gestaltAppearanceExists ) ) )
	{
		// At least Version 1.0 is present.  Set a flag.
		appearancePresent = true;
		
		// If the bit in response represented by the constant gestaltAppearanceCompatMode
		// is set, system-wide Appearance is off.  The result of this check will be
		// relevant only where Versions 1.0 through 1.0.3 are present.
		if( BitTst( &response, 31 - gestaltAppearanceCompatMode ) )
			inCompatibilityMode = true;
		
		// Call Gestalt again with the gestaltAppearanceVersion selector.
		Gestalt( gestaltAppearanceVersion, &response );
		
		// If the low order word in response is 0x0101, Version 1.0.1, 1.0.2, or 1.0.3 is
		// present. If the low order word in response is 0x0110, Version 1.1 is available.
		if( response == 0x00000101 )
			appearance101present = true;
		else if( response == 0x00000110 )
			appearance110present = true;
	}
	/*else
	{
		StopAlert( kNoAppearanceAlert, nil );
		ExitToShell();
	}*/
	
	// Register this app as an Appearance Client
	//RegisterAppearanceClient();
	
	return appearancePresent;
	
}

/********************> SetupScreen() <*****/
CGrafPtr	SetupScreen( int width, int height )
{
	
	OSStatus theError;
	CGrafPtr theFrontBuffer;
	
	// Start DrawSprocket
	theError = DSpStartup();
	if( theError )
		FatalErrorAlert( kErr_DSpStartupFailed, theError );
	
	// Set the Context Attributes
	gDSpContextAttributes.displayWidth = width;
	gDSpContextAttributes.displayHeight = height;
	gDSpContextAttributes.colorNeeds = kDSpColorNeeds_Require;
	gDSpContextAttributes.displayDepthMask = kDSpDepthMask_32;
	gDSpContextAttributes.backBufferDepthMask = kDSpDepthMask_32;
	gDSpContextAttributes.displayBestDepth = 32;
	gDSpContextAttributes.backBufferBestDepth = 32;
	gDSpContextAttributes.pageCount = 1;
	
	// Find the best context for our attributes
	theError = DSpFindBestContext( &gDSpContextAttributes, &gDSpContext );
	if( theError != noErr )
		FatalErrorAlert( kErr_DSpFindBestContextFailed, theError ); // This function is in my Alerts.cpp
	
	// Reserve that context
	theError = DSpContext_Reserve( gDSpContext, &gDSpContextAttributes );
	if( theError != noErr )
		FatalErrorAlert( kErr_DSpContext_ReserveFailed, theError );
	
	// Fade out
	theError = DSpContext_FadeGammaOut( NULL, NULL );	
	if( theError != noErr )
		FatalErrorAlert( kErr_DSpFadeFailed, theError );
	
	// Activate the context
	theError = DSpContext_SetState( gDSpContext, kDSpContextState_Active );
	if( theError != noErr )
	{
		// Fade back in the display before dying
		theError = DSpContext_FadeGammaIn( NULL, NULL );
		
		// Now do the fatal error alert
		FatalErrorAlert( kErr_ActivateContextFailed, theError );
	}
	
	// Fade in
	theError = DSpContext_FadeGammaIn( NULL, NULL );
	if( theError != noErr )
		FatalErrorAlert( kErr_DSpFadeFailed, theError );
		
	// Create a window to draw in
	CreateWindow( theFrontBuffer, width, height );
	
	return theFrontBuffer;
	
}

/********************> CreateWindow() <*****/
void		CreateWindow( CGrafPtr &theFrontBuffer, int width, int height )
{
	
	Rect			rect;
	AuxWinHandle	awh;
	CTabHandle		theColorTable;
	OSErr			error;
	RGBColor 		backColor = { 0xFFFF, 0xFFFF, 0xFFFF };
	RGBColor 		foreColor = { 0x0000, 0x0000, 0x0000 };
	
	// Set the window rect
	rect.top = rect.left = 0;
	DSpContext_LocalToGlobal( gDSpContext, ( Point* )&rect );
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;
	
	// Create a new color window
	theFrontBuffer = ( CGrafPtr )NewCWindow( NULL, &rect, "\p", 0, plainDBox, kMoveToFront, 0, 0 );
	
	// set the content color of the window to black to avoid a white flash when the window appears.		
	if ( GetAuxWin( ( WindowPtr )theFrontBuffer, &awh ) )
	{
		theColorTable = ( **awh ).awCTable;
		error = HandToHand( ( Handle* )&theColorTable );
		if ( error )
			DebugStr( "\pOut of memory!" );
			
		( **theColorTable ).ctTable[wContentColor].rgb.red = 0;
		( **theColorTable ).ctTable[wContentColor].rgb.green = 0;
		( **theColorTable ).ctTable[wContentColor].rgb.blue = 0;
		
		CTabChanged( theColorTable );
		
		// the color table will be disposed by the window manager when the window is disposed
		SetWinColor( ( WindowPtr )theFrontBuffer, ( WCTabHandle )theColorTable );
	}
	
	// Show the window
	ShowWindow( ( GrafPtr )theFrontBuffer );
	SetPort( ( GrafPtr )theFrontBuffer );
	
	// Set current pen colors
	RGBForeColor( &foreColor );
	RGBBackColor( &backColor );
	
}

/********************> ShutdownScreen() <*****/
void		ShutdownScreen( CGrafPtr theFrontBuffer )
{
	
	DSpContext_FadeGammaOut( NULL, NULL );
	DisposeWindow( ( WindowPtr )theFrontBuffer );
	DSpContext_SetState( gDSpContext, kDSpContextState_Inactive );
	DSpContext_FadeGammaIn( NULL, NULL );
	DSpContext_Release( gDSpContext );
	DSpShutdown();
	
}

/********************> SetupAGL() <*****/
AGLContext	SetupAGL( AGLDrawable window )
{
	GLint          attrib[] = { AGL_RGBA, AGL_DEPTH_SIZE, 24, AGL_DOUBLEBUFFER, AGL_NONE };
	AGLPixelFormat format;
	AGLContext     context;
	GLboolean      ok;

	// Choose an rgb pixel format
	format = aglChoosePixelFormat( NULL, 0, attrib );
	if ( format == NULL )
		return NULL;

	// Create an AGL context
	context = aglCreateContext( format, NULL );
	if ( context == NULL )
		return NULL;

	// Attach the window to the context
	ok = aglSetDrawable( context, window );
	if ( !ok )
		return NULL;
	
	// Make the context the current context
	ok = aglSetCurrentContext( context );
	if ( !ok )
		return NULL;

	// The pixel format is no longer needed so get rid of it
	aglDestroyPixelFormat( format );

	return context;
	
}

/********************> CleanupAGL() <*****/
void		CleanupAGL( AGLContext context )
{
	
	aglSetCurrentContext( NULL );
	aglSetDrawable( context, NULL );
	aglDestroyContext( context );
	
}
