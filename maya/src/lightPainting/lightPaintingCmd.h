#ifndef __lightPaintingCmd_H__
#define __lightPaintingCmd_H__

#include <maya/MIOStream.h>
#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include "lightPaintingNode.h"

#define kStrokeCountFlag "-sc"
#define kStrokeCountFlagL "-strokeCount"

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

#define kStrokeLayerIdFlag "-lid"
#define kStrokeLayerIdFlagL "-strokeLayerId"

#define kStrokeBrushIdFlag "-bid"
#define kStrokeBrushIdFlagL "-strokeBrushId"

#define kStrokeApproxDistFlag "-sad"
#define kStrokeApproxDistFlagL "-strokeApoproximationDistance"

#define kStrokeColorsFlag "-scl"
#define kStrokeColorsFlagL "-strokeColors"

#define kStrokeWaitsFlag "-wat"
#define kStrokeWaitsFlagL "-strokeWaits"

#define kStrokeTargetCountFlag  "-tc"
#define kStrokeTargetCountFlagL "-targetCount"

#define kStrokeArcLengthFlag "-sal"
#define kStrokeArcLengthFlagL "-strokeArcLength"

#define kStrokeParentIndexFlag "-spi"
#define kStrokeParentIndexFlagL "-strokeParentIndex"

class lightPaintingCmd : public MPxCommand

{
public:
	lightPaintingCmd() {}
	virtual ~lightPaintingCmd() {}

	MStatus doIt(const MArgList &args);

	static void *creator();

	static MSyntax newSyntax();

private:
	MStatus handleStrokeCountFlag(const std::vector<Stroke> &strokes);

	int getStrokeId(const std::vector<Stroke> &strokes, MArgDatabase &argData,
					MStatus *status);

	MStatus handleStrokeSpeedLinearFlag(const std::vector<Stroke> &strokes, MArgDatabase &argData);

	MStatus handleStrokeSpeedAngularFlag(const std::vector<Stroke> &strokes, MArgDatabase &argData);

	MStatus handleStrokeApproxDistFlag(const std::vector<Stroke> &strokes, MArgDatabase &argData);

	MStatus handleStrokeLayerIdFlag(const std::vector<Stroke> &strokes, MArgDatabase &argData);
	MStatus handleStrokeBrushIdFlag(const std::vector<Stroke> &strokes, MArgDatabase &argData);

	MStatus handleStrokePositionsFlag(const std::vector<Stroke> &strokes, MArgDatabase &argData,
									  const MFloatMatrix &worldMatrix);

	MStatus handleStrokeRotationsFlag(const std::vector<Stroke> &strokes, MArgDatabase &argData,
									  const MFloatMatrix &worldMatrix);

	MStatus handleStrokeColorsFlag(
		const std::vector<Stroke> &strokes,
		MArgDatabase &argData,
		float colorGain,
		float whiteGain);

	MStatus handleStrokeWaitsFlag(
		const std::vector<Stroke> &strokes,
		MArgDatabase &argData,
		float waitGain);

	MStatus handleStrokeTargetCountFlag(const std::vector<Stroke> &strokes, MArgDatabase &argData);

	MStatus handleStrokeBackstrokeFlag(const std::vector<Stroke> &strokes, MArgDatabase &argData);

	MStatus handleStrokeArcLengthFlag(const std::vector<Stroke> &strokes, MArgDatabase &argData);

	MStatus handleStrokeParentIndexFlag(const std::vector<Stroke> &strokes, MArgDatabase &argData);

	MFloatMatrix getWorldMatrix(MObject &paintingObject, MStatus *st);

	MTransformationMatrix::RotationOrder getRotationOrder(MArgDatabase &argData);

	MAngle::Unit getRotationUnit(MArgDatabase &argData);
};

#endif
