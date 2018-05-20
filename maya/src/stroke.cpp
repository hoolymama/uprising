/***************************************************************************
stroke.cpp  -  description
-------------------
    begin                : Wed Mar 29 2018
    copyright            : (C) 2018 by Julian Mann
    email                : julian.mann@gmail.com
	This is just a position and id to be stored
	id isn't really used at the moment -
	however it will be useful in the future to have it qrapped up in our own data type
	because then we can store say RGB values or other data

	***************************************************************************/

#include <maya/MFnNurbsCurve.h>
#include <maya/MQuaternion.h>
#include <stroke.h>
#include "errorMacros.h"






MStatus calcBoundaryPoints(const MVector &tangent, const MVector &planeNormal,
                           const double3 &params, double tipDist, MVectorArray &result) {

	result = MVectorArray(2, MVector::zero);

	if (tangent.isParallel(planeNormal)) {
		return MS::kFailure;
	}

	MVector flat = (planeNormal ^ (tangent ^ planeNormal)).normal();

	result[0] = (flat * params[0]) + planeNormal * (params[1] + tipDist);
	result[1]  - flat *params[2];

	return MS::kSuccess;

}

MVector calcBrushDirection(const MVector &tangent, const MVector &planeNormal,
                           double elevation) {
	if (tangent.isParallel(planeNormal)) {
		return planeNormal;
	}
	MVector side = (tangent ^ planeNormal);
	MVector flat = (planeNormal ^ side);
	MQuaternion	q(elevation, side);
	return flat.rotateBy(q).normal();
}


stroke::stroke(
  double startDist,
  double endDist,
  double density,
  const MVector &planeNormal,
  const double3 &attack,
  const double3 &lift,
  double elevation,
  double tipDist,
  short brushId,
  short paintId,
  const MObject &curveObject
)	:
	points(),
	normals(),
	m_brushId(brushId),
	m_paintId(paintId)
{
	MStatus st;
	double arcLength = endDist - startDist;
	unsigned numPoints = unsigned(density * arcLength);

	MFnNurbsCurve curveFn(curveObject, &st);


	if (numPoints < 2) { numPoints = 2; }
	double recip = 1.0f / (numPoints - 1);

	// get an array of params
	MDoubleArray params(numPoints);
	for (unsigned i = 0; i < numPoints; i++) {
		double fraction =  (double(i) * recip);
		double dist = startDist + (fraction * arcLength);
		double param = curveFn.findParamFromLength(dist, &st); er;
		params.set(param, i);
	}


	MPoint resultPoint;
	for (unsigned i = 0; i < numPoints; i++) {
		st = curveFn.getPointAtParam(params[i], resultPoint, MSpace::kObject);

		MVector tangent = curveFn.tangent(params[i]);
		MVector brushDirection = calcBrushDirection(tangent, planeNormal, elevation);

		if (i == 0) {
			MVectorArray boundaryPoints;
			st = calcBoundaryPoints(-tangent, planeNormal, attack, tipDist, boundaryPoints); er;
			points.append( MVector(resultPoint + boundaryPoints[0])  );
			points.append( MVector(resultPoint + boundaryPoints[1])  );
			normals.append(brushDirection);
		}
		else if (i == (numPoints - 1))  {
			MVectorArray boundaryPoints;
			st = calcBoundaryPoints(tangent, planeNormal, lift, tipDist, boundaryPoints); er;
			points.append( MVector(resultPoint + boundaryPoints[1])  );
			points.append( MVector(resultPoint + boundaryPoints[0])  );
			normals.append(brushDirection);
		}
		else {
			points.append( MVector(resultPoint)  );
			normals.append(brushDirection);
		}

	}
}

stroke::~stroke() {}




short stroke::brushId() const {
	return m_brushId;
}

short stroke::paintId() const {
	return m_paintId;
}





// double stroke::startParam()  const {return m_startParam; }
// double stroke::endParam()  const {return m_endParam; }
// unsigned int stroke::id() const {return m_id;}
