#include <target.h>

#include <maya/MFnNurbsCurve.h>

const double rad_to_deg = (180 / 3.1415927);

Target::Target(
  const MObject &curveObject,
  double startDist,
  double endDist,
  double dist,
  double curveLength)	:
	m_matrix()
{
	MStatus st;
	MFnNurbsCurve curveFn(curveObject);

	double strokeLength = endDist - startDist;
	m_param = (dist - startDist) / strokeLength;
	m_curveParam = dist / curveLength;
	double prm = curveFn.findParamFromLength(dist, &st);

	// tangent should be flat
	m_tangent = curveFn.tangent(prm).normal();
	m_tangent.z = 0;
	m_tangent.normalize();

	MPoint pt;
	st = curveFn.getPointAtParam(prm, pt, MSpace::kObject);
	m_matrix[3][0] = pt.x;
	m_matrix[3][1] = pt.y;
	m_matrix[3][2] = 0.0;
}


Target::~Target() {}


void Target::setRotation(double tilt, double bank, double twist, bool follow,
                         bool backstroke)
{
	if (backstroke) {
		tilt = -tilt;
	}
	MVector pos(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);


	if (follow) {
		MMatrix mat = mayaMath::matFromAim(pos, m_tangent, MVector::zAxis, mayaMath::yAxis ,
		                                   mayaMath::zAxisNeg);

		MTransformationMatrix tmat = MTransformationMatrix::identity;
		double3 rot;
		rot[0] = tilt;
		rot[1] = bank;
		rot[2] = twist;
		tmat.setRotation(rot, MTransformationMatrix::kZYX);
		m_matrix =  tmat.asMatrix() * mat;
	}
	else
	{

		MMatrix mat =  mayaMath::matFromAim(MVector::zero, MVector::xNegAxis, MVector::zAxis,
		                                    mayaMath::yAxis ,
		                                    mayaMath::zAxisNeg);
		MVector side((MVector::zAxis ^ m_tangent).normal());

		MMatrix twistMat = MQuaternion(twist, MVector::zAxis).asMatrix();
		MMatrix bankMat = MQuaternion(bank, m_tangent).asMatrix();
		MMatrix tiltMat = MQuaternion(tilt, side).asMatrix();

		mat = mat * twistMat * bankMat * tiltMat;
		mat[3][0] = pos.x;
		mat[3][1] = pos.y;
		mat[3][2] = pos.z;
		m_matrix = mat;
	}
}


double Target::distanceTo(const Target &other) const
{
	MMatrix otherMat = other.matrix();
	return MPoint(
	         m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]
	       ).distanceTo(
	         MPoint(otherMat[3][0], otherMat[3][1], otherMat[3][2])
	       );
}


const MMatrix &Target::matrix() const
{
	return m_matrix;
}

const MVector &Target::tangent() const
{
	return m_tangent;
}

MPoint Target::position() const
{
	return MPoint(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);
}

MVector Target::rotation(
  MTransformationMatrix::RotationOrder order,
  MAngle::Unit unit) const
{
	double rotValue[3];
	MTransformationMatrix tMat(m_matrix);
	tMat.reorderRotation(order);

	MTransformationMatrix::RotationOrder throwAway;
	tMat.getRotation( rotValue, throwAway );
	if (unit == MAngle::kDegrees) {
		rotValue[0] *= rad_to_deg;
		rotValue[1] *= rad_to_deg;
		rotValue[2] *= rad_to_deg;
	}
	return MVector(rotValue[0], rotValue[1], rotValue[2]);
}



const double &Target::param() const
{
	return m_param;
}

const double &Target::curveParam() const
{
	return m_curveParam;
}

void Target::offsetBy(const MVector &offset) {
	m_matrix[3][0] = m_matrix[3][0] + offset.x;
	m_matrix[3][1] = m_matrix[3][1] + offset.y;
	m_matrix[3][2] = m_matrix[3][2] + offset.z;
}

