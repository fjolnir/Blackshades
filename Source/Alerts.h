#ifndef _MYALERTS_H_
#define _MYALERTS_H_


/**> HEADER FILES <**/
#include <string.h>
#include <stdlib.h>


/**> CONSTANT DECLARATIONS <**/
#define	kMoveToFront		kFirstWindowOfClass
// Alerts
#define	kResID_ALRT_MessageAlert		128
#define	kResID_ALRT_ErrorAlert			129
// Dialogs
#define	kResID_DLOG_SelectResolution	130
#define	iOK								1
#define	iResolutionPopUp				2
// Menus
#define	mResolution						128
#define	i640x480						1
#define	i800x600						2
#define	i1024x768						3
// String resources
#define	kResID_STRn_ErrorStrings		128
// Misc
#define kInsignificantConstant7454955	0

// Error numbers	
#define	kErr_DSpFindBestContextFailed	1
#define	kErr_DSpContext_ReserveFailed	2
#define	kErr_ActivateContextFailed		3
#define	kErr_DSpStartupFailed			4
#define	kErr_DSpFadeFailed				5
#define	kErr_AGLContext_CreationFailed	6


/**> FUNCTION PROTOTYPES <**/
int		SelectResolution( void );
void	MessageAlert( unsigned char *theMessage );
void	FatalErrorAlert( UInt16 errorNum, OSErr osError );


#endif