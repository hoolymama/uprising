#include <target.h>

#include <maya/MFnNurbsCurve.h>

Target::Target(
  const MObject &curveObject,
  const double &curveLength,
  const double &strokeStartDist,
  const double &strokeEndDist,
  double dist
)	:
	m_dist(dist),
	m_tangent(),
	m_curvePoint(),
	m_curveFraction(),
	m_strokeFraction(),
	m_brushRotate()
{
	MStatus st;
	MFnNurbsCurve curveFn(curveObject);

	double curveParamStart;
	double curveParamEnd;
	st = curveFn.getKnotDomain(curveParamStart, curveParamEnd);

	if (m_dist < 0) {
		// m_dist is before the start, so extrapolate along the start tangent
		m_tangent = curveFn.tangent(curveParamStart).normal();
		curveFn.getPointAtParam(curveParamStart, m_curvePoint, MSpace::kObject);
		MVector extrapolate = m_tangent * m_dist;
		m_curvePoint = m_curvePoint +  extrapolate;
		m_curveFraction = 0.0;
		m_strokeFraction = 0.0;
	}
	else if (m_dist > curveLength) {
		// m_dist is after the end, so extrapolate along the end tangent
		m_tangent = curveFn.tangent(curveParamEnd).normal();
		curveFn.getPointAtParam(curveParamEnd, m_curvePoint, MSpace::kObject);
		MVector extrapolate = m_tangent * (m_dist - curveLength);
		m_curvePoint = m_curvePoint + extrapolate;
		m_curveFraction = 1.0;
		m_strokeFraction = 1.0;
	}
	else {
		m_curveFraction = m_dist / curveLength;
		m_strokeFraction =
		  (m_dist - strokeStartDist) / (strokeEndDist - strokeStartDist);
		double param = curveFn.findParamFromLength(m_dist, &st);
		m_tangent = curveFn.tangent(param).normal();
		st = curveFn.getPointAtParam(param, m_curvePoint, MSpace::kObject);
	}
}


Target::~Target() {}


void Target::followMatrix(const MVector &planeNormal,  double height, bool backstroke ,
                          MMatrix &mat) const  {

	MVector position = MVector(m_curvePoint + (planeNormal * height));

	mat = mayaMath::matFromAim(position, m_tangent, planeNormal, mayaMath::yAxis ,
	                           mayaMath::zAxisNeg);

	MTransformationMatrix tmat = MTransformationMatrix::identity;
	double3 rot;
	rot[0] = backstroke ? -m_brushRotate.x : m_brushRotate.x;
	rot[1] = m_brushRotate.y;
	rot[2] = m_brushRotate.z;
	tmat.setRotation(rot, MTransformationMatrix::kZYX);
	mat =  tmat.asMatrix() * mat;
}


void Target::flatMatrix(const MVector &planeNormal,  double height, bool backstroke,
                        MMatrix &mat) const  {
	MVector position = MVector(m_curvePoint + (planeNormal * height));

	mat =  mayaMath::matFromAim(MVector::zero, MVector::xNegAxis, MVector::zAxis,
	                            mayaMath::yAxis ,
	                            mayaMath::zAxisNeg);


	// tilt it along the direction of travel
	MVector side((planeNormal ^ m_tangent).normal());

	double tilt = backstroke ? -m_brushRotate.x : m_brushRotate.x;
	double bank = m_brushRotate.y;
	double twist = m_brushRotate.z;

	MMatrix twistMat = MQuaternion(twist, planeNormal).asMatrix();
	MMatrix bankMat = MQuaternion(bank, m_tangent).asMatrix();
	MMatrix tiltMat = MQuaternion(tilt, side).asMatrix();

	mat = mat * twistMat * bankMat * tiltMat;
	mat[3][0] = position.x;
	mat[3][1] = position.y;
	mat[3][2] = position.z;


}

MMatrix Target::matrix(const MVector &planeNormal, double height, bool backstroke,
                       bool follow ) const  {

	// MVector position = MVector(m_curvePoint + (planeNormal * m_height));
	MMatrix mat;
	if (follow) {
		followMatrix(planeNormal, height,  backstroke, mat );
	}
	else {
		flatMatrix(planeNormal, height, backstroke, mat );
	}
	return mat;
}


MVector Target::tangent() const {
	return m_tangent;
}

MPoint Target::curvePoint() const {
	return m_curvePoint;
}

void Target::setCurvePoint(const MPoint &point) {
	m_curvePoint = point;
}
// void Target::setCurvePoint(const MVector &point) {
// 	m_curvePoint = MPoint(point);
// }
void Target::offsetBy(const MVector &offset) {
	m_curvePoint += offset;
}

double Target::strokeFraction() const {
	return m_strokeFraction;
}
double Target::curveFraction() const {
	return m_curveFraction;
}

double Target::reverseStrokeFraction() const {
	return 1.0 - m_strokeFraction;
}

// void Target::setHeight(double height) {
// 	m_height = height;
// }

void Target::setRotation(double tilt, double bank, double twist) {
	m_brushRotate = MVector(tilt,  bank,  twist);
}
