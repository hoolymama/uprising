#include <maya/MPlugArray.h>
#include <maya/MTypeId.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnPluginData.h>
#include <maya/MFnMatrixData.h>
#include "brushCmd.h"
#include "brushData.h"
#include "brush.h"
#include "cmdUtils.h"



/*
Command will return data from the brush output plug.
	std::vector<clusterGeom> m_clusters;
	std::map<int, Paint> m_paints;
	std::map<int, Brush> m_brushes;
*/


void *brushCmd::creator()
{
	return new brushCmd();
}

MSyntax brushCmd::newSyntax()
{
	MSyntax syn;


	syn.addFlag( kTrianglesFlag	, kTrianglesFlagL);
	syn.addFlag( kTcpFlag	, kTcpFlagL);
	// syn.addFlag( kPaintingFlag	, kPaintingFlagL);
	// syn.addFlag( kDipFlag	, kDipFlagL);
	// syn.addFlag( kWipeFlag	, kWipeFlagL);


	syn.setObjectType(MSyntax::kSelectionList, 1, 1);

	syn.useSelectionAsDefault(false);

	return syn;

}

MStatus brushCmd::doIt( const MArgList &args )
{
	MStatus st;

	MSelectionList list;
	MArgDatabase  argData(syntax(), args);
	argData.getObjects(list);

	MString msg("Must provide a valid brush plug");
	MItSelectionList brushIter( list, MFn::kPluginDependNode);
	if (brushIter.isDone()) {
		displayError(msg);
		return MS::kUnknownParameter;
	}



	MObject  brushObject;
	brushIter.getDependNode( brushObject );

	MPlug brushPlug;
	st = 	brushIter.getPlug (brushPlug);
	// cerr << "PLUG NAME: " << brushPlug.name() << endl;

	MFnDependencyNode brushFn( brushObject );
	if (brushFn.typeId() != brushNode::id) {
		displayError(msg);
		return MS::kUnknownParameter;
	}

	// MPlug plugBrushData = 	brushFn.findPlug (dataPlugName, true, &st);

	MObject dbrushData;
	st = brushPlug.getValue(dbrushData);
	MFnPluginData fnbrushData(dbrushData);
	brushData *ptd  =  (brushData *)fnbrushData.data();
	if (! ptd)  {
		displayError("No valid brush data");
		return MS::kUnknownParameter;
	}

	Brush *pGeom = ptd->fGeometry;

	if (argData.isFlagSet(kTrianglesFlag)) {
		return handleTrianglesFlag(*pGeom);
	}
	else if (argData.isFlagSet(kTcpFlag))
	{
		return handleTcpFlag(*pGeom);
	}
	return MS::kUnknownParameter;
}

MStatus brushCmd::handleTcpFlag(const Brush &geom)
{
	MFloatMatrix mat = geom.tcp();
	MDoubleArray result;
	CmdUtils::flatten(mat, result );
	setResult(result);
	return MS::kSuccess;
}

MStatus brushCmd::handleTrianglesFlag(const Brush &geom)
{
	MFloatPointArray triangles;
	geom.getTriangles(triangles);

	MDoubleArray result;
	CmdUtils::flatten(triangles, result );
	setResult(result);
	return MS::kSuccess;
}
