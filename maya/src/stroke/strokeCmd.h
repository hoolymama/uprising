#ifndef strokeCmd_H__
#define strokeCmd_H__

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include "stroke.h"

#define kStrokeIndexFlag "-si"
#define kStrokeIndexFlagL "-strokeIndex"

#define kCoilFlag "-cl"
#define kCoilFlagL "-coil"

#define kMaxCoilFlag "-mcl"
#define kMaxCoilFlagL "-maxCoil"

#define kStrokeCountFlag "-sc"
#define kStrokeCountFlagL "-strokeCount"


class strokeCmd : public MPxCommand

{
public:
	strokeCmd() {}
	virtual ~strokeCmd() {}

	MStatus doIt(const MArgList &args);

	static void *creator();

	static MSyntax newSyntax();

	MStatus handleStrokeCountFlag(const std::vector<Stroke> &strokes);
	MStatus handleCoilFlag(const std::vector<Stroke> &strokes, int strokeId);
	MStatus handleMaxCoilFlag(const std::vector<Stroke> &strokes);
	int getStrokeId(const std::vector<Stroke> &strokes, MArgDatabase &argData,
						MStatus *status);



};

#endif
