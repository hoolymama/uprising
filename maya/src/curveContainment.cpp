

#include <math.h>
#include <maya/MFnVectorArrayData.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MFnNurbsCurveData.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFloatPointArray.h>
#include <maya/MGlobal.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMesh.h>

#include <maya/MArrayDataHandle.h>
#include <maya/MArrayDataBuilder.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>

#include "errorMacros.h"
#include "curveContainment.h"

#include "jMayaIds.h"

const double epsilon = 0.0001;

MTypeId curveContainment::id(k_curveContainment);

MObject curveContainment::aCurve;
MObject curveContainment::aMesh;
MObject curveContainment::aContainmentType ;
MObject curveContainment::aSampleDistance ;
MObject curveContainment::aMaxIterations ;
MObject curveContainment::aTolerance ;

MObject curveContainment::aOutLength;
// MObject curveContainment::aOutParam;
// MObject curveContainment::aOutMinPoint;
// MObject curveContainment::aOutMaxPoint;


void *curveContainment::creator () {
	return new curveContainment;
}

/// Post constructor
void
curveContainment::postConstructor()
{
	MPxNode::postConstructor();

	setExistWithoutInConnections(false);
	setExistWithoutOutConnections(true);
}

MStatus curveContainment::initialize () {

	MStatus st;


	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;
	MFnEnumAttribute eAttr;

	aCurve = tAttr.create("curve", "crv", MFnNurbsCurveData::kNurbsCurve, & st); er;
	tAttr.setReadable(false);
	tAttr.setStorable(false);
	tAttr.setArray(true);
	tAttr.setIndexMatters(true);
	st = addAttribute(aCurve); er;

	aMesh = tAttr.create( "mesh", "msh", MFnData::kMesh, &st ); er
	tAttr.setReadable(false);
	st = addAttribute(aMesh);	er;

	aContainmentType = eAttr.create( "containmentType", "cty", curveContainment::kFirst);
	eAttr.addField("first", curveContainment::kFirst);
	eAttr.addField("longest", curveContainment::kLongest);
	eAttr.setKeyable(true);
	eAttr.setHidden(false);
	st = addAttribute( aContainmentType ); er;

	aSampleDistance = nAttr.create( "sampleDistance", "sdc",
	                                MFnNumericData::kDouble);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	addAttribute(aSampleDistance);

	aMaxIterations = nAttr.create( "maxIterations", "mxi",
	                               MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	addAttribute(aMaxIterations);

	aTolerance = nAttr.create( "tolerance", "tol",
	                           MFnNumericData::kDouble);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	addAttribute(aTolerance);

	aOutLength = nAttr.create ("outLength", "ol", MFnNumericData::k2Double);
	nAttr.setStorable (false);
	nAttr.setWritable (false);
	nAttr.setReadable (true);
	nAttr.setArray(true);
	nAttr.setUsesArrayDataBuilder( true );
	addAttribute (aOutLength);

	// aOutMinPoint = nAttr.createPoint( "outMinPoint", "mnpt" );
	// nAttr.setStorable(false);
	// nAttr.setReadable(true);
	// nAttr.setHidden(false);
	// nAttr.setWritable(false);
	// st = addAttribute( aOutMinPoint ); er;

	// aOutMaxPoint = nAttr.createPoint( "outMaxPoint", "mxpt" );
	// nAttr.setStorable(false);
	// nAttr.setReadable(true);
	// nAttr.setHidden(false);
	// nAttr.setWritable(false);
	// st = addAttribute( aOutMaxPoint ); er;

	attributeAffects (aCurve, aOutLength);
	attributeAffects (aMesh, aOutLength);
	attributeAffects (aContainmentType, aOutLength);
	attributeAffects (aSampleDistance, aOutLength);
	attributeAffects (aMaxIterations, aOutLength);
	attributeAffects (aTolerance, aOutLength);

	// attributeAffects (aCurve, aOutMinPoint);
	// attributeAffects (aMesh, aOutMinPoint);
	// attributeAffects (aContainmentType, aOutMinPoint);
	// attributeAffects (aSampleDistance, aOutMinPoint);
	// attributeAffects (aMaxIterations, aOutMinPoint);
	// attributeAffects (aTolerance, aOutMinPoint);

	// attributeAffects (aCurve, aOutMaxPoint);
	// attributeAffects (aMesh, aOutMaxPoint);
	// attributeAffects (aContainmentType, aOutMaxPoint);
	// attributeAffects (aSampleDistance, aOutMaxPoint);
	// attributeAffects (aMaxIterations, aOutMaxPoint);
	// attributeAffects (aTolerance, aOutMaxPoint);
	return MS::kSuccess;
}

curveContainment::curveContainment () {}
curveContainment::~curveContainment () {}


void curveContainment::getLongestChainIterators(
  const std::vector<pointOnCurveInfo> &curvePoints,
  std::vector<pointOnCurveInfo>::const_iterator &longestChainStart,
  std::vector<pointOnCurveInfo>::const_iterator &longestChainEnd)
{

	std::vector<pointOnCurveInfo>::const_iterator chainStart;
	std::vector<pointOnCurveInfo>::const_iterator chainEnd;

	std::vector<pointOnCurveInfo>::const_iterator citer;
	bool inChain = false;
	for (citer = curvePoints.begin(); citer !=  curvePoints.end(); citer++)
	{
		if (citer->inside()) {
			if (!inChain) {
				/*This is the first in a chain*/
				chainStart = citer;
			}
			if (next(citer) == curvePoints.end()) {
				/* its the last element and its inside
				so end is the end of chain
				*/
				chainEnd = curvePoints.end();
				if ((chainEnd - chainStart) > (longestChainEnd - longestChainStart))
				{
					longestChainStart = chainStart;
					longestChainEnd = chainEnd;
				}
			}
			inChain = true;

		}
		else {
			// outside
			if (inChain) {
				/* this is the first out of a chain */
				chainEnd = citer;
				if ((chainEnd - chainStart) > (longestChainEnd - longestChainStart))
				{
					longestChainStart = chainStart;
					longestChainEnd = chainEnd;
				}
			}
			inChain = false;
		}
	}
}




void curveContainment::getFirstChainIterators(
  const std::vector<pointOnCurveInfo> &curvePoints,
  std::vector<pointOnCurveInfo>::const_iterator &chainStart,
  std::vector<pointOnCurveInfo>::const_iterator &chainEnd)
{

	// std::vector<pointOnCurveInfo>::const_iterator chainStart;
	// std::vector<pointOnCurveInfo>::const_iterator chainEnd;

	std::vector<pointOnCurveInfo>::const_iterator citer;
	bool inChain = false;
	for (citer = curvePoints.begin(); citer !=  curvePoints.end(); citer++)
	{
		if (citer->inside()) {
			if (!inChain) {
				/*This is the first in a chain*/
				chainStart = citer;
			}
			if (next(citer) == curvePoints.end()) {
				/* its the last element and its inside
				so end is the end of chain
				*/
				chainEnd = curvePoints.end();
			}
			inChain = true;

		}
		else {
			// outside
			if (inChain) {
				/* this is the first out of a chain */
				chainEnd = citer;
				break;
			}
			inChain = false;
		}
	}
}


MStatus curveContainment::compute(const MPlug &plug, MDataBlock &data) {
	//DBG;
	if (!(
	      plug == aOutLength ||
	      plug.parent() == aOutLength
	      // plug == aOutParam ||
	      // plug == aOutMinPoint ||
	      // plug == aOutMaxPoint

	    )) 	{ return MS::kUnknownParameter; }



	MStatus st;
	MFn::Type type = MFn::kInvalid;

	curveContainment::ContainmentType ctype = curveContainment::ContainmentType(
	      data.inputValue(aContainmentType).asShort());
	double sampleDist = data.inputValue(aSampleDistance).asDouble();
	if (sampleDist < 0.01) {
		sampleDist = 0.01;
	}
	double tolerance = data.inputValue(aTolerance).asDouble();
	if (tolerance < 0.01) {
		tolerance = 0.01;
	}
	int maxIterations = data.inputValue(aMaxIterations).asInt();
	if (maxIterations > 100) {
		maxIterations = 100;
	}

	// Mesh and accelerator
	MObject dMesh =  data.inputValue(aMesh).asMeshTransformed();
	MFnMesh meshFn(dMesh, &st);

	if (st.error()) { return MS::kUnknownParameter; }
	MMeshIsectAccelParams ap = meshFn.autoUniformGridParams();


	MArrayDataHandle hCurves = data.inputArrayValue(aCurve, &st ); ert;
	unsigned nPlugs = hCurves.elementCount();

	MArrayDataHandle     hOutput = data.outputArrayValue( aOutLength, &st ); er;
	MArrayDataBuilder    bOutput = hOutput.builder();

	for (unsigned i = 0; i < nPlugs; i++, hCurves.next()) {
		unsigned index = hCurves.elementIndex(&st);

		MDataHandle hCurve = hCurves.inputValue(&st);
		if (st.error()) {
			continue;
		}
		MObject  dCurve	= hCurve.asNurbsCurveTransformed();


		double startPoint;
		double endPoint;

		st = getLengthsForCurve(ctype, dCurve, meshFn, ap, sampleDist, maxIterations, tolerance,
		                        startPoint,
		                        endPoint);
		if (st.error()) {
			startPoint =  0.0;
			endPoint = 0.0;
		}
		MDataHandle hOut = bOutput.addElement(index);
		double2 &outLength = hOut.asDouble2();
		outLength[0] = startPoint;
		outLength[1] = endPoint;

	}
	hOutput.setAllClean();

	return MS::kSuccess;


}



MStatus curveContainment::getLengthsForCurve(ContainmentType ctype, MObject &dCurve,
    MFnMesh &meshFn, MMeshIsectAccelParams &ap, double sampleDist, int maxIterations,
    double tolerance, double &startLength, double &endLength)
{

	MStatus st;
	MFnNurbsCurve curveFn(dCurve, &st);

	if (st.error()) {
		return st;
	}
	double curveLen = curveFn.length(epsilon);


	int numChunks =  ceil(curveLen / sampleDist);
	sampleDist = curveLen / double(numChunks);

	std::vector<pointOnCurveInfo > curvePoints;

	MFloatPointArray hitPoints;
	double dist = 0;

	MFloatVector rayDirection(MFloatVector::zAxis);
	bool hit;
	int numPoints = numChunks + 1;
	for (int i = 0; i < numPoints; ++i)
	{
		double dist = (i * sampleDist);
		curvePoints.push_back(
		  pointOnCurveInfo( curveFn, meshFn, ap, dist )
		);
	}


	// now go through to find the index of the first and last contiguous inside points.
	std::vector<pointOnCurveInfo>::const_iterator chainStart = curvePoints.begin();
	std::vector<pointOnCurveInfo>::const_iterator chainEnd = curvePoints.begin();

	if (ctype == curveContainment::kFirst) {
		getFirstChainIterators(curvePoints, chainStart, chainEnd);
	}
	else {
		getLongestChainIterators(curvePoints, chainStart, chainEnd);
	}

	if (chainEnd ==  curvePoints.begin()) {
		// MGlobal::displayWarning("Something wrong with curve in curveContainment node");
		return MS::kUnknownParameter;
	}

	/* we want the result in startInsidePoint */
	pointOnCurveInfo startInsidePoint = *chainStart;

	if (chainStart != curvePoints.begin()) {
		// there is an outsidePoint before this
		pointOnCurveInfo startOutsidePoint = *(prev(chainStart));

		for (int i = 0; i < maxIterations; ++i)
		{
			double diff =  (startInsidePoint.distance() - startOutsidePoint.distance());
			if (diff < tolerance) {
				break;
			}
			double halfwayDist = startOutsidePoint.distance() + (diff * 0.5);
			pointOnCurveInfo pt = pointOnCurveInfo(curveFn, meshFn, ap, halfwayDist );
			if (pt.inside()) {
				startInsidePoint = pt;
			}
			else {
				startOutsidePoint = pt;
			}
		}
	}

	pointOnCurveInfo endInsidePoint = *(prev(chainEnd));
	if (chainEnd != curvePoints.end())
	{
		/* chainEnd is the outside point */
		pointOnCurveInfo endOutsidePoint = *chainEnd;

		for (int i = 0; i < maxIterations; ++i)
		{
			double diff =  (endOutsidePoint.distance() - endInsidePoint.distance());
			if (diff < tolerance) {
				break;
			}
			double halfwayDist = endInsidePoint.distance() + (diff * 0.5);
			pointOnCurveInfo pt = pointOnCurveInfo(curveFn, meshFn, ap, halfwayDist );
			if (pt.inside()) {
				endInsidePoint = pt;
			}
			else {
				endOutsidePoint = pt;
			}
		}
	}


	startLength = startInsidePoint.distance();
	endLength = endInsidePoint.distance();

	return MS::kSuccess;
}
