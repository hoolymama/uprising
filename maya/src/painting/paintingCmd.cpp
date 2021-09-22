#include <maya/MPlugArray.h>
#include <maya/MTypeId.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnPluginData.h>
#include <maya/MFnMatrixData.h>
#include "paintingCmd.h"
#include "paintingData.h"
#include "paintingGeom.h"
#include "cluster.h"
#include "cmdUtils.h"

const MString clusterIdPresenceMsg("You must provide a cluster Id");
const MString clusterIdRangeMsg("Cluster Id not in range");
const MString strokeIdPresenceMsg("You must provide a stroke Id");
const MString strokeIdRangeMsg("Stroke Id not in range");

void *paintingCmd::creator()
{
	return new paintingCmd();
}

MSyntax paintingCmd::newSyntax()
{
	MSyntax syn;

	syn.addFlag(kClusterCountFlag, kClusterCountFlagL);

	syn.addFlag(kClusterIndexFlag, kClusterIndexFlagL, MSyntax::kLong);

	syn.addFlag(kStrokeCountFlag, kStrokeCountFlagL);

	syn.addFlag(kClusterReasonFlag, kClusterReasonFlagL);

	syn.addFlag(kClusterPaintIdFlag, kClusterPaintIdFlagL);

	syn.addFlag(kClusterPotIdFlag, kClusterPotIdFlagL);

	syn.addFlag(kClusterBrushIdFlag, kClusterBrushIdFlagL);

	syn.addFlag(kClusterTravelCutoffFlag, kClusterTravelCutoffFlagL);

	syn.addFlag(kClusterTravelFlag, kClusterTravelFlagL);

	syn.addFlag(kStrokeIndexFlag, kStrokeIndexFlagL, MSyntax::kLong);

	syn.addFlag(kRotateOrderFlag, kRotateOrderFlagL, MSyntax::kString);

	syn.addFlag(kRotateUnitFlag, kRotateUnitFlagL, MSyntax::kString);

	syn.addFlag(kStrokeSpeedLinearFlag, kStrokeSpeedLinearFlagL);

	syn.addFlag(kStrokeSpeedAngularFlag, kStrokeSpeedAngularFlagL);
	
	syn.addFlag(kStrokeApproxDistFlag, kStrokeApproxDistFlagL);

	syn.addFlag(kStrokeLayerIdFlag, kStrokeLayerIdFlagL);

	syn.addFlag(kStrokePositionsFlag, kStrokePositionsFlagL);

	syn.addFlag(kStrokeRotationsFlag, kStrokeRotationsFlagL);

	syn.addFlag(kStrokeArrivalPositionsFlag, kStrokeArrivalPositionsFlagL);

	syn.addFlag(kStrokeArrivalRotationsFlag, kStrokeArrivalRotationsFlagL);

	syn.addFlag(kStrokeDeparturePositionFlag, kStrokeDeparturePositionFlagL);

	syn.addFlag(kStrokeDepartureRotationFlag, kStrokeDepartureRotationFlagL);

	syn.addFlag(kStrokeColorsFlag, kStrokeColorsFlagL);

	syn.addFlag(kStrokeArcLengthFlag, kStrokeArcLengthFlagL);

	syn.addFlag(kStrokeParentIndexFlag, kStrokeParentIndexFlagL);

	syn.addFlag(kGlobalStrokeIndexFlag, kGlobalStrokeIndexFlagL);
 
	syn.addFlag(kToolCombinationsFlag, kToolCombinationsFlagL);


	syn.addFlag(kJsonFlag, kJsonFlagL);

	syn.setObjectType(MSyntax::kSelectionList, 1, 1);

	syn.useSelectionAsDefault(true);

	return syn;
}

MStatus paintingCmd::doIt(const MArgList &args)
{
	MStatus st;

	MSelectionList list;
	MArgDatabase argData(syntax(), args);
	argData.getObjects(list);

	MItSelectionList paintingIter(list, MFn::kPluginDependNode);
	if (paintingIter.isDone())
	{
		displayError("Must pick at least one painting node");
		return MS::kUnknownParameter;
	}

	MObject paintingObject;
	paintingIter.getDependNode(paintingObject);
	MFnDependencyNode paintingFn(paintingObject);

	if (paintingFn.typeId() != painting::id)
	{
		displayError("Must supply one painting node");
		return MS::kUnknownParameter;
	}

	MPlug plugPaintingData = paintingFn.findPlug("output", true, &st);
	MObject dPaintingData;
	st = plugPaintingData.getValue(dPaintingData);
	MFnPluginData fnPaintingData(dPaintingData);
	paintingData *ptd = (paintingData *)fnPaintingData.data();
	if (!ptd)
	{
		displayError("No valid painting data");
		return MS::kUnknownParameter;
	}

	paintingGeom *pGeom = ptd->geometry();

	if (!pGeom)
	{
		displayError("No valid painting geometry");
		return MS::kUnknownParameter;
	}


	if (argData.isFlagSet(kToolCombinationsFlag))
	{
		return handleToolCombinationsFlag(*pGeom);
	}

	if (argData.isFlagSet(kClusterCountFlag))
	{
		return handleClusterCountFlag(*pGeom);
	}

	if (argData.isFlagSet(kStrokeCountFlag))
	{
		return handleStrokeCountFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kClusterReasonFlag))
	{
		return handleClusterReasonFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kClusterPaintIdFlag))
	{
		return handleClusterPaintIdFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kClusterPotIdFlag))
	{
		return handleClusterPotIdFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kClusterBrushIdFlag))
	{
		return handleClusterBrushIdFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kClusterTravelCutoffFlag))
	{
		return handleClusterTravelCutoffFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kClusterTravelFlag))
	{
		return handleClusterTravelFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kStrokeSpeedLinearFlag))
	{
		return handleStrokeSpeedLinearFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kStrokeSpeedAngularFlag))
	{
		return handleStrokeSpeedAngularFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kStrokeApproxDistFlag))
	{
		return handleStrokeApproxDistFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kStrokeLayerIdFlag))
	{
		return handleStrokeLayerIdFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kStrokePositionsFlag))
	{
		MFloatMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokePositionsFlag(*pGeom, argData, wm);
	}

	if (argData.isFlagSet(kStrokeRotationsFlag))
	{
		MFloatMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokeRotationsFlag(*pGeom, argData, wm);
	}

	if (argData.isFlagSet(kStrokeArrivalPositionsFlag))
	{
		MFloatMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokeArrivalPositionsFlag(*pGeom, argData, wm);
	}

	if (argData.isFlagSet(kStrokeArrivalRotationsFlag))
	{
		MFloatMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokeArrivalRotationsFlag(*pGeom, argData, wm);
	}

	if (argData.isFlagSet(kStrokeDeparturePositionFlag))
	{
		MFloatMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokeDeparturePositionFlag(*pGeom, argData, wm);
	}

	if (argData.isFlagSet(kStrokeDepartureRotationFlag))
	{
		MFloatMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokeDepartureRotationFlag(*pGeom, argData, wm);
	}

	if (argData.isFlagSet(kStrokeColorsFlag))
	{
		return handleStrokeColorsFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kStrokeArcLengthFlag))
	{
		return handleStrokeArcLengthFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kStrokeParentIndexFlag))
	{
		return handleStrokeParentIndexFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kGlobalStrokeIndexFlag))
	{
		return handleGlobalStrokeIndexFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kJsonFlag))
	{
		return handleJsonFlag(*pGeom);
	}

	return MS::kUnknownParameter;
}

MTransformationMatrix::RotationOrder paintingCmd::getRotationOrder(MArgDatabase &argData)
{
	MStatus st;
	MString sRotOrder;
	st = argData.getFlagArgument(kRotateOrderFlag, 0, sRotOrder);
	if (st.error())
	{
		displayInfo("No rotation order specified. Default to XYZ");
		return MTransformationMatrix::kXYZ;
	}

	sRotOrder = sRotOrder.toLowerCase();
	MTransformationMatrix::RotationOrder order;

	if (sRotOrder == "xyz")
	{
		return MTransformationMatrix::kXYZ;
	}
	if (sRotOrder == "yzx")
	{
		return MTransformationMatrix::kYZX;
	}
	if (sRotOrder == "zxy")
	{
		return MTransformationMatrix::kZXY;
	}
	if (sRotOrder == "xzy")
	{
		return MTransformationMatrix::kXZY;
	}
	if (sRotOrder == "yxz")
	{
		return MTransformationMatrix::kYXZ;
	}
	if (sRotOrder == "zyx")
	{
		return MTransformationMatrix::kZYX;
	}

	displayInfo("Invalid rotation order. Default to XYZ");
	return MTransformationMatrix::kXYZ;
}

MAngle::Unit paintingCmd::getRotationUnit(MArgDatabase &argData)
{
	MStatus st;
	MString sRotUnit;
	st = argData.getFlagArgument(kRotateUnitFlag, 0, sRotUnit);
	if (st.error())
	{
		displayInfo("No rotation unit specified. Default to Radians");
		return MAngle::kRadians;
	}

	MAngle::Unit unit;
	sRotUnit = sRotUnit.substring(0, 0).toLowerCase();
	if (sRotUnit == "d")
	{
		return MAngle::kDegrees;
	}
	else if (sRotUnit != "r")
	{
		displayInfo("Unknown rotation unit specified. Default to Radians");
	}
	return MAngle::kRadians;
}

MFloatMatrix paintingCmd::getWorldMatrix(MObject &paintingObject, MStatus *st)
{
	MFloatMatrix pmat;
	pmat.setToIdentity();
	MFnDependencyNode paintingFn(paintingObject);

	MPlug plugMatrixMulti = paintingFn.findPlug("worldMatrix", true, st);
	if (st->error())
	{
		return pmat;
	}
	MPlug plugMatrix(plugMatrixMulti.elementByLogicalIndex(0));
	MObject dMatrix;
	plugMatrix.getValue(dMatrix);
	MFnMatrixData fnX(dMatrix);
	return MFloatMatrix(fnX.matrix().matrix);
}

int paintingCmd::getClusterId(const paintingGeom &geom, MArgDatabase &argData,
							  MStatus *status)
{
	int clusterId = -1;
	*status = MS::kSuccess;
	MStatus st = argData.getFlagArgument(kClusterIndexFlag, 0, clusterId);
	if (st.error())
	{
		displayError(clusterIdPresenceMsg);
		*status = MS::kUnknownParameter;
	}
	else if (clusterId >= geom.clusters().size())
	{
		clusterId = -1;
		displayError(clusterIdRangeMsg);
		*status = MS::kUnknownParameter;
	}
	return clusterId;
}

int paintingCmd::getStrokeId(const paintingGeom &geom, MArgDatabase &argData,
							 MStatus *status)
{
	int clusterId = getClusterId(geom, argData, status);
	int strokeId = -1;
	if (!(*status).error())
	{
		MStatus st = argData.getFlagArgument(kStrokeIndexFlag, 0, strokeId);
		if (st.error())
		{
			displayError(strokeIdPresenceMsg);
			*status = MS::kUnknownParameter;
		}
		else
		{
			int numStrokes = geom.clusters()[clusterId].strokes().size();
			if (strokeId >= numStrokes)
			{
				strokeId = -1;
				displayError(strokeIdRangeMsg);
				*status = MS::kUnknownParameter;
			}
		}
	}
	return strokeId;
}

MStatus paintingCmd::handleToolCombinationsFlag(const paintingGeom &geom)
{
	MIntArray result;
	geom.toolCombinations(result);
	setResult(result);
	return MS::kSuccess;
}


MStatus paintingCmd::handleClusterCountFlag(const paintingGeom &geom)
{

	int siz = geom.clusters().size();
	setResult(siz);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeCountFlag(const paintingGeom &geom,
										   MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int numStrokes = geom.clusters()[clusterId].strokes().size();
	setResult(numStrokes);
	return MS::kSuccess;
}

MStatus paintingCmd::handleClusterReasonFlag(const paintingGeom &geom,
											 MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	Cluster::Reason reason = geom.clusters()[clusterId].reason();

	switch (reason)
	{
	case Cluster::kBrush:
		setResult(MString("tool"));
		break;
	case Cluster::kPaint:
		setResult(MString("dip"));
		break;

	default:
		setResult(MString("none"));
		break;
	}
	return MS::kSuccess;
}

MStatus paintingCmd::handleClusterPaintIdFlag(const paintingGeom &geom,
											  MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	setResult(geom.clusters()[clusterId].paintId());
	return MS::kSuccess;
}


MStatus paintingCmd::handleClusterPotIdFlag(const paintingGeom &geom,
											  MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}

	setResult(geom.clusters()[clusterId].potId());
	return MS::kSuccess;
}


MStatus paintingCmd::handleClusterBrushIdFlag(const paintingGeom &geom,
											  MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	setResult(geom.clusters()[clusterId].brushId());
	return MS::kSuccess;
}

MStatus paintingCmd::handleClusterTravelCutoffFlag(const paintingGeom &geom,
												   MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	setResult(geom.clusters()[clusterId].travelCutoff());
	return MS::kSuccess;
}

MStatus paintingCmd::handleClusterTravelFlag(const paintingGeom &geom,
											 MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	setResult(geom.clusters()[clusterId].travel());
	return MS::kSuccess;
}
///////////////////////

MStatus paintingCmd::handleStrokeSpeedLinearFlag(const paintingGeom &geom,
												 MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	setResult(geom.clusters()[clusterId].strokes()[strokeId].linearSpeed());
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeSpeedAngularFlag(const paintingGeom &geom,
												  MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	MAngle::Unit unit = getRotationUnit(argData);

	setResult(geom.clusters()[clusterId].strokes()[strokeId].angularSpeed(unit));
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeApproxDistFlag(const paintingGeom &geom,
												  MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	setResult(geom.clusters()[clusterId].strokes()[strokeId].approximationDistance());
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeLayerIdFlag(const paintingGeom &geom,
												  MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	setResult(geom.clusters()[clusterId].strokes()[strokeId].layerId());
	return MS::kSuccess;
}


MStatus paintingCmd::handleStrokePositionsFlag(const paintingGeom &geom,
											   MArgDatabase &argData, const MFloatMatrix &worldMatrix)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	MFloatPointArray pos;
	geom.clusters()[clusterId].strokes()[strokeId].positions(worldMatrix, pos);
	MDoubleArray result;
	CmdUtils::flatten(pos, result);
	setResult(result);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeRotationsFlag(const paintingGeom &geom,
											   MArgDatabase &argData, const MFloatMatrix &worldMatrix)
{
	MStatus st;
	MTransformationMatrix::RotationOrder order = getRotationOrder(argData);
	MAngle::Unit unit = getRotationUnit(argData);

	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	MFloatVectorArray rot;
	geom.clusters()[clusterId].strokes()[strokeId].rotations(worldMatrix, order, unit,
															 rot);
	MDoubleArray result;
	CmdUtils::flatten(rot, result);
	setResult(result);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeArrivalPositionsFlag(const paintingGeom &geom,
													  MArgDatabase &argData, const MFloatMatrix &worldMatrix)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	MFloatPointArray pos;
	geom.clusters()[clusterId].strokes()[strokeId].arrivalPositions(worldMatrix, pos);
	MDoubleArray result;
	CmdUtils::flatten(pos, result);
	setResult(result);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeArrivalRotationsFlag(const paintingGeom &geom,
													  MArgDatabase &argData, const MFloatMatrix &worldMatrix)
{
	MStatus st;
	MTransformationMatrix::RotationOrder order = getRotationOrder(argData);
	MAngle::Unit unit = getRotationUnit(argData);

	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	MFloatVectorArray rot;
	geom.clusters()[clusterId].strokes()[strokeId].arrivalRotations(worldMatrix, order, unit,
																	rot);
	MDoubleArray result;
	CmdUtils::flatten(rot, result);
	setResult(result);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeDeparturePositionFlag(const paintingGeom &geom,
													   MArgDatabase &argData, const MFloatMatrix &worldMatrix)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	MFloatPoint pos;
	geom.clusters()[clusterId].strokes()[strokeId].departurePosition(worldMatrix, pos);
	MDoubleArray result;
	CmdUtils::flatten(MVector(pos), result);
	setResult(result);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeDepartureRotationFlag(const paintingGeom &geom,
													   MArgDatabase &argData, const MFloatMatrix &worldMatrix)
{
	MStatus st;
	MTransformationMatrix::RotationOrder order = getRotationOrder(argData);
	MAngle::Unit unit = getRotationUnit(argData);

	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	MFloatVector rot;
	geom.clusters()[clusterId].strokes()[strokeId].departureRotation(worldMatrix, order,
																	 unit,
																	 rot);
	MDoubleArray result;
	CmdUtils::flatten(rot, result);
	setResult(result);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeColorsFlag(const paintingGeom &geom, MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	MColorArray colors;
	geom.clusters()[clusterId].strokes()[strokeId].colors(colors);
	MDoubleArray result;
	CmdUtils::flatten(colors, result);
	setResult(result);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeArcLengthFlag(const paintingGeom &geom,
											   MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	setResult(geom.clusters()[clusterId].strokes()[strokeId].calculateArcLength());
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeParentIndexFlag(const paintingGeom &geom,
												 MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	int pid = geom.clusters()[clusterId].strokes()[strokeId].parentId();
	setResult(pid);
	return MS::kSuccess;
}


MStatus paintingCmd::handleGlobalStrokeIndexFlag (const paintingGeom &geom,
												 MArgDatabase &argData)
{
	MStatus st;
	int strokeIndex = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	int gsid = geom.clusters()[clusterId].strokes()[strokeIndex].strokeId();
	setResult(gsid);
	return MS::kSuccess;
}


MStatus paintingCmd::handleJsonFlag(const paintingGeom &geom)
{

	std::vector<Cluster>::const_iterator citer;
	for (citer = geom.clusters().begin(); citer != geom.clusters().end(); citer++)
	{
		int brushId = citer->brushId();
		int paintId = citer->paintId();
		std::vector<Stroke>::const_iterator siter;

		for (siter = citer->strokes().begin(); siter != citer->strokes().end(); siter++)
		{
			int layerId = siter->layerId();
			int parentId = siter->parentId();
			MString strokeJson("{");
			strokeJson += ("\"brushId\":");
			strokeJson += brushId;
			strokeJson += (",\"paintId\":");
			strokeJson += paintId;
			strokeJson += (",\"layerId\":");
			strokeJson += layerId;
			strokeJson += (",\"parentId\":");
			strokeJson += parentId;
			strokeJson += "}";
			appendToResult(strokeJson);
		}
	}
	return MS::kSuccess;
}
