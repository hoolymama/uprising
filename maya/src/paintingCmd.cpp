#include <maya/MPlugArray.h>
#include <maya/MTypeId.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnPluginData.h>
#include <maya/MFnMatrixData.h>
#include "paintingCmd.h"
#include "paintingData.h"
#include "paintingGeom.h"
#include "clusterGeom.h"
#include "cmdUtils.h"
//	static

/*
Command will return data from the painting output plug.
	std::vector<clusterGeom> m_clusters;
	std::map<int, Paint> m_paints;
	std::map<int, Brush> m_brushes;
*/

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

	syn.addFlag(kClusterBrushIdFlag, kClusterBrushIdFlagL);

	syn.addFlag(kClusterTravelCutoffFlag, kClusterTravelCutoffFlagL);

	syn.addFlag(kClusterTravelFlag, kClusterTravelFlagL);

	syn.addFlag(kStrokeIndexFlag, kStrokeIndexFlagL, MSyntax::kLong);

	syn.addFlag(kRotateOrderFlag, kRotateOrderFlagL, MSyntax::kString);

	syn.addFlag(kRotateUnitFlag, kRotateUnitFlagL, MSyntax::kString);

	syn.addFlag(kStrokeSpeedLinearFlag, kStrokeSpeedLinearFlagL);

	syn.addFlag(kStrokeSpeedAngularFlag, kStrokeSpeedAngularFlagL);

	syn.addFlag(kStrokePositionsFlag, kStrokePositionsFlagL);

	syn.addFlag(kStrokeRotationsFlag, kStrokeRotationsFlagL);

	syn.addFlag(kStrokeArrivalPositionsFlag, kStrokeArrivalPositionsFlagL);

	syn.addFlag(kStrokeArrivalRotationsFlag, kStrokeArrivalRotationsFlagL);

	syn.addFlag(kStrokeDeparturePositionFlag, kStrokeDeparturePositionFlagL);

	syn.addFlag(kStrokeDepartureRotationFlag, kStrokeDepartureRotationFlagL);

	syn.addFlag(kStrokeTangentsFlag, kStrokeTangentsFlagL);

	syn.addFlag(kStrokeColorsFlag, kStrokeColorsFlagL);

	syn.addFlag(kStrokeBackstrokeFlag, kStrokeBackstrokeFlagL);

	syn.addFlag(kStrokeArcLengthFlag, kStrokeArcLengthFlagL);
	syn.addFlag(kStrokeParentIndexFlag, kStrokeParentIndexFlagL);

	syn.addFlag(kDipCombinationsFlag, kDipCombinationsFlagL);
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

	if (!(pGeom && pGeom->clusters().size()))
	{
		displayError("No valid painting geometry");
		return MS::kUnknownParameter;
	}

	if (argData.isFlagSet(kDipCombinationsFlag))
	{
		return handleDipCombinationsFlag(*pGeom);
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

	if (argData.isFlagSet(kStrokePositionsFlag))
	{
		MMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokePositionsFlag(*pGeom, argData, wm);
	}

	if (argData.isFlagSet(kStrokeRotationsFlag))
	{
		MMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokeRotationsFlag(*pGeom, argData, wm);
	}

	if (argData.isFlagSet(kStrokeArrivalPositionsFlag))
	{
		MMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokeArrivalPositionsFlag(*pGeom, argData, wm);
	}

	if (argData.isFlagSet(kStrokeArrivalRotationsFlag))
	{
		MMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokeArrivalRotationsFlag(*pGeom, argData, wm);
	}

	if (argData.isFlagSet(kStrokeDeparturePositionFlag))
	{
		MMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokeDeparturePositionFlag(*pGeom, argData, wm);
	}

	if (argData.isFlagSet(kStrokeDepartureRotationFlag))
	{
		MMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokeDepartureRotationFlag(*pGeom, argData, wm);
	}

	if (argData.isFlagSet(kStrokeTangentsFlag))
	{
		MMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokeTangentsFlag(*pGeom, argData, wm);
	}


	if (argData.isFlagSet(kStrokeColorsFlag))
	{
		return handleStrokeColorsFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kStrokeBackstrokeFlag))
	{
		return handleStrokeBackstrokeFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kStrokeArcLengthFlag))
	{
		return handleStrokeArcLengthFlag(*pGeom, argData);
	}
	if (argData.isFlagSet(kStrokeParentIndexFlag))
	{
		return handleStrokeParentIndexFlag(*pGeom, argData);
	}

	if (argData.isFlagSet(kJsonFlag))
	{
		return handleJsonFlag(*pGeom);
	}

	// if ( argData.isFlagSet(kStrokeNormalFlag)) {
	// 	MMatrix wm = getWorldMatrix(paintingObject, &st); msert;
	// 	return handleStrokeNormalFlag(*pGeom, argData, wm);
	// }

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

MMatrix paintingCmd::getWorldMatrix(MObject &paintingObject, MStatus *st)
{
	MMatrix pmat;
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
	pmat = fnX.matrix(st);
	return pmat;
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

MStatus paintingCmd::handleDipCombinationsFlag(const paintingGeom &geom)
{
	MIntArray result;
	geom.dipCombinations(result);
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
	clusterGeom::Reason reason = geom.clusters()[clusterId].reason();

	switch (reason)
	{
	case clusterGeom::kTool:
		setResult(MString("tool"));
		break;
	case clusterGeom::kDip:
		setResult(MString("dip"));
		break;
	case clusterGeom::kTcp:
		setResult(MString("tcp"));
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
	setResult(geom.clusters()[clusterId].strokes()[strokeId].angularSpeed());
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokePositionsFlag(const paintingGeom &geom,
											   MArgDatabase &argData, const MMatrix &worldMatrix)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	MPointArray pos;
	geom.clusters()[clusterId].strokes()[strokeId].positions(worldMatrix, pos);
	MDoubleArray result;
	CmdUtils::flatten(pos, result);
	setResult(result);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeRotationsFlag(const paintingGeom &geom,
											   MArgDatabase &argData, const MMatrix &worldMatrix)
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
	MVectorArray rot;
	geom.clusters()[clusterId].strokes()[strokeId].rotations(worldMatrix, order, unit,
															 rot);
	MDoubleArray result;
	CmdUtils::flatten(rot, result);
	setResult(result);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeArrivalPositionsFlag(const paintingGeom &geom,
													  MArgDatabase &argData, const MMatrix &worldMatrix)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	MPointArray pos;
	geom.clusters()[clusterId].strokes()[strokeId].arrivalPositions(worldMatrix, pos);
	MDoubleArray result;
	CmdUtils::flatten(pos, result);
	setResult(result);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeArrivalRotationsFlag(const paintingGeom &geom,
													  MArgDatabase &argData, const MMatrix &worldMatrix)
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
	MVectorArray rot;
	geom.clusters()[clusterId].strokes()[strokeId].arrivalRotations(worldMatrix, order, unit,
																	rot);
	MDoubleArray result;
	CmdUtils::flatten(rot, result);
	setResult(result);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeDeparturePositionFlag(const paintingGeom &geom,
													   MArgDatabase &argData, const MMatrix &worldMatrix)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	MPoint pos;
	geom.clusters()[clusterId].strokes()[strokeId].departurePosition(worldMatrix, pos);
	MDoubleArray result;
	CmdUtils::flatten(MVector(pos), result);
	setResult(result);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeDepartureRotationFlag(const paintingGeom &geom,
													   MArgDatabase &argData, const MMatrix &worldMatrix)
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
	MVector rot;
	geom.clusters()[clusterId].strokes()[strokeId].departureRotation(worldMatrix, order,
																	 unit,
																	 rot);
	MDoubleArray result;
	CmdUtils::flatten(rot, result);
	setResult(result);
	return MS::kSuccess;
}

///////////////////////

MStatus paintingCmd::handleStrokeTangentsFlag(const paintingGeom &geom,
											  MArgDatabase &argData, const MMatrix &worldMatrix)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	MVectorArray tangents;
	geom.clusters()[clusterId].strokes()[strokeId].tangents(
		worldMatrix, tangents);
	MDoubleArray result;
	CmdUtils::flatten(tangents, result);
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



MStatus paintingCmd::handleStrokeBackstrokeFlag(const paintingGeom &geom,
												MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(geom, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	int clusterId = getClusterId(geom, argData, &st);
	setResult(geom.clusters()[clusterId].strokes()[strokeId].backstroke());
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
	setResult(geom.clusters()[clusterId].strokes()[strokeId].arcLength());
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

MStatus paintingCmd::handleJsonFlag(const paintingGeom &geom)
{

	std::vector<clusterGeom>::const_iterator citer;
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

// MStatus paintingCmd::handleStrokeNormalFlag(const paintingGeom &geom,
//     MArgDatabase &argData, const MMatrix &worldMatrix)
// {
// 	MStatus st;
// 	int strokeId = getStrokeId(geom, argData, &st);
// 	if (st.error()) {
// 		return MS::kUnknownParameter;
// 	}
// 	int clusterId = getClusterId(geom, argData, &st);
// 	MDoubleArray result;
// 	CmdUtils::flatten(geom.clusters()[clusterId].strokes()[strokeId].planeNormal(), result);
// 	setResult(result);
// 	return MS::kSuccess;
// }
