

# include <maya/MIOStream.h>
# include <math.h>

# include <maya/MFnPluginData.h>
# include <maya/MDoubleArray.h>
# include <maya/MFloatVectorArray.h>
# include <maya/MFloatArray.h>
# include <maya/MFloatPointArray.h>
# include <maya/MPoint.h>

#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnVectorArrayData.h>



#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>


#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixArrayData.h>


#include <maya/MFnMatrixAttribute.h>

#include "projectionPoints.h"

#include <jMayaIds.h>
#include "mayaMath.h"

#include "dot_data.h"
#include "dot_tree.h"

#include "textureSampling.h"

// #include "CImg.h"
// using namespace cimg_library;

const double rad_to_deg = (180 / 3.1415927);
MObject projectionPoints::aDensity;
MObject projectionPoints::aRadius;
MObject projectionPoints::aDensityMap;
MObject projectionPoints::aIterations;
MObject projectionPoints::aNeighbors;
MObject projectionPoints::aMagnitude;
MObject projectionPoints::aOutPoints;
MObject projectionPoints::aProjectionMatrix;

MObject projectionPoints::aSeed;


MTypeId projectionPoints:: id(k_projectionPoints );

projectionPoints:: projectionPoints() {}

projectionPoints:: ~projectionPoints() {}

void *projectionPoints::creator() {
	return new projectionPoints();
}

// const double epsilon = 0.0001;

MStatus projectionPoints:: initialize()
{
	MStatus st;
	MString method("projectionPoints::initialize");

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	MFnMatrixAttribute mAttr;


	MMatrix identity;
	identity.setToIdentity();


	aProjectionMatrix = mAttr.create( "projectionMatrix", "pmat",
	                                  MFnMatrixAttribute::kFloat );
	mAttr.setStorable( false );
	mAttr.setHidden( false );
	nAttr.setKeyable(true);

	mAttr.setDefault(identity);
	addAttribute(aProjectionMatrix);



	aDensity = nAttr.create("density", "den", MFnNumericData::kFloat);
	nAttr.setHidden( false );
	nAttr.setKeyable( true );
	nAttr.setDefault( 100 );

	st = addAttribute( aDensity );

	aRadius = nAttr.create("radius", "rad", MFnNumericData::kFloat);
	nAttr.setHidden( false );
	nAttr.setKeyable( true );
	nAttr.setDefault( 100 );
	st = addAttribute(  aRadius );

	aDensityMap = nAttr.createColor( "densityMap", "denm");
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setConnectable(true);
	addAttribute(aDensityMap);


	aSeed = nAttr.create( "seed", "sd", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0);
	st = addAttribute(aSeed);

	aIterations = nAttr.create( "iterations", "it", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1);
	st = addAttribute(aIterations);

	aNeighbors = nAttr.create( "neighbors", "nbs", MFnNumericData::kInt);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0);
	st = addAttribute(aNeighbors);

	aMagnitude = nAttr.create("magnitude", "mag", MFnNumericData::kFloat);
	nAttr.setHidden( false );
	nAttr.setKeyable( true );
	nAttr.setDefault( 100 );
	st = addAttribute(  aMagnitude );

	aOutPoints = tAttr.create("outPoints", "opts", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutPoints );

	st = attributeAffects( aDensity, aOutPoints);
	st = attributeAffects( aRadius, aOutPoints);
	st = attributeAffects( aDensityMap, aOutPoints);
	st = attributeAffects( aSeed, aOutPoints);
	st = attributeAffects( aProjectionMatrix, aOutPoints);

	st = attributeAffects(aIterations, aOutPoints);
	st = attributeAffects(aNeighbors, aOutPoints);
	st = attributeAffects(aMagnitude, aOutPoints);


	return (MS::kSuccess );
}



void projectionPoints::makeDots(MDataBlock &data, std::vector<dotData> &dots)
{

	long seedVal = data.inputValue( aSeed).asLong()  ;
	srand48(seedVal);
	int numPoints = int(data.inputValue( aDensity).asFloat());
	float radius = data.inputValue(aRadius).asFloat();
	MFloatMatrix proj = data.inputValue(aProjectionMatrix).asFloatMatrix();

	MFloatArray uVals(numPoints);
	MFloatArray vVals(numPoints);
	for (int i = 0; i < numPoints; ++i)
	{
		uVals.set(float(drand48()), i);
		vVals.set(float(drand48()), i);
	}

	MObject thisObj = thisMObject();
	MFloatVectorArray colors;
	MStatus st = sampleUVTexture(thisObj, projectionPoints::aDensityMap, uVals, vVals,
	                             colors) ;

	if (! st.error()) {
		int j = 0;
		for (int i = 0; i < numPoints; ++i)
		{
			if (drand48() < colors[i].x) {
				dots.push_back(dotData(proj, uVals[i], vVals[i], radius, colors[i] , j)) ;
				j++;
			}
		}
	}
	else {
		MFloatVector densityVals(1.0f, 1.0f, 1.0f);
		for (int i = 0; i < numPoints; ++i)
		{
			dotData d(proj, uVals[i], vVals[i], radius, densityVals , i);
			dots.push_back(d) ;
		}
	}
}




MStatus projectionPoints::compute(const MPlug &plug, MDataBlock &data )
{
	MStatus st;
	if (plug != aOutPoints) {
		return (MS::kUnknownParameter );
	}

	int iterations = int(data.inputValue(aIterations).asInt());
	int neighbors = int(data.inputValue(aNeighbors).asInt());
	float mag = data.inputValue(aMagnitude).asFloat();

	std::vector<dotData> dots;
	makeDots(data, dots);
	int numPoints = dots.size();

	for (int i = 0; i < iterations; ++i)
	{
		dotTree *pTree = new dotTree();
		pTree->populate(dots);
		pTree->build();

		for (unsigned i = 0; i < numPoints; i++ ) {

			KNN_PD_QUEUE *q = new KNN_PD_QUEUE;
			for (int n = 0; n < neighbors; n++) {
				knnSearchPdData k;
				k.weakness = 1.0f;
				k.dot = 0;
				q->push(k);
			}

			const dotData *searchDot = pTree->dots()->at(i);

			pTree->strongestNPoints( pTree->root(), searchDot, q);

			JVector2D repulsion;
			while (!q->empty()) {
				if (q->top().dot) {
					if (q->top().hasDifference()) {
						repulsion += q->top().force();
					}
				}
				q->pop();
			}
			repulsion *= mag;
			dots[i].push(repulsion);
			delete q;
		}
		if (pTree) {delete pTree; pTree = 0;}
	}

	MVectorArray result;
	std::vector<dotData>::const_iterator citer;
	for (citer = dots.begin(); citer != dots.end(); citer++)
	{
		result.append(citer->asVector());
	}


	MDataHandle hOutPoints = data.outputValue(aOutPoints);
	MFnVectorArrayData fnOutPoints;
	MObject dOutPoints = fnOutPoints.create(result);
	hOutPoints.set(dOutPoints);
	st = data.setClean( aOutPoints );


	return MS:: kSuccess;
}

void projectionPoints:: postConstructor()
{
	// setExistWithoutInConnections(true);
	// setExistWithoutOutConnections(true);
}
