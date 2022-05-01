
#include <maya/MItSelectionList.h>
#include <maya/MArgDatabase.h>
#include <maya/MTypeId.h>
#include <maya/MFnPluginData.h>
#include "strokeCmd.h"
#include "strokeNodeBase.h"
#include "strokeData.h"
#include "cmdUtils.h"
const MString strokeIdRangeMsg("Stroke Id not in range");

void *strokeCmd::creator()
{
	return new strokeCmd();
}

MSyntax strokeCmd::newSyntax()
{
	MSyntax syn;

	syn.addFlag(kStrokeCountFlag, kStrokeCountFlagL);
	syn.addFlag(kStrokeIndexFlag, kStrokeIndexFlagL, MSyntax::kLong);

	syn.addFlag(kCoilFlag, kCoilFlagL);
	syn.addFlag(kMaxCoilFlag, kMaxCoilFlagL);

	syn.addFlag(kWeightFlag, kWeightFlagL);
	syn.addFlag(kPivotPositionFlag, kPivotPositionFlagL);

	syn.setObjectType(MSyntax::kSelectionList, 1, 1);

	syn.useSelectionAsDefault(true);

	return syn;
}

MStatus strokeCmd::doIt(const MArgList &args)
{
	MStatus st;

	MSelectionList list;
	MArgDatabase argData(syntax(), args);
	argData.getObjects(list);

	MItSelectionList strokeNodeIter(list, MFn::kPluginDependNode);
	if (strokeNodeIter.isDone())
	{
		displayError("Must pick at least one node that outputs strokes");
		return MS::kUnknownParameter;
	}

	MObject strokeNodeObject;
	strokeNodeIter.getDependNode(strokeNodeObject);
	MFnDependencyNode strokeNodeFn(strokeNodeObject);

	MPlug plugStrokeData = strokeNodeFn.findPlug("output", true, &st);
	MObject dStrokeData;
	st = plugStrokeData.getValue(dStrokeData);
	if (st.error())
	{
		displayError("Can't get output attribute");
		return MS::kUnknownParameter;
	}

	MFnPluginData fnStrokeData(dStrokeData);
	strokeData *pStrokeData = (strokeData *)fnStrokeData.data();
	if (!pStrokeData)
	{
		displayError("Not valid stroke data");
		return MS::kUnknownParameter;
	}

	std::vector<Stroke> *pStrokes = pStrokeData->fGeometry;

	if (!pStrokes)
	{
		displayError("No valid painting strokes.");
		return MS::kUnknownParameter;
	}

	int strokeId = getStrokeId(*pStrokes, argData, &st);
	msert;

	if (argData.isFlagSet(kStrokeCountFlag))
	{
		return handleStrokeCountFlag(*pStrokes);
	}
	if (argData.isFlagSet(kCoilFlag))
	{
		return handleCoilFlag(*pStrokes, strokeId);
	}
	if (argData.isFlagSet(kMaxCoilFlag))
	{
		return handleMaxCoilFlag(*pStrokes);
	}
	if (argData.isFlagSet(kWeightFlag))
	{
		return handleWeightFlag(*pStrokes, strokeId);
	}
	if (argData.isFlagSet(kPivotPositionFlag))
	{
		return handlePivotPositionFlag(*pStrokes, strokeId);
	}

	// setResult(paintStrokeCreatorNode->maxCoil());
	return MS::kSuccess;
}
MStatus strokeCmd::handleStrokeCountFlag(const std::vector<Stroke> &strokes)
{
	setResult(int(strokes.size()));
	return MS::kSuccess;
}

MStatus strokeCmd::handleCoilFlag(const std::vector<Stroke> &strokes, int strokeId)
{
	MStatus st;

	MAngle::uiUnit();

	if (strokeId == -1)
	{
		MFloatArray coils;
		for (std::vector<Stroke>::const_iterator curr_stroke = strokes.begin();
			 curr_stroke != strokes.end();
			 curr_stroke++)
		{
			coils.append(MAngle(curr_stroke->coil(), MAngle::kRadians).as(MAngle::uiUnit()));
		}

		MDoubleArray result;
		CmdUtils::flatten(coils, result);
		setResult(result);
		return MS::kSuccess;
	}

	setResult(MAngle(strokes[strokeId].coil(), MAngle::kRadians).as(MAngle::uiUnit()));
	return MS::kSuccess;
}

MStatus strokeCmd::handleMaxCoilFlag(const std::vector<Stroke> &strokes)
{
	MStatus st;

	float maxCoil = 0.0f;
	for (std::vector<Stroke>::const_iterator curr_stroke = strokes.begin();
		 curr_stroke != strokes.end();
		 curr_stroke++)
	{
		const float &coil = curr_stroke->coil();
		if (coil > maxCoil)
		{
			maxCoil = coil;
		}
	}
	setResult(MAngle(maxCoil, MAngle::kRadians).as(MAngle::uiUnit()));
	return MS::kSuccess;
}

MStatus strokeCmd::handleWeightFlag(const std::vector<Stroke> &strokes, int strokeId)
{

	std::vector<Stroke>::const_iterator current_stroke = strokes.begin();
	std::vector<Stroke>::const_iterator end_stroke = strokes.end();
	if (strokeId > -1) {
		current_stroke = strokes.begin()+strokeId;
		end_stroke = std::next(current_stroke);
	}

	MFloatArray weights;
	for (;current_stroke != end_stroke; current_stroke++)
	{
		Stroke::const_target_iterator current_target = current_stroke->targets_begin();
		for (; current_target != current_stroke->targets_end(); current_target++)
		{
			weights.append(current_target->weight());
		}
	}

	MDoubleArray result;
	CmdUtils::flatten(weights, result);
	setResult(result);
	return MS::kSuccess;
}


MStatus strokeCmd::handlePivotPositionFlag(const std::vector<Stroke> &strokes, int strokeId)
{

	if (strokeId < 0 || strokeId >= strokes.size())
	{
		MGlobal::displayError("Invalid stroke index");
		return MS::kUnknownParameter;
	}
	MFloatPoint piv = strokes[strokeId].pivot().position();
	MDoubleArray result;
	result.append(piv.x);
	result.append(piv.y);
	result.append(piv.z);
	setResult(result);
	return MS::kSuccess;
}


int strokeCmd::getStrokeId(const std::vector<Stroke> &strokes, MArgDatabase &argData,
						   MStatus *status)
{
	if (!argData.isFlagSet(kStrokeIndexFlag))
	{
		*status = MS::kSuccess;
		return -1;
	}

	int strokeId;
	MStatus st = argData.getFlagArgument(kStrokeIndexFlag, 0, strokeId);
	if (st.error())
	{
		*status = MS::kFailure;
		return -1;
	}

	int numStrokes = strokes.size();
	if (strokeId < 0 || strokeId >= numStrokes)
	{
		displayError(strokeIdRangeMsg);
		*status = MS::kUnknownParameter;
		return -1;
	}
	*status = MS::kSuccess;
	return strokeId;
}
