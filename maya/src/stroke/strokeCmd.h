#ifndef strokeCmd_H__
#define strokeCmd_H__

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

#define kCoilFlag "-cl"
#define kCoilFlagL "-coil"

class strokeCmd : public MPxCommand

{
public:
	strokeCmd() {}
	virtual ~strokeCmd() {}

	MStatus doIt(const MArgList &args);

	static void *creator();

	static MSyntax newSyntax();
};

#endif
