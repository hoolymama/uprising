#include <maya/MPlugArray.h>
#include <maya/MTypeId.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnPluginData.h>
#include "paintingCmd.h"
#include "paintingData.h"
#include "paintingGeom.h"
#include "clusterGeom.h"
//	static


/*
Command will return data from the painting output plug.
	std::vector<clusterGeom> m_clusters;
	std::map<short, Paint> m_paints;
	std::map<short, Brush> m_brushes;
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
	syn.addFlag(kStrokeIndexFlag, kStrokeIndexFlagL, MSyntax::kLong);


	syn.addFlag(kStrokeCountFlag, kStrokeCountFlagL);
	syn.addFlag(kClusterReasonFlag, kClusterReasonFlagL);
	syn.addFlag(kClusterNameFlag, kClusterNameFlagL);
	syn.addFlag(kClusterPaintIdFlag, kClusterPaintIdFlagL);
	syn.addFlag(kClusterBrushIdFlag, kClusterBrushIdFlagL);
	syn.addFlag(kClusterTravelCutoffFlag, kClusterTravelCutoffFlagL);
	syn.addFlag(kClusterTravelFlag, kClusterTravelFlagL);

	syn.setObjectType(MSyntax::kSelectionList, 1, 1);

	syn.useSelectionAsDefault(true);

	return syn;

}


MStatus paintingCmd::doIt( const MArgList &args )
{
	MStatus st;

	MSelectionList list;
	MArgDatabase  argData(syntax(), args);
	argData.getObjects(list);

	MItSelectionList paintingIter( list, MFn::kPluginDependNode);
	if (paintingIter.isDone()) {
		displayError("Must pick at least one painting node");
		return MS::kUnknownParameter;
	}

	MObject  paintingObject;
	paintingIter.getDependNode( paintingObject );
	MFnDependencyNode paintingFn( paintingObject );

	if (paintingFn.typeId() != painting::id) {
		displayError("Must supply one painting node");
		return MS::kUnknownParameter;
	}

	MPlug plugPaintingData = 	paintingFn.findPlug ("output", true, &st);
	MObject dPaintingData;
	st = plugPaintingData.getValue(dPaintingData);
	MFnPluginData fnPaintingData(dPaintingData);
	paintingData *ptd  =  (paintingData *)fnPaintingData.data();
	if (! ptd)  {
		displayError("No valid painting data");
		return MS::kUnknownParameter;
	}

	paintingGeom *pGeom = ptd->geometry();

	if (! (pGeom && pGeom->clusters().size())) {
		displayError("No valid painting geometry");
		return MS::kUnknownParameter;
	}

	if (argData.isFlagSet(kClusterCountFlag)) {
		return handleClusterCountFlag(*pGeom);
	}

	if (argData.isFlagSet(kStrokeCountFlag)) {
		return handleStrokeCountFlag(*pGeom, argData);
	}

	if ( argData.isFlagSet(kClusterReasonFlag))
	{
		return handleClusterReasonFlag(*pGeom, argData);
	}

	if ( argData.isFlagSet(kClusterNameFlag))
	{
		return handleClusterNameFlag(*pGeom, argData);
	}

	if ( argData.isFlagSet(kClusterPaintIdFlag))
	{
		return handleClusterPaintIdFlag(*pGeom, argData);
	}

	if ( argData.isFlagSet(kClusterBrushIdFlag))
	{
		return handleClusterBrushIdFlag(*pGeom, argData);
	}

	if ( argData.isFlagSet(kClusterTravelCutoffFlag))
	{
		return handleClusterTravelCutoffFlag(*pGeom, argData);
	}

	if ( argData.isFlagSet(kClusterTravelFlag))
	{
		return handleClusterTravelFlag(*pGeom, argData);
	}






	return MS::kUnknownParameter;
}


int paintingCmd::getClusterId(const paintingGeom &geom, MArgDatabase &argData,
                              MStatus *status)
{
	int clusterId = -1;
	*status = MS::kSuccess;
	MStatus st = argData.getFlagArgument (kClusterIndexFlag, 0, clusterId);
	if (st.error())  {
		displayError(clusterIdPresenceMsg);
		*status = MS::kUnknownParameter;
	}
	else if (clusterId >= geom.clusters().size()) {
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
	if (! (*status).error()) {
		MStatus st = argData.getFlagArgument (kStrokeIndexFlag, 0, strokeId);
		if (st.error())  {
			displayError(strokeIdPresenceMsg);
			*status = MS::kUnknownParameter;
		}
		else {
			int numStrokes = geom.clusters()[clusterId].strokes().size();
			if (strokeId >= numStrokes) {
				strokeId = -1;
				displayError(strokeIdRangeMsg);
				*status = MS::kUnknownParameter;
			}
		}
	}
	return strokeId;
}


MStatus paintingCmd::handleClusterCountFlag(const paintingGeom &geom) {
	int siz = geom.clusters().size();
	setResult(siz);
	return MS::kSuccess;
}

MStatus paintingCmd::handleStrokeCountFlag(const paintingGeom &geom,
    MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st); ert;
	int numStrokes = geom.clusters()[clusterId].strokes().size();
	setResult(numStrokes);
	return MS::kSuccess;
}


MStatus paintingCmd::handleClusterReasonFlag(const paintingGeom &geom,
    MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st); ert;
	clusterGeom::Reason reason = geom.clusters()[clusterId].reason();

	switch (reason)
	{
		case clusterGeom::kTool:
			setResult(MString("tool"));
			break;
		case clusterGeom::kDip:
			setResult(MString("dip"));
			break;
		default:
			setResult(MString("none"));
			break;
	}
	return MS::kSuccess;
}

MStatus paintingCmd::handleClusterNameFlag(const paintingGeom &geom,
    MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st); ert;
	setResult(geom.clusters()[clusterId].name());
	return MS::kSuccess;
}

MStatus paintingCmd::handleClusterPaintIdFlag(const paintingGeom &geom,
    MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st); ert;
	setResult(geom.clusters()[clusterId].paintId());
	return MS::kSuccess;
}

MStatus paintingCmd::handleClusterBrushIdFlag(const paintingGeom &geom,
    MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st); ert;
	setResult(geom.clusters()[clusterId].brushId());
	return MS::kSuccess;
}

MStatus paintingCmd::handleClusterTravelCutoffFlag(const paintingGeom &geom,
    MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st); ert;
	setResult(geom.clusters()[clusterId].travelCutoff());
	return MS::kSuccess;
}

MStatus paintingCmd::handleClusterTravelFlag(const paintingGeom &geom,
    MArgDatabase &argData)
{
	MStatus st;
	int clusterId = getClusterId(geom, argData, &st); ert;
	setResult(geom.clusters()[clusterId].travel());
	return MS::kSuccess;
}


