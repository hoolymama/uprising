
#include <maya/MIOStream.h>
#include <math.h>
#include <maya/MPxNode.h>
#include <maya/MStatus.h>

#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnPluginData.h>
#include <maya/MFloatVector.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnMeshData.h>
#include <maya/MFloatPoint.h>
#include <maya/MFnMesh.h>
#include <maya/MTime.h>
#include <maya/MMatrixArray.h>

#include <mayaMath.h>
#include "errorMacros.h"
#include "jMayaIds.h"

// #include <profile_chain_data.h>

#include "strokeData.h"

#include "strokeMesh.h"

// Data from particle system
MObject strokeMesh::aStrokes; // array of chain point positions

MObject strokeMesh::aNumProfilePoints; // if no profile curves attached, make a regular n-gon with this number of points.
MObject strokeMesh::aProfileScale;	   // cross section global scale
MObject strokeMesh::aCurveSet;		   // set of profile curves
MObject strokeMesh::aProfileCurves;	   // compound attribute to hold many curve sets.

// // UV Coord creation
// MObject strokeMesh::aUWidth; //
// MObject strokeMesh::aUOffset; // 0.0 means whole width will occupy the same U value - good for tubes

// MObject strokeMesh::aVLength; //  for example - 0.2 means whole extrusion takes up 0.2 of V space
// MObject strokeMesh::aVOffset; // v coord offset

MObject strokeMesh::aOutMesh;

const double PI_X2 = 6.283185306;
const double PI = 3.141592653;

MTypeId strokeMesh::id(k_strokeMesh);

strokeMesh::strokeMesh() {}

strokeMesh::~strokeMesh()
{
}

void *strokeMesh::creator()
{
	return new strokeMesh();
}
void strokeMesh::postConstructor() {}

MStatus strokeMesh::initialize()
{
	MStatus st;

	MString method("strokeMesh::initialize");
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnCompoundAttribute cAttr;
	MFnUnitAttribute uAttr;

	aStrokes = tAttr.create("strokes", "stks", strokeData::id);
	tAttr.setReadable(false);
	tAttr.setStorable(false);
	tAttr.setKeyable(true);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	addAttribute(aStrokes);

	aNumProfilePoints = nAttr.create("defaultProfilePoints", "dpps", MFnNumericData::kLong);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(6);
	nAttr.setMin(3);

	addAttribute(aNumProfilePoints);

	aProfileScale = nAttr.create("defaultProfileScale", "dpsc", MFnNumericData::kDouble);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0f);
	addAttribute(aProfileScale);

	aCurveSet = tAttr.create("curveSet", "cst", MFnData::kNurbsCurve);
	tAttr.setStorable(false);
	tAttr.setReadable(false);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	tAttr.setArray(true);
	tAttr.setIndexMatters(false);

	aProfileCurves = cAttr.create("profileCurves", "pcs");
	cAttr.addChild(aCurveSet);
	cAttr.setArray(true);
	cAttr.setIndexMatters(true);
	cAttr.setDisconnectBehavior(MFnAttribute::kReset);
	st = addAttribute(aProfileCurves);

	// aUWidth = nAttr.create("uWidth","uwid", MFnNumericData::kDouble);
	// nAttr.setKeyable(true);
	// nAttr.setStorable(true);
	// nAttr.setDefault(1.0f);
	// addAttribute( aUWidth );

	// aUOffset = tAttr.create("uOffset", "uoff", MFnData::kDoubleArray);
	// tAttr.setStorable(false);
	// tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	// st = addAttribute(aUOffset);mser;

	// aVLength = nAttr.create("vLength","vlen", MFnNumericData::kDouble);
	// nAttr.setKeyable(true);
	// nAttr.setStorable(true);
	// nAttr.setDefault(1.0f);
	// addAttribute( aVLength );

	// aVOffset = tAttr.create("vOffset", "voff", MFnData::kDoubleArray);
	// tAttr.setStorable(false);
	// tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	// st = addAttribute(aVOffset);mser;

	aOutMesh = tAttr.create("outMesh", "om", MFnData::kMesh);
	tAttr.setStorable(false);
	tAttr.setWritable(false);
	addAttribute(aOutMesh);

	attributeAffects(aStrokes, aOutMesh);

	attributeAffects(aCurveSet, aOutMesh); // is this needed?
	attributeAffects(aProfileCurves, aOutMesh);

	attributeAffects(aNumProfilePoints, aOutMesh);
	attributeAffects(aProfileScale, aOutMesh);

	// attributeAffects (aUWidth, aOutMesh);
	// attributeAffects (aUOffset, aOutMesh);

	// attributeAffects (aVLength, aOutMesh);
	// attributeAffects (aVOffset, aOutMesh);

	return MS::kSuccess;
}

// In the absence of explicit profile curve geometry, we make a regular polygon for the extrusion
void strokeMesh::calcDefaultProfile(
	int numProfilePoints,
	double profileScale,
	MPointArray &profileVertices) const
{
	for (unsigned i = 0; i < numProfilePoints; i++)
	{
		double gap = ((i * PI_X2) + PI) / double(numProfilePoints);
		double x = sin(gap) * profileScale * 0.5;
		double y = cos(gap) * profileScale * 0.5;
		profileVertices.append(MPoint(0, y, x));
	}
}

MStatus strokeMesh::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus st;

	MString method("strokeMesh::compute");

	if (plug != aOutMesh)
		return MS::kUnknownParameter;

	// create mesh data structure for output
	MFnMeshData fnC;
	MObject outGeom = fnC.create(&st);
	mser;
	MFnMesh fnM;

	// Get the input strokes
	//////////////////////////////////////////////////
	MDataHandle hInStrokes = data.inputValue(aStrokes, &st);
	msert;
	MObject dInStrokes = hInStrokes.data();
	MFnPluginData fnInStrokes(dInStrokes, &st);
	msert;
	strokeData *sData = (strokeData *)fnInStrokes.data();
	const std::vector<Stroke> *pStrokes = sData->fGeometry;

	if (!pStrokes->size())
	{
		fnM.create(0, 0, MFloatPointArray(), MIntArray(), MIntArray(), outGeom, &st);
		mser;
		MDataHandle hMesh = data.outputValue(aOutMesh, &st);
		hMesh.set(outGeom);
		data.setClean(aOutMesh);
		return MS::kUnknownParameter;
	}

	int numProfilePoints = data.inputValue(aNumProfilePoints).asShort();
	if (numProfilePoints < 3)
		numProfilePoints = 3;
	double profileScale = data.inputValue(aProfileScale).asDouble();

	MPointArray profileVertices;
	calcDefaultProfile(numProfilePoints, profileScale, profileVertices);

	extrude(pStrokes, profileVertices, outGeom);

	MDataHandle hMesh = data.outputValue(aOutMesh, &st);
	hMesh.set(outGeom);
	data.setClean(aOutMesh);

	return MS::kSuccess;
}

void strokeMesh::extrude(const std::vector<Stroke> *pStrokes, const MPointArray &profileVertices, MObject &geom)
{
	MStatus st;

	// normalizeProfileGroupUVs();

	std::vector<Stroke>::const_iterator siter;
	MPointArray curr_verts;
	int nProfileVertices = profileVertices.length();
	MIntArray faceCounts;
	MIntArray connectivity;
	MPointArray vertices;
	int vertexIndex = 0;

	MIntArray vertList;
	MColorArray colorList;
	int k=0;

	for (siter = pStrokes->begin(); siter != pStrokes->end(); siter++)
	{

		MMatrix rMatrix(MMatrix::identity);
		MVector lastTangent(MVector::xAxis);

		Stroke::const_target_iterator titer = siter->targets_begin();
		for (; titer != siter->targets_end(); titer++)
		{
			const MVector &tangent = titer->drawTangent();
			MMatrix deltaRotation = MQuaternion(lastTangent, tangent).asMatrix();
			rMatrix *= deltaRotation;

			MMatrix tf(rMatrix); // copy
			const MFloatMatrix &targetMatrix = titer->matrix();
			tf[3][0] = targetMatrix[3][0];
			tf[3][1] = targetMatrix[3][1];
			tf[3][2] = targetMatrix[3][2];
			// tubeMatrices.append(tf);

			curr_verts.copy(profileVertices);
			mayaMath::transformPoints(tf, 1.0, curr_verts);
			const MColor & color = titer->color();
			for (int i = 0; i < nProfileVertices; ++i, k++)
			{
				vertices.append(curr_verts[i]);
				vertList.append(k);
				colorList.append(color);
			}
			lastTangent = tangent;
		}
		vertexIndex = createTube(siter->size(), nProfileVertices, vertexIndex, faceCounts, connectivity);
	}

	MFnMesh fnMesh;

	fnMesh.create(
		vertices.length(),
		faceCounts.length(),
		vertices,
		faceCounts,
		connectivity, geom, &st);
	mser;

	fnMesh.setVertexColors(colorList,vertList);

}

int strokeMesh::createTube(
	int numTargets,
	int numProfilePoints,
	int vertexIndex,
	MIntArray &faceCounts,
	MIntArray &connectivity)
{

	// start cap
	faceCounts.append(numProfilePoints);
	for (unsigned k = 0; k < numProfilePoints; k++)
	{
		int the_id = vertexIndex + ((numProfilePoints - 1) - k);
		connectivity.append(the_id);
	}

	// quads of the tube
	for (unsigned j = 0; j < (numTargets - 1); j++)
	{

		// for all but the last quad in U, we connect to the next verts around.
		for (unsigned k = 0; k < (numProfilePoints - 1); k++)
		{
			faceCounts.append(4);
			connectivity.append(vertexIndex);
			connectivity.append(vertexIndex + 1);
			connectivity.append(vertexIndex + numProfilePoints + 1);
			connectivity.append(vertexIndex + numProfilePoints);

			vertexIndex += 1;
		}
		// for the last quad, to close the tube, we connect to the first verts in U
		faceCounts.append(4);
		connectivity.append(vertexIndex);
		connectivity.append((vertexIndex + 1) - numProfilePoints);
		connectivity.append(vertexIndex + 1);
		connectivity.append(vertexIndex + numProfilePoints);
		vertexIndex += 1;
	}

	// end cap
	faceCounts.append(numProfilePoints);
	for (unsigned k = 0; k < (numProfilePoints); k++)
	{
		connectivity.append(vertexIndex + k);
	}

	vertexIndex += numProfilePoints; // jump off the end
	return vertexIndex;
}

// unsigned nVerts = outVertices.length();

// fnM.create( nVerts, outFaceCounts.length(),
//     outVertices, outFaceCounts,
//     outConnectivity, outGeom, &st
// );  er;

// if (colorsValid) {
// 	MIntArray vertList(nVerts);
// 	MColorArray colorList(nVerts);
// 	unsigned i = 0;
// 	for (unsigned j = 0; j < pl; ++j)
// 		{

// 		for (int k = 0; k < 8; ++k)
// 		{
// 			colorList.set(MColor(colors[j].x,colors[j].y,colors[j].z ),i);
// 			vertList.set(i,i);
// 			i++;
// 		}
// 	}
// 	fnM.setVertexColors(colorList,vertList);
// }
