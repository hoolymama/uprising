#ifndef	__pngInfo_H__
#define	__pngInfo_H__

#include <maya/MIOStream.h>
#include <math.h>
#include "lodepng.h"
#include <maya/MPxCommand.h>


// #define kFilenameFlag	"-fn"
// #define kFilenameFlagL "-filename"

#define kPaletteFlag "-p"
#define kPaletteFlagL "-palette"

#define kPaletteSizeFlag "-ps"
#define kPaletteSizeFlagL "-paletteSize"

class pngInfo : public MPxCommand
{
public:

	pngInfo() {}
	virtual ~pngInfo() {}
	MStatus doIt( const MArgList &args );
	static void *creator();
	static MSyntax      newSyntax();



private:



	MStatus readImage(const MString &filename, lodepng::State &state, unsigned &w,
	                  unsigned &h, std::vector<unsigned char> &image );

	MStatus handlePaletteFlag(const MString &filename);
	MStatus handlePaletteSizeFlag(const MString &filename);

};

#endif

