#ifndef	__imageMeasureCmd_H__
#define	__imageMeasureCmd_H__

#include <maya/MIOStream.h>
#include <math.h>
#include <maya/MPxCommand.h>

#include "cImgData.h"

#define kAxisFlag "-a"
#define kAxisFlagL "-axis"

#define kSliceFlag "-s"
#define kSliceFlagL "-slice"

#define kThresholdFlag "-t"
#define kThresholdFlagL "-threshold"

#define kInvertFlag "-i"
#define kInvertFlagL "-invert"


// #define kPercentageFlag "-p"
// #define kPercentageFlagL "-percentage"


#define kScaleFlag "-sc"
#define kScaleFlagL "-scale"

#define  kExtentsFlag "-e"
#define  kExtentsFlagL "-extents"

#define kFilenameFlag "-f"
#define kFilenameFlagL "-filename"


class imageMeasureCmd : public MPxCommand
{
public:

	imageMeasureCmd() {}
	virtual ~imageMeasureCmd() {}
	MStatus doIt( const MArgList &args );
	static void *creator();
	static MSyntax      newSyntax();

private:

};

#endif

