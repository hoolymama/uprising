

# include <maya/MIOStream.h>
# include <math.h>
# include <algorithm>
# include <map>

# include <maya/MFnPluginData.h>
# include <maya/MDoubleArray.h>
# include <maya/MFloatVectorArray.h>
# include <maya/MFloatArray.h>
# include <maya/MFloatPointArray.h>
# include <maya/MPoint.h>

#include <maya/MString.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>


#include <maya/MRampAttribute.h>

#include <maya/MArrayDataHandle.h>
#include <maya/MAngle.h>

#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixArrayData.h>

#include <maya/MFnVectorArrayData.h>

#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MTransformationMatrix.h>


#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MFnEnumAttribute.h>

#include <maya/MFnNurbsCurveData.h>

#include "strokeGeometryData.h"
#include "projectStrokeNode.h"
#include "stroke.h"
#include "backstroke.h"
#include "strokeRotationSpec.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"

const double rad_to_deg = (180 / 3.1415927);
MObject projectStroke::aDensity;
MObject projectStroke::aDensityMap;
MObject projectStroke::aOutPoints;
MObject projectStroke::aProjectionMatrix;

MTypeId projectStroke:: id(k_projectStroke );

projectStroke:: projectStroke() {}

projectStroke:: ~projectStroke() {}

void *projectStroke::creator() {
	return new projectStroke();
}

const double epsilon = 0.0001;

MStatus projectStroke:: initialize()
{
	MStatus st;
	MString method("projectStroke::initialize");

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnUnitAttribute uAttr;
	MFnCompoundAttribute cAttr;
	MFnMatrixAttribute mAttr;
	MFnEnumAttribute eAttr;

	MMatrix identity;
	identity.setToIdentity();

	inheritAttributesFrom("strokeNode");


	aProjectionMatrix = mAttr.create( "projectionMatrix", "pmat",
	                                  MFnMatrixAttribute::kDouble );
	mAttr.setStorable( false );
	mAttr.setHidden( true );
	mAttr.setDefault(identity);
	addAttribute(aProjectionMatrix);



	aDensity = nAttr.create("density", "den", MFnNumericData::kDouble);
	nAttr.setHidden( false );
	nAttr.setKeyable( true );
	nAttr.setDefault( 100 );

	st = addAttribute( aDensity ); er;

	aDensityMap = nAttr.create("densityMap", "denm", MFnNumericData::kDouble);
	nAttr.setHidden( false );
	nAttr.setKeyable( true );
	st = addAttribute( aDensityMap ); er;

	aOutPoints = tAttr.create("outPoints", "opts", MFnData::kVectorArray, &st); er;
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutPoints ); er;

	st = attributeAffects( aDensity, aOutPoints);
	st = attributeAffects( aDensityMap, aOutPoints);
	st = attributeAffects( aSeed, aOutPoints);
	st = attributeAffects( aProjectionMatrix, aOutPoints);

	return (MS::kSuccess );
}


MStatus projectStroke::compute(const MPlug &plug, MDataBlock &data )
{
	MStatus st;
	if (plug != aOutPoints) {
		return (MS::kUnknownParameter );
	}

	long seedVal = data.inputValue( aSeed).asLong()  ;
	srand48(seedVal);
	int numPoints = int(data.inputValue( aDensity).asDouble());

	MVectorArray result(numPoints);

	MMatrix projectionMatrix = data.inputValue(aProjectionMatrix).asMatrix();
	for (int i = 0; i < numPoints; ++i)
	{
		double x = (drand48() * 2.0) - 1.0;
		double y = (drand48() * 2.0) - 1.0;

		result[i] =  MVector(MPoint(x, y, 0.0) * projectionMatrix);
	}

	MDataHandle hOutPoints = data.outputValue(aOutPoints);
	MFnVectorArrayData fnOutPoints;
	MObject dOutPoints = fnOutPoints.create(result);
	hOutPoints.set(dOutPoints);
	st = data.setClean( aOutPoints ); er;

	return MS:: kSuccess;
}

// MStatus projectStroke::generateStrokeGeometry(MDataBlock &data,
//     std::vector < strokeGeom > *geom) const
// {
// 	return MS::kSuccess;
// }

void projectStroke:: postConstructor()
{
	// setExistWithoutInConnections(true);
	// setExistWithoutOutConnections(true);
}
