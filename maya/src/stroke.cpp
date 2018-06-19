/***************************************************************************
stroke.cpp  -  description
-------------------
    begin                :  2018
    copyright            : (C) 2018 by Julian Mann
    email                : julian.mann@gmail.com

	***************************************************************************/

#include <maya/MFnNurbsCurve.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MQuaternion.h>
#include <stroke.h>
#include <brush.h>
#include <paint.h>
#include "errorMacros.h"

#include "mayaMath.h"

/*
The two points that describe entry and exit at the boundaries of the stroke
*/
MStatus calcBoundaryPositions(const MPoint &curvePoint, const MVector &tangent,
                              const MVector &planeNormal,
                              const Brush &brush, MVectorArray &result) {
	result = MVectorArray(2, MVector::zero);
	result[0] = curvePoint + ((tangent * brush.liftLength) + (planeNormal *
	                          brush.liftHeight));
	result[1] = curvePoint - (tangent * brush.liftBias);
	return MS::kSuccess;
}

MStatus calcBrushMatrixStroke(const MVector &position, const MVector &tangent,
                              const MVector &planeNormal,
                              const MVector &brushRotate, MMatrix &result,
                              bool isBackstroke) {

	MMatrix mat =  mayaMath::matFromAim(position, tangent, planeNormal, mayaMath::yAxis ,
	                                    mayaMath::zAxisNeg);

	MTransformationMatrix tmat = MTransformationMatrix::identity;
	double3 rot;
	rot[0] = isBackstroke ? -brushRotate.x : brushRotate.x;
	rot[1] = brushRotate.y;
	rot[2] = brushRotate.z;
	tmat.setRotation(rot, MTransformationMatrix::kZYX);
	result =  tmat.asMatrix() * mat;
	return MS::kSuccess;
}


MStatus calcBrushMatrixWorld(const MVector &position,
                             const MVector &tangent,
                             const MVector &planeNormal,
                             const MVector &brushRotate,
                             MMatrix &result,
                             bool isBackstroke) {





	MMatrix mat =  mayaMath::matFromAim(MVector::zero, MVector::xNegAxis, MVector::zAxis,
	                                    mayaMath::yAxis ,
	                                    mayaMath::zAxisNeg);


	// tilt it along the direction of travel
	MVector side(planeNormal ^ tangent);

	double tilt = isBackstroke ? -brushRotate.x : brushRotate.x;
	double bank = brushRotate.y;
	double twist = brushRotate.z;

	MMatrix twistMat = MQuaternion(twist, planeNormal).asMatrix();
	MMatrix bankMat = MQuaternion(bank, tangent).asMatrix();
	MMatrix tiltMat = MQuaternion(tilt, side).asMatrix();

	result = mat * twistMat * bankMat * tiltMat;
	result[3][0] = position.x;
	result[3][1] = position.y;
	result[3][2] = position.z;


	return MS::kSuccess;
}





MStatus calcBrushMatrix(const MVector &position, const MVector &tangent,
                        const MVector &planeNormal,
                        const MVector &brushRotate, MMatrix &result,
                        bool follow, bool isBackstroke = false) {

	if (follow) {

		return calcBrushMatrixStroke(position, tangent, planeNormal, brushRotate,
		                             result, isBackstroke);
	}


	return calcBrushMatrixWorld(position , tangent, planeNormal, brushRotate,
	                            result, isBackstroke);
}





Stroke::Stroke(
  unsigned curveId,
  double startDist,
  double endDist,
  double density,
  const MVector &planeNormal,
  double rotation,
  double translation,
  double pivotFraction,
  const MVector &brushRotate,
  bool follow,
  bool forceDip,
  const Brush &brush,
  const Paint &paint,
  const MObject &curveObject
)	:
	m_curveId(curveId),
	m_targets(),
	m_tangents(),
	m_brush(brush),
	m_paint(paint),
	m_rotation(rotation),
	m_translation(translation),
	m_pivot(),
	m_brushRotate(brushRotate),
	m_follow(follow),
	m_forceDip(forceDip),
	m_arcLength(),
	m_isBackstroke(false)
{
	MStatus st;
	double curveArcLength = endDist - startDist;
	unsigned numPoints = unsigned(density * curveArcLength);

	MFnNurbsCurve curveFn(curveObject, &st);


	if (numPoints < 2) { numPoints = 2; }
	double recip = 1.0 / (numPoints - 1);

	// get an array of params
	MDoubleArray params(numPoints);
	for (unsigned i = 0; i < numPoints; i++) {
		double fraction =  (double(i) * recip);
		double dist = startDist + (fraction * curveArcLength);
		double param = curveFn.findParamFromLength(dist, &st); er;
		params.set(param, i);
	}

	MPoint lastPoint;
	m_arcLength = 0.0;
	MPoint curvePoint;
	MMatrix brushMatrix;


	for (unsigned i = 0; i < numPoints; i++) {

		st = curveFn.getPointAtParam(params[i], curvePoint, MSpace::kObject);

		// get the flat tangent
		MVector tangent = curveFn.tangent(params[i]).normal();
		tangent = (planeNormal ^ (tangent ^ planeNormal)).normal();

		if (i == 0) {

			MVectorArray boundaryPositions;
			st = calcBoundaryPositions(curvePoint, -tangent, planeNormal, brush , boundaryPositions);
			er;



			st = calcBrushMatrix(boundaryPositions[0] , tangent, planeNormal, brushRotate,
			                     brushMatrix, follow);



			m_targets.append(brushMatrix);
			st = calcBrushMatrix(boundaryPositions[1], tangent, planeNormal, brushRotate,
			                     brushMatrix, follow);

			m_targets.append(brushMatrix);
			m_tangents.append(tangent);
			m_tangents.append(tangent);

		}
		else if (i == (numPoints - 1))  {
			MVectorArray boundaryPositions;
			st = calcBoundaryPositions(curvePoint, tangent, planeNormal, brush, boundaryPositions);
			er;

			st = calcBrushMatrix(boundaryPositions[1], tangent, planeNormal, brushRotate,
			                     brushMatrix, follow);
			m_targets.append(brushMatrix);

			st = calcBrushMatrix(boundaryPositions[0], tangent, planeNormal, brushRotate,
			                     brushMatrix, follow);
			m_targets.append(brushMatrix);
			m_tangents.append(tangent);
			m_tangents.append(tangent);
		}
		else {
			st = calcBrushMatrix(curvePoint, tangent, planeNormal, brushRotate,  brushMatrix, follow);
			m_targets.append(brushMatrix);
			m_tangents.append(tangent);
		}

		if (i != 0) {
			m_arcLength += lastPoint.distanceTo(curvePoint);
		}
		lastPoint = curvePoint;
	}

	// For now, set pivot halfway along stroke.
	double dist = startDist + (pivotFraction *  curveArcLength );
	double param = curveFn.findParamFromLength(dist, &st); er;
	st = curveFn.getPointAtParam(param, m_pivot, MSpace::kObject);

}

Stroke::~Stroke() {}

const MMatrixArray &Stroke::targets() const {
	return m_targets;
}
const MVectorArray &Stroke::tangents() const {
	return m_tangents;
}

const Brush &Stroke::brush() const {
	return m_brush;
}
const Paint &Stroke::paint() const {
	return m_paint;
}

const MPoint &Stroke::pivot() const {
	return m_pivot;
}

const MVector &Stroke::brushRotate() const {
	return m_brushRotate;
}

double Stroke::arcLength() const {
	return m_arcLength;
}
double Stroke::rotation() const {
	return m_rotation;
}
double Stroke::translation() const {
	return m_translation;
}

bool Stroke::follow() const {
	return m_follow;
}

bool  Stroke::forceDip() const {
	return m_forceDip;
}

unsigned  Stroke::curveId() const {
	return m_curveId;
}

void Stroke::getPivotUVs(const MMatrix &inversePlaneMatrix, float &u, float &v) const {
	MPoint p = ((m_pivot * inversePlaneMatrix) * 0.5) + MVector(0.5, 0.5, 0.0);
	u = p.x;
	v = p.y;
}


bool Stroke::overlapsPlane(const MMatrix &inversePlaneMatrix) const {
	unsigned len = m_targets.length();
	for (int i = 0; i < len; ++i)
	{
		MPoint p = MPoint(m_targets[i][3][0], m_targets[i][3][1],
		                  m_targets[i][3][2]) * inversePlaneMatrix;
		if (p.x > -1 && p.x < 1 && p.y > -1 && p.y < 1) {
			return true;
		}
	}
	return false;
}

void Stroke::rotate(float rotation, const MVector &axis) {
	double  rotateAmount = rotation * m_rotation;
	MMatrix mat = MQuaternion(rotateAmount, axis).asMatrix();
	unsigned len = m_targets.length();
	MMatrix pivotMat;
	pivotMat[3][0] = m_pivot.x;
	pivotMat[3][1] = m_pivot.y;
	pivotMat[3][2] = m_pivot.z;

	for (int i = 0; i < len; ++i)
	{
		m_targets[i] = ((m_targets[i] * pivotMat.inverse()) * mat) * pivotMat ;
	}
}


void Stroke::translate(const MFloatVector &translation, const MVector &planeNormal) {
	// rotation to planNormal.
	MMatrix mat = MQuaternion(MVector::zAxis, planeNormal).asMatrix();
	MVector trans = (MVector(translation) * m_translation) * mat;
	MMatrix transMat;
	transMat[3][0] = trans.x;
	transMat[3][1] = trans.y;
	transMat[3][2] = trans.z;

	unsigned len = m_targets.length();
	for (int i = 0; i < len; ++i)
	{
		m_targets[i]  = m_targets[i] + transMat ;
	}
}

Stroke::Stroke(
  const Stroke &mother,
  double offset,
  bool reverse,
  const MVector &planeNormal
)	:
	m_curveId(mother.curveId()),
	m_targets(),
	m_tangents(),
	m_brush(mother.brush()),
	m_paint(mother.paint()),
	m_rotation(mother.rotation()),
	m_translation(mother.translation()),
	m_pivot(mother.pivot()),
	m_brushRotate(mother.brushRotate()),
	m_follow(mother.follow()),
	m_forceDip(false),
	m_arcLength(),
	m_isBackstroke(reverse)
{
	MStatus st;
	// m_arcLength = endDist - startDist;
	unsigned numPoints = mother.targets().length();
	MPoint lastPoint;
	m_arcLength = 0.0;
	MPoint thisPoint;
	MMatrix brushMatrix;

	for (int i = 0; i < numPoints; ++i)
	{
		int j = reverse ? numPoints - (i + 1) : i;

		const MMatrix &mm = mother.targets()[j];
		const MVector &tangent = mother.tangents()[j];

		MVector offsetVec = (tangent ^ planeNormal) * offset;
		thisPoint = MPoint(mm[3][0], mm[3][1], mm[3][2]) + offsetVec;

		st = calcBrushMatrix(thisPoint, tangent, planeNormal, m_brushRotate,
		                     brushMatrix, m_follow, reverse);

		m_targets.append(brushMatrix);
		if (reverse) {
			m_tangents.append(-tangent);
		}
		else {
			m_tangents.append(tangent);
		}


		if (i > 1 && i < numPoints - 1) {
			m_arcLength += lastPoint.distanceTo(thisPoint);
		}
		lastPoint = thisPoint;
	}
}
