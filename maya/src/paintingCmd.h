#ifndef	__paintingCmd_H__
#define	__paintingCmd_H__

#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxCommand.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MFnDependencyNode.h>
#include "paintingNode.h"


#define kClusterCountFlag				"-cc"
#define kClusterCountFlagL 			"-clusterCount"

#define  kClusterIndexFlag "-ci"
#define  kClusterIndexFlagL "-clusterIndex"

#define kStrokeCountFlag				"-sc"
#define kStrokeCountFlagL 			"-strokeCount"

#define kClusterReasonFlag				"-cr"
#define kClusterReasonFlagL 			"-clusterReason"

#define kClusterNameFlag				"-cn"
#define kClusterNameFlagL 			"-clusterName"

#define kClusterPaintIdFlag				"-cpid"
#define kClusterPaintIdFlagL 			"-clusterPaintId"

#define kClusterBrushIdFlag				"-cbid"
#define kClusterBrushIdFlagL 			"-clusterBrushId"

#define kClusterTravelCutoffFlag	"-ctc"
#define kClusterTravelCutoffFlagL "-clusterTravelCutoff"

#define kClusterTravelFlag				"-ct"
#define kClusterTravelFlagL 			"-clusterTravel"

#define  kStrokeIndexFlag "-si"
#define  kStrokeIndexFlagL "-strokeIndex"

// -clusterCount
// -clusterIndex
// -strokeIndex
// -strokeCount
// -clusterReason
// -clusterName
// -paintId
// -brushId
// -travelCutoff
// -travel


// #define kBrushCountFlag				"-bc"
// #define kBrushCountFlagL 			"-brushCount"

// #define kJointsFlag				"-j"
// #define kJointsFlagL 			"-joints"

/*
#define kCacheFrameFlag			"-c"
#define kCacheFrameFlagL 		"-cacheFrame"

#define kForceCacheFlag			"-fc"
#define kForceCacheFlagL 		"-forceCache"
*/

// #define kFixFlag				"-fx"
// #define kFixFlagL 				"-fix"






/////////////////////////////////////////
//
//	paintingCmd
//		: MPxCommand
//
class paintingCmd : public MPxCommand
/** @dia:pos -52.2,6.6 */
{
public:

	paintingCmd() {}
	virtual ~paintingCmd() {}

	MStatus doIt( const MArgList &args );

	static void *creator();

	static MSyntax      newSyntax();

private:
	MStatus handleClusterCountFlag(const paintingGeom &geom) ;

	MStatus handleStrokeCountFlag(const paintingGeom &geom,
	                              MArgDatabase &argData);

	int getClusterId(const paintingGeom &geom, MArgDatabase &argData,
	                 MStatus *status);

	int getStrokeId(const paintingGeom &geom, MArgDatabase &argData,
	                MStatus *status);

	MStatus handleClusterReasonFlag(const paintingGeom &geom, MArgDatabase &argData);
	MStatus handleClusterNameFlag(const paintingGeom &geom, MArgDatabase &argData);
	MStatus handleClusterPaintIdFlag(const paintingGeom &geom, MArgDatabase &argData);
	MStatus handleClusterBrushIdFlag(const paintingGeom &geom, MArgDatabase &argData);
	MStatus handleClusterTravelCutoffFlag(const paintingGeom &geom, MArgDatabase &argData);
	MStatus handleClusterTravelFlag(const paintingGeom &geom, MArgDatabase &argData);
};

#endif	//	!__paintingCmd_H__

