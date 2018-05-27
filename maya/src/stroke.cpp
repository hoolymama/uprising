/***************************************************************************
stroke.cpp  -  description
-------------------
    begin                :  2018
    copyright            : (C) 2018 by Julian Mann
    email                : julian.mann@gmail.com

	***************************************************************************/

#include <maya/MFnNurbsCurve.h>
#include <maya/MQuaternion.h>
#include <stroke.h>
#include <brush.h>
#include <paint.h>
#include "errorMacros.h"





MStatus calcBoundaryPoints(const MVector &tangent, const MVector &planeNormal,
                           const double3 &params, double tipDist, MVectorArray &result) {

	result = MVectorArray(2, MVector::zero);

	if (tangent.isParallel(planeNormal)) {
		return MS::kFailure;
	}

	MVector flat = (planeNormal ^ (tangent ^ planeNormal)).normal();

	result[0] = (flat * params[0]) + planeNormal * (params[1] + tipDist);
	result[1] = -(flat * params[2]);

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
  double rotation,
  double translation,
  double pivotFraction,
  const Brush &brush,
  const Paint &paint,
  const MObject &curveObject
)	:
	m_points(),
	m_normals(),
	m_brush(brush),
	m_paint(paint),
	m_rotation(rotation),
	m_translation(translation),
	m_pivot(),
	m_arcLength()
{
	MStatus st;
	m_arcLength = endDist - startDist;
	unsigned numPoints = unsigned(density * m_arcLength);

	MFnNurbsCurve curveFn(curveObject, &st);


	if (numPoints < 2) { numPoints = 2; }
	double recip = 1.0f / (numPoints - 1);

	// get an array of params
	MDoubleArray params(numPoints);
	for (unsigned i = 0; i < numPoints; i++) {
		double fraction =  (double(i) * recip);
		double dist = startDist + (fraction * m_arcLength);
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
			st = calcBoundaryPoints(-tangent, planeNormal, attack, brush.tip, boundaryPoints); er;
			m_points.append( MVector(resultPoint + boundaryPoints[0])  ); // up
			m_points.append( MVector(resultPoint + boundaryPoints[1])  ); // bias
			m_normals.append(brushDirection);
			m_normals.append(brushDirection);
		}
		else if (i == (numPoints - 1))  {
			MVectorArray boundaryPoints;
			st = calcBoundaryPoints(tangent, planeNormal, lift, brush.tip, boundaryPoints); er;
			m_points.append( MVector(resultPoint + boundaryPoints[1])  );
			m_points.append( MVector(resultPoint + boundaryPoints[0])  );
			m_normals.append(brushDirection);
			m_normals.append(brushDirection);
		}
		else {
			m_points.append( MVector(resultPoint)  );
			m_normals.append(brushDirection);
		}
	}

	// For now, set pivot halfway along stroke.
	double dist = startDist + (pivotFraction * m_arcLength);
	double param = curveFn.findParamFromLength(dist, &st); er;
	st = curveFn.getPointAtParam(param, m_pivot, MSpace::kObject);

}

stroke::~stroke() {}

const MVectorArray &stroke::points() const {
	return m_points;
}
const MVectorArray &stroke::normals() const {
	return m_normals;
}
const Brush &stroke::brush() const {
	return m_brush;
}
const Paint &stroke::paint() const {
	return m_paint;
}

MPoint stroke::pivot() const {
	return m_pivot;
}

double stroke::arcLength() const {
	return m_arcLength;
}

void stroke::getPivotUVs(const MMatrix &inversePlaneMatrix, float &u, float &v) const {
	MPoint p = ((m_pivot * inversePlaneMatrix) * 0.5) + MVector(0.5, 0.5, 0.0);
	u = p.x;
	v = p.y;
}


bool stroke::overlapsPlane(const MMatrix &inversePlaneMatrix) const {
	unsigned len = m_points.length();
	for (int i = 0; i < len; ++i)
	{
		MPoint p = MPoint(m_points[i]) * inversePlaneMatrix;
		if (p.x > -1 && p.x < 1 && p.y > -1 && p.y < 1) {
			return true;
		}
	}
	return false;
}

void stroke::rotate(float rotation, const MVector &axis) {
	double  rotateAmount = rotation * m_rotation;
	MMatrix mat = MQuaternion(rotateAmount, axis).asMatrix();
	unsigned len = m_points.length();
	for (int i = 0; i < len; ++i)
	{
		m_points[i]  = ((m_points[i] - m_pivot) * mat) + m_pivot ;
		m_normals[i]  = m_normals[i] * mat;
	}
}


void stroke::translate(const MFloatVector &translation, const MVector &planeNormal) {
	MMatrix mat = MQuaternion(MVector::zAxis, planeNormal).asMatrix();
	MVector trans = (MVector(translation) * m_translation) * mat;

	unsigned len = m_points.length();
	for (int i = 0; i < len; ++i)
	{
		m_points[i]  = m_points[i] + trans ;
	}
}



// short stroke::brushId() const {
// 	return m_brushId;
// }

// short stroke::paintId() const {
// 	return m_paintId;
// }





// double stroke::startParam()  const {return m_startParam; }
// double stroke::endParam()  const {return m_endParam; }
// unsigned int stroke::id() const {return m_id;}
