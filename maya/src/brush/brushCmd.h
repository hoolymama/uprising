#ifndef	__brushCmd_H__
#define	__brushCmd_H__

#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxCommand.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MFnDependencyNode.h>
#include "brushNode.h"


#define kTrianglesFlag	 "-tri"
#define kTrianglesFlagL "-triangles"

#define kTcpFlag	 "-tcp"
#define kTcpFlagL "-toolCenterPoint"


/////////////////////////////////////////

class brushCmd : public MPxCommand
/** @dia:pos -52.2,6.6 */
{
public:

	brushCmd() {}
	virtual ~brushCmd() {}

	MStatus doIt( const MArgList &args );

	static void *creator();

	static MSyntax      newSyntax();

private:
	MStatus handleTrianglesFlag(const Brush &geom) ;
	MStatus handleTcpFlag(const Brush &geom) ;

};

#endif	//	!____

