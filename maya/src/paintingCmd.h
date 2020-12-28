#ifndef __paintingCmd_H__
#define __paintingCmd_H__

#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxCommand.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MFnDependencyNode.h>
#include "paintingNode.h"

#define kClusterCountFlag "-cc"
#define kClusterCountFlagL "-clusterCount"

#define kClusterIndexFlag "-ci"
#define kClusterIndexFlagL "-clusterIndex"

#define kStrokeCountFlag "-sc"
#define kStrokeCountFlagL "-strokeCount"

#define kClusterReasonFlag "-cr"
#define kClusterReasonFlagL "-clusterReason"

#define kClusterPaintIdFlag "-cpi"
#define kClusterPaintIdFlagL "-clusterPaintId"

#define kClusterBrushIdFlag "-cbi"
#define kClusterBrushIdFlagL "-clusterBrushId"

#define kClusterTravelCutoffFlag "-ctc"
#define kClusterTravelCutoffFlagL "-clusterTravelCutoff"

#define kClusterTravelFlag "-ct"
#define kClusterTravelFlagL "-clusterTravel"

#define kStrokeIndexFlag "-si"
#define kStrokeIndexFlagL "-strokeIndex"

#define kRotateOrderFlag "-ro"
#define kRotateOrderFlagL "-rotateOrder"

#define kRotateUnitFlag "-ru"
#define kRotateUnitFlagL "-rotateUnit"

#define kStrokePositionsFlag "-sp"
#define kStrokePositionsFlagL "-strokePositions"

#define kStrokeRotationsFlag "-sr"
#define kStrokeRotationsFlagL "-strokeRotations"

#define kStrokeSpeedLinearFlag "-ssl"
#define kStrokeSpeedLinearFlagL "-strokeSpeedLinear"

#define kStrokeSpeedAngularFlag "-ssa"
#define kStrokeSpeedAngularFlagL "-strokeSpeedAngular"

#define kStrokeColorsFlag "-tcl"
#define kStrokeColorsFlagL "-strokeColors"

#define kStrokeArrivalPositionsFlag "sap"
#define kStrokeArrivalPositionsFlagL "strokeArrivalPositions"

#define kStrokeArrivalRotationsFlag "sar"
#define kStrokeArrivalRotationsFlagL "strokeArrivalRotations"

#define kStrokeDeparturePositionFlag "sdp"
#define kStrokeDeparturePositionFlagL "strokeDeparturePosition"

#define kStrokeDepartureRotationFlag "sdr"
#define kStrokeDepartureRotationFlagL "strokeDepartureRotation"

#define kStrokeTangentsFlag "-st"
#define kStrokeTangentsFlagL "-strokeTangents"

#define kStrokeBackstrokeFlag "-sb"
#define kStrokeBackstrokeFlagL "-strokeBackstroke"

#define kStrokeArcLengthFlag "-sal"
#define kStrokeArcLengthFlagL "-strokeArcLength"

#define kStrokeParentIndexFlag "-spi"
#define kStrokeParentIndexFlagL "-strokeParentIndex"

#define kDipCombinationsFlag "-dc"
#define kDipCombinationsFlagL "-dipCombinations"

#define kJsonFlag "-js"
#define kJsonFlagL "-json"

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

	MStatus doIt(const MArgList &args);

	static void *creator();

	static MSyntax newSyntax();

private:
	MStatus handleClusterCountFlag(const paintingGeom &geom);

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

	MStatus handleStrokeSpeedLinearFlag(const paintingGeom &geom, MArgDatabase &argData);

	MStatus handleStrokeSpeedAngularFlag(const paintingGeom &geom, MArgDatabase &argData);

	MStatus handleStrokePositionsFlag(const paintingGeom &geom, MArgDatabase &argData,
									  const MMatrix &worldMatrix);

	MStatus handleStrokeRotationsFlag(const paintingGeom &geom, MArgDatabase &argData,
									  const MMatrix &worldMatrix);

	MStatus handleStrokeArrivalPositionsFlag(const paintingGeom &geom, MArgDatabase &argData,
											 const MMatrix &worldMatrix);
	MStatus handleStrokeArrivalRotationsFlag(const paintingGeom &geom, MArgDatabase &argData,
											 const MMatrix &worldMatrix);
	MStatus handleStrokeDeparturePositionFlag(const paintingGeom &geom, MArgDatabase &argData,
											  const MMatrix &worldMatrix);
	MStatus handleStrokeDepartureRotationFlag(const paintingGeom &geom, MArgDatabase &argData,
											  const MMatrix &worldMatrix);

	MStatus handleStrokeTangentsFlag(const paintingGeom &geom, MArgDatabase &argData,
									 const MMatrix &worldMatrix);

	MStatus handleStrokeColorsFlag(const paintingGeom &geom, MArgDatabase &argData);

	MStatus handleStrokeBackstrokeFlag(const paintingGeom &geom, MArgDatabase &argData);

	MStatus handleStrokeArcLengthFlag(const paintingGeom &geom, MArgDatabase &argData);

	MStatus handleStrokeParentIndexFlag(const paintingGeom &geom, MArgDatabase &argData);

	MStatus handleDipCombinationsFlag(const paintingGeom &geom);

	MStatus handleJsonFlag(const paintingGeom &geom);

	MMatrix getWorldMatrix(MObject &paintingObject, MStatus *st);

	MTransformationMatrix::RotationOrder getRotationOrder(MArgDatabase &argData);

	MAngle::Unit getRotationUnit(MArgDatabase &argData);
};

#endif //	!____
