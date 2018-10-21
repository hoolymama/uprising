#ifndef	__imgTestCmd_H__
#define	__imgTestCmd_H__

#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxCommand.h>


#define kFilenameFlag			"-fn"
#define kFilenameFlagL 			"-filename"


#define kPlugFlag "-p"
#define kPlugFlagL "-plug"

#define kResolutionFlag	"-r"
#define kResolutionFlagL "-resolution"


class imgTestCmd : public MPxCommand
{
public:

	imgTestCmd() {}
	virtual ~imgTestCmd() {}
	MStatus doIt( const MArgList &args );
	static void *creator();
	static MSyntax      newSyntax();

	// void intensityProfile( MString &filename) ;

	void pngInfo(const MString &mayaFn);
};

#endif

