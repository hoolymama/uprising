#include <vector>
#include <maya/MPlugArray.h>
#include <maya/MTypeId.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnPluginData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>

#include "lightPaintingCmd.h"
#include "lightPaintingData.h"

#include "cmdUtils.h"

const MString strokeIdPresenceMsg("You must provide a stroke Id");
const MString strokeIdRangeMsg("Stroke Id not in range");

void *lightPaintingCmd::creator()
{
	return new lightPaintingCmd();
}

MSyntax lightPaintingCmd::newSyntax()
{
	MSyntax syn;

	syn.addFlag(kStrokeCountFlag, kStrokeCountFlagL);

	syn.addFlag(kStrokeIndexFlag, kStrokeIndexFlagL, MSyntax::kLong);

	syn.addFlag(kRotateOrderFlag, kRotateOrderFlagL, MSyntax::kString);

	syn.addFlag(kRotateUnitFlag, kRotateUnitFlagL, MSyntax::kString);

	syn.addFlag(kStrokeSpeedLinearFlag, kStrokeSpeedLinearFlagL);

	syn.addFlag(kStrokeSpeedAngularFlag, kStrokeSpeedAngularFlagL);

	syn.addFlag(kStrokePositionsFlag, kStrokePositionsFlagL);

	syn.addFlag(kStrokeRotationsFlag, kStrokeRotationsFlagL);

	// syn.addFlag(kStrokeTangentsFlag, kStrokeTangentsFlagL);

	syn.addFlag(kStrokeColorsFlag, kStrokeColorsFlagL);

	// syn.addFlag(kStrokeBackstrokeFlag, kStrokeBackstrokeFlagL);

	syn.addFlag(kStrokeArcLengthFlag, kStrokeArcLengthFlagL);

	syn.addFlag(kStrokeParentIndexFlag, kStrokeParentIndexFlagL);

	syn.setObjectType(MSyntax::kSelectionList, 1, 1);

	syn.useSelectionAsDefault(true);

	return syn;
}

MStatus lightPaintingCmd::doIt(const MArgList &args)
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

	if (paintingFn.typeId() != lightPainting::id)
	{
		displayError("Must supply one lightPainting node");
		return MS::kUnknownParameter;
	}

	MPlug plugPaintingData = paintingFn.findPlug("output", true, &st);
	MObject dPaintingData;
	st = plugPaintingData.getValue(dPaintingData);
	MFnPluginData fnPaintingData(dPaintingData);
	lightPaintingData *ptd = (lightPaintingData *)fnPaintingData.data();
	if (!ptd)
	{
		displayError("No valid lightPainting data");
		return MS::kUnknownParameter;
	}


	std::vector<Stroke> *pStrokes = ptd->strokes();

	cerr << "pStrokes->size():" << pStrokes->size() << endl;
	// Brush *pBrush = ptd->brush();

	if (!(pStrokes && pStrokes->size()))
	{
		displayError("No valid painting strokes geometry.");
		return MS::kUnknownParameter;
	}
 
	if (argData.isFlagSet(kStrokeCountFlag))
	{
		return handleStrokeCountFlag(*pStrokes);
	}

	if (argData.isFlagSet(kStrokeSpeedLinearFlag))
	{
		return handleStrokeSpeedLinearFlag(*pStrokes, argData);
	}

	if (argData.isFlagSet(kStrokeSpeedAngularFlag))
	{
		return handleStrokeSpeedAngularFlag(*pStrokes, argData);
	}

	if (argData.isFlagSet(kStrokePositionsFlag))
	{
		MFloatMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokePositionsFlag(*pStrokes, argData, wm);
	}

	if (argData.isFlagSet(kStrokeRotationsFlag))
	{
		MFloatMatrix wm = getWorldMatrix(paintingObject, &st);
		msert;
		return handleStrokeRotationsFlag(*pStrokes, argData, wm);
	}

	// if (argData.isFlagSet(kStrokeTangentsFlag))
	// {
	// 	MFloatMatrix wm = getWorldMatrix(paintingObject, &st);
	// 	msert;
	// 	return handleStrokeTangentsFlag(*pStrokes, argData, wm);
	// }

	if (argData.isFlagSet(kStrokeColorsFlag))
	{
		return handleStrokeColorsFlag(*pStrokes, argData);
	}

	// if (argData.isFlagSet(kStrokeBackstrokeFlag))
	// {
	// 	return handleStrokeBackstrokeFlag(*pStrokes, argData);
	// }

	if (argData.isFlagSet(kStrokeArcLengthFlag))
	{
		return handleStrokeArcLengthFlag(*pStrokes, argData);
	}
	if (argData.isFlagSet(kStrokeParentIndexFlag))
	{
		return handleStrokeParentIndexFlag(*pStrokes, argData);
	}

	return MS::kUnknownParameter;
}

MTransformationMatrix::RotationOrder lightPaintingCmd::getRotationOrder(MArgDatabase &argData)
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

MAngle::Unit lightPaintingCmd::getRotationUnit(MArgDatabase &argData)
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

MFloatMatrix lightPaintingCmd::getWorldMatrix(MObject &paintingObject, MStatus *st)
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
	
	pmat = MFloatMatrix(fnX.matrix().matrix);
	return pmat;
}

int lightPaintingCmd::getStrokeId(const std::vector<Stroke> &strokes, MArgDatabase &argData,
							 MStatus *status)
{

	int strokeId = -1;
	MStatus st = argData.getFlagArgument(kStrokeIndexFlag, 0, strokeId);
	if (st.error())
	{
		displayError(strokeIdPresenceMsg);
		*status = MS::kUnknownParameter;
	}
	else
	{
		int numStrokes = strokes.size();
		if (strokeId >= numStrokes)
		{
			strokeId = -1;
			displayError(strokeIdRangeMsg);
			*status = MS::kUnknownParameter;
		}
	}
	return strokeId;
}

MStatus lightPaintingCmd::handleStrokeCountFlag(const std::vector<Stroke> &strokes)
{
	int numStrokes = strokes.size();
	setResult(numStrokes);
	return MS::kSuccess;
}

MStatus lightPaintingCmd::handleStrokeSpeedLinearFlag(const std::vector<Stroke> &strokes,
												 MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(strokes, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}

	setResult(strokes[strokeId].linearSpeed());
	return MS::kSuccess;
}

MStatus lightPaintingCmd::handleStrokeSpeedAngularFlag(const std::vector<Stroke> &strokes,
												  MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(strokes, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}

	setResult(strokes[strokeId].angularSpeed());
	return MS::kSuccess;
}

MStatus lightPaintingCmd::handleStrokePositionsFlag(const std::vector<Stroke> &strokes,
											   MArgDatabase &argData, const MFloatMatrix &worldMatrix)
{
	MStatus st;
	int strokeId = getStrokeId(strokes, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}

	MFloatPointArray pos;
	strokes[strokeId].positions(worldMatrix, pos);
	MDoubleArray result;
	CmdUtils::flatten(pos, result);
	setResult(result);
	return MS::kSuccess;
}

MStatus lightPaintingCmd::handleStrokeRotationsFlag(const std::vector<Stroke> &strokes,
											   MArgDatabase &argData, const MFloatMatrix &worldMatrix)
{
	MStatus st;
	MTransformationMatrix::RotationOrder order = getRotationOrder(argData);
	MAngle::Unit unit = getRotationUnit(argData);

	int strokeId = getStrokeId(strokes, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}

	MFloatVectorArray rot;
	strokes[strokeId].rotations(worldMatrix, order, unit, rot);
	MDoubleArray result;
	CmdUtils::flatten(rot, result);
	setResult(result);
	return MS::kSuccess;
}
  
 

// MStatus lightPaintingCmd::handleStrokeTangentsFlag(const std::vector<Stroke> &strokes,
// 											  MArgDatabase &argData, const MFloatMatrix &worldMatrix)
// {
// 	MStatus st;
// 	int strokeId = getStrokeId(strokes, argData, &st);
// 	if (st.error())
// 	{
// 		return MS::kUnknownParameter;
// 	}

// 	MFloatVectorArray tangents;
// 	strokes[strokeId].tangents(worldMatrix, tangents);
// 	MDoubleArray result;
// 	CmdUtils::flatten(tangents, result);
// 	setResult(result);
// 	return MS::kSuccess;
// }

MStatus lightPaintingCmd::handleStrokeColorsFlag(const std::vector<Stroke> &strokes, MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(strokes, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}
	MColorArray colors;
	strokes[strokeId].colors(colors);
	MDoubleArray result;
	CmdUtils::flatten(colors, result);
	setResult(result);
	return MS::kSuccess;
}



// MStatus lightPaintingCmd::handleStrokeBackstrokeFlag(const std::vector<Stroke> &strokes,
// 												MArgDatabase &argData)
// {
// 	MStatus st;
// 	int strokeId = getStrokeId(strokes, argData, &st);
// 	if (st.error())
// 	{
// 		return MS::kUnknownParameter;
// 	}
// 	setResult(strokes[strokeId].backstroke());
// 	return MS::kSuccess;
// }

MStatus lightPaintingCmd::handleStrokeArcLengthFlag(const std::vector<Stroke> &strokes,
											   MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(strokes, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}

	setResult(double(strokes[strokeId].calculateArcLength()));
	return MS::kSuccess;
}

MStatus lightPaintingCmd::handleStrokeParentIndexFlag(const std::vector<Stroke> &strokes,
												 MArgDatabase &argData)
{
	MStatus st;
	int strokeId = getStrokeId(strokes, argData, &st);
	if (st.error())
	{
		return MS::kUnknownParameter;
	}

	int pid = strokes[strokeId].parentId();
	setResult(pid);
	return MS::kSuccess;
}
