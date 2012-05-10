/**> HEADER FILES <**/
#include "Alerts.h"


/********************> SelectResolution() <*****/
int		SelectResolution( void )
{
	
	DialogPtr	dialog;
	Boolean		dialogDone = false;
	short		itemHit, itemType;
	Handle		okItem;
	Handle		resolutionItem;
	Rect		itemRect;
	int			selectionNum;
	
	// Load the dialog
	dialog = GetNewDialog( kResID_DLOG_SelectResolution, nil, kMoveToFront );
	
	// Display the dialog
	ShowWindow( dialog );
	SetPort( dialog );
	
	// Load dialog items
	SetDialogDefaultItem( dialog, iOK );
	SetDialogTracksCursor( dialog, true );
	GetDialogItem( dialog, iOK, &itemType, &okItem, &itemRect );
	GetDialogItem( dialog, iResolutionPopUp, &itemType, &resolutionItem, &itemRect );
	
	// Set item values
	SetControlValue( ( ControlHandle )resolutionItem, i640x480 );
	
	
	while ( !dialogDone )
	{
		
		ModalDialog( nil, &itemHit );
		
		switch( itemHit )
		{
			case iOK:
				dialogDone = true;
				// Get the item number selected int the popup
				selectionNum = GetControlValue( ( ControlHandle )resolutionItem );
				break;
			case iResolutionPopUp:
				// We don't actually need to do anything here
				break;
		}
		
	}
	
	DisposeDialog( dialog );

	// Return the item selected in the popup menu
	return selectionNum;
}

/********************> MessageAlert() <*****/
void	MessageAlert( unsigned char *theMessage )
{
	
	// Set parameter ^0 to our message (I could set up to three, but for simplicity's sake I won't)
	ParamText( ( unsigned char * )theMessage, NULL, NULL, NULL );
	
	// Do the Alert
	NoteAlert( kResID_ALRT_MessageAlert, nil );
	
}

/********************> FatalErrorAlert() <*****/
void	FatalErrorAlert( UInt16 errorNum, OSErr osError )
{
	
	Str15				errNumStr;
	Str255				mainMessage;
	
	// Convert the OSErr to a string
	NumToString( osError, errNumStr );
	
	// Get the error description (inErrorDesc) from the STR# resource
	GetIndString( mainMessage, kResID_STRn_ErrorStrings, errorNum );
	
	// Set the parameters (^0 and ^1) in the ALRT to our error messages
	ParamText( mainMessage, errNumStr, NULL, NULL );
	
	// Do the alert (which now has our messages in it)
	StopAlert( kResID_ALRT_ErrorAlert, NULL );
	
	// Quit
	exit( EXIT_SUCCESS );
	
}
