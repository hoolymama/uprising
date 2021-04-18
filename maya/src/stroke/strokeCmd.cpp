
#include <maya/MItSelectionList.h>
#include <maya/MArgDatabase.h>
#include <maya/MTypeId.h>
#include <maya/MFnPluginData.h>
#include "strokeCmd.h"
#include "paintingData.h"
#include "skeletonStrokeNode.h"
#include "curveStrokeNode.h"
#include "paintStrokeCreator.h"

void *strokeCmd::creator()
{
	return new strokeCmd();
}

MSyntax strokeCmd::newSyntax()
{
	MSyntax syn;

	syn.addFlag(kCoilFlag, kCoilFlagL);

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

	if (!argData.isFlagSet(kCoilFlag))
	{
		displayError("Coil flag not set");
		return MS::kUnknownParameter;
	}

	MItSelectionList strokeNodeIter(list, MFn::kPluginDependNode);
	if (strokeNodeIter.isDone())
	{
		displayError("Must pick at least one stroke node");
		return MS::kUnknownParameter;
	}

	MObject strokeNodeObject;
	strokeNodeIter.getDependNode(strokeNodeObject);
	MFnDependencyNode strokeNodeFn(strokeNodeObject);

	if (strokeNodeFn.typeId() != skeletonStrokeNode::id && strokeNodeFn.typeId() != curveStrokeNode::id)
	{
		displayError("Must supply one stroke node");
		return MS::kUnknownParameter;
	}

	paintStrokeCreator *paintStrokeCreatorNode = (paintStrokeCreator *)strokeNodeFn.userNode();

	setResult(paintStrokeCreatorNode->maxCoil());
	return MS::kSuccess;
}
