/**> HEADER FILES <**/
#include "TGALoader.h"


/********************> LoadTGA() <*****/
TGAImageRec*	LoadTGA( char *filename )
{    
	GLubyte			TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	GLubyte			TGAcompare[12];								// Used To Compare TGA Header
	GLubyte			header[6];									// First 6 Useful Bytes From The Header
	GLuint			bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint			imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint			temp;										// Temporary Variable
	GLuint			type = GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)
	TGAImageRec		*texture;
	FILE			*file;
	
	// Open The TGA File
	file = cfh_fopen( filename, "rb" );

	if( ( file == NULL ) ||	// Does File Even Exist?
		( fread( TGAcompare, 1, sizeof( TGAcompare ), file ) != sizeof( TGAcompare ) ) ||	// Are There 12 Bytes To Read?
		( memcmp( TGAheader, TGAcompare, sizeof( TGAheader ) ) != 0 ) ||	// Does The Header Match What We Want?
		( fread( header, 1, sizeof( header ), file ) != sizeof( header ) ) )// If So Read Next 6 Header Bytes
	{
		// If anything failed then close the file and return false
		fclose( file );
		return NULL;
	}
	
	// Create a new RGBAImageRec
	texture = ( TGAImageRec* )malloc( sizeof( TGAImageRec ) );
	
	// Determine the TGA width (highbyte*256+lowbyte) and height (highbyte*256+lowbyte)
	texture->sizeX  = header[1] * 256 + header[0];
	texture->sizeY = header[3] * 256 + header[2];
    
    // Make sure the height, width, and bit depth are valid
 	if(	( texture->sizeX <= 0 ) || ( texture->sizeY <= 0 ) || ( ( header[4] != 24 ) && ( header[4] != 32 ) ) )
	{
		// If anything failed then close the file, free up memory for the image, and return NULL
		fclose( file );
		free( texture );
		return NULL;
	}
	
	// Grab The TGA's Bits Per Pixel (24 or 32)
	texture->bpp = header[4];							
	bytesPerPixel = texture->bpp/8;	// Divide By 8 To Get The Bytes Per Pixel
	
	// Calculate The Memory Required For The TGA Data
	imageSize = texture->sizeX * texture->sizeY * bytesPerPixel;
	
	// Reserve Memory To Hold The TGA Data
	texture->data = ( GLubyte* )malloc( imageSize );		
	
	// Make sure the right amount of memory was allocated
	if(	( texture->data == NULL ) || ( fread( texture->data, 1, imageSize, file ) != imageSize ) )
	{
		// Free up the image data if there was any
		if( texture->data != NULL )
			free( texture->data );

		// If anything failed then close the file, free up memory for the image, and return NULL
		fclose( file );
		free( texture );
		return NULL;
	}
	
	// Loop Through The Image Data
	for( GLuint i = 0; i < int( imageSize ); i += bytesPerPixel )
	{
		// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp = texture->data[i];					// Temporarily Store The Value At Image Data 'i'
		texture->data[i] = texture->data[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->data[i + 2] = temp;				// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}
	
	// Close The File
	fclose( file );
	
	return texture;
}
