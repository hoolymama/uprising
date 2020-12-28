#include <target.h>

#include <maya/MFnNurbsCurve.h>

const double rad_to_deg = (180 / 3.1415927);

Target::Target() : m_matrix(),
				   m_tangent(),
				   m_param(0.0),
				   m_curveParam(0.0),
				   m_contact(1.0),
				   m_tilt(0.0),
				   m_bank(0.0),
				   m_twist(0.0),
				   m_u(0.0f),
				   m_v(0.0f),
				   m_color(0.0,0.0,0.0)
{
}

Target::Target(
	const MMatrix &mat,
	const MVector &tangent,
	double param,
	double curveParam) : m_matrix(mat),
						 m_tangent(tangent),
						 m_param(param),
						 m_curveParam(curveParam),
						 m_contact(1.0),
						 m_tilt(0.0),
						 m_bank(0.0),
						 m_twist(0.0),
						 m_u(0.0f),
						 m_v(0.0f)

{
}

Target::Target(
	const MPoint &pt,
	const MVector &tangent,
	double strokeParam,
	double curveParam,
	double contact)
	: m_tangent(tangent),
	  m_param(strokeParam),
	  m_curveParam(curveParam),
	  m_contact(contact),
	  m_matrix(),
	  m_tilt(0.0),
	  m_bank(0.0),
	  m_twist(0.0),
	  m_u(0.0f),
	  m_v(0.0f)
{
	m_matrix[3][0] = pt.x;
	m_matrix[3][1] = pt.y;
	m_matrix[3][2] = pt.z;
}

Target::Target(
	const MFnNurbsCurve &curveFn,
	double dist,
	double startDist,
	double strokeRange,
	double curveLength) : m_contact(1.0),
						  m_u(0.0f),
						  m_v(0.0f)
{
	m_param = (dist - startDist) / strokeRange;
	m_curveParam = dist / curveLength;
	double prm = curveFn.findParamFromLength(dist);

	m_tangent = curveFn.tangent(prm);
	m_tangent.z = 0;
	m_tangent.normalize();

	MPoint pt;
	curveFn.getPointAtParam(prm, pt, MSpace::kObject);
	MMatrix mat;
	mat[3][0] = pt.x;
	mat[3][1] = pt.y;
	mat[3][2] = pt.z;

	m_matrix = mat;
}

Target::~Target() {}

void Target::setTangent(const MVector &tangent)
{
	m_tangent = tangent;
}

void Target::setRotation(double tilt, double bank, double twist, bool follow,
						 bool backstroke)
{
	m_tilt = tilt;
	m_bank = bank;
	m_twist = twist;
	setRotation(follow, backstroke);
}

void Target::setRotation(bool follow, bool backstroke)
{
	double tilt = m_tilt;
	if (backstroke)
	{
		tilt = -tilt;
	}
	MVector pos(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);

	if (follow)
	{
		MMatrix mat = mayaMath::matFromAim(pos, m_tangent, MVector::zAxis, mayaMath::yAxis,
										   mayaMath::zAxisNeg);

		MTransformationMatrix tmat = MTransformationMatrix::identity;
		double3 rot;
		rot[0] = tilt;
		rot[1] = m_bank;
		rot[2] = m_twist;
		tmat.setRotation(rot, MTransformationMatrix::kZYX);
		m_matrix = tmat.asMatrix() * mat;
	}
	else
	{

		MMatrix mat = mayaMath::matFromAim(MVector::zero, MVector::xNegAxis, MVector::zAxis,
										   mayaMath::yAxis,
										   mayaMath::zAxisNeg);
		MVector side((MVector::zAxis ^ m_tangent).normal());

		MMatrix twistMat = MQuaternion(m_twist, MVector::zAxis).asMatrix();
		MMatrix bankMat = MQuaternion(m_bank, m_tangent).asMatrix();
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
			   m_matrix[3][0], m_matrix[3][1], m_matrix[3][2])
		.distanceTo(
			MPoint(otherMat[3][0], otherMat[3][1], otherMat[3][2]));
}

MMatrix Target::directionMatrix(bool backstroke) const
{
	MVector front = backstroke ? -m_tangent : m_tangent;
	MVector side = (MVector::zAxis ^ front).normal();

	MMatrix res = m_matrix;
	res[0][0] = front.x;
	res[0][1] = front.y;
	res[0][2] = front.z;
	res[0][3] = 0.0;
	res[1][0] = side.x;
	res[1][1] = side.y;
	res[1][2] = side.z;
	res[1][3] = 0.0;
	res[2][0] = 0.0;
	res[2][1] = 0.0;
	res[2][2] = 1.0;
	res[2][3] = 0.0;

	return res;
}

const MMatrix &Target::matrix() const
{
	return m_matrix;
}

const MVector &Target::tangent() const
{
	return m_tangent;
}

MPoint Target::position(const MMatrix &space) const
{
	return MPoint(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]) * space;
}

void Target::setPosition(const MPoint &rhs)
{
	m_matrix[3][0] = rhs.x;
	m_matrix[3][1] = rhs.y;
	m_matrix[3][2] = rhs.z;
}

void Target::setMatrix(const MMatrix &rhs)
{
	m_matrix = rhs;
}

void Target::rotate(const MPoint &pivot, const MMatrix &rotation)
{
	m_matrix[3][0] = m_matrix[3][0] - pivot.x;
	m_matrix[3][1] = m_matrix[3][1] - pivot.y;
	m_matrix[3][2] = m_matrix[3][2] - pivot.z;

	m_matrix = m_matrix * rotation;

	m_matrix[3][0] = m_matrix[3][0] + pivot.x;
	m_matrix[3][1] = m_matrix[3][1] + pivot.y;
	m_matrix[3][2] = m_matrix[3][2] + pivot.z;

	m_tangent = m_tangent * rotation;
}

MVector Target::rotation(
	MTransformationMatrix::RotationOrder order,
	MAngle::Unit unit,
	const MMatrix &space) const
{
	double rotValue[3];
	MTransformationMatrix tMat(m_matrix * space);
	tMat.reorderRotation(order);

	MTransformationMatrix::RotationOrder throwAway;
	tMat.getRotation(rotValue, throwAway);
	if (unit == MAngle::kDegrees)
	{
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

void Target::reverseParam()
{
	m_param = 1.0 - m_param;
}

const double &Target::curveParam() const
{
	return m_curveParam;
}

void Target::offsetBy(const MVector &offset)
{
	m_matrix[3][0] = m_matrix[3][0] + offset.x;
	m_matrix[3][1] = m_matrix[3][1] + offset.y;
	m_matrix[3][2] = m_matrix[3][2] + offset.z;
}

/* This is like sliding in or out along the brush axis*/
void Target::offsetLocalZ(double dist)
{

	MVector offset = MVector(m_matrix[2][0], m_matrix[2][1],
							 m_matrix[2][2])
						 .normal() *
					 dist;

	m_matrix[3][0] = m_matrix[3][0] + offset.x;
	m_matrix[3][1] = m_matrix[3][1] + offset.y;
	m_matrix[3][2] = m_matrix[3][2] + offset.z;
}

MVector Target::transform(const MVector &rhs) const
{
	return rhs * m_matrix;
}

void Target::setContact(double contact)
{
	if (contact < 0.0)
	{
		m_contact = 0.0;
	}
	else if (contact > 1.0)
	{
		m_contact = 1.0;
	}
	else
	{
		m_contact = contact;
	}
}

const double &Target::contact() const
{
	return m_contact;
}

void Target::getBorderPoints(
	MPoint &left,
	MPoint &right,
	double width,
	bool flat,
	bool displayContactWidth) const
{

	double contact = m_contact;
	if (flat || (!displayContactWidth))
	{
		contact = 1.0;
	}
	MPoint p = position();
	MVector xOffset;
	if (flat)
	{
		xOffset = (((MVector::xAxis * m_matrix) ^ MVector::zAxis) ^
				   MVector::zAxis)
					  .normal();
	}
	else
	{
		xOffset = (m_tangent ^ MVector::zAxis).normal();
	}
	xOffset *= (width * contact);
	left = p + xOffset;
	right = p - xOffset;
}

void Target::setUV(const MMatrix &inversePlaneMatrix)
{

	MPoint p = ((this->position() * inversePlaneMatrix) * 0.5) + MVector(0.5, 0.5, 0.0);

	m_u = fmax(fmin(p.x, 1.0f), 0.0f);
	m_v = fmax(fmin(p.y, 1.0f), 0.0f);
}

void Target::appendUVsTo(MFloatArray &uVals, MFloatArray &vVals) const
{
	uVals.append(m_u);
	vVals.append(m_v);
}

void Target::applyGlobalTilt(const MFloatVector &gradient)
{
	float mag = gradient.length();
	if (mag < 0.000001)
	{
		return;
	}
	MFloatVector axis = (gradient ^ MFloatVector::zAxis).normal();

	MMatrix rotMat = MQuaternion(mag, axis).asMatrix();

	MMatrix centerMat = MMatrix::identity;
	centerMat[3][0] = -m_matrix[3][0];
	centerMat[3][1] = -m_matrix[3][1];

	m_matrix = m_matrix * centerMat * rotMat * centerMat.inverse();
}

void Target::applyGlobalAim(const MPoint &point)
{

	MVector z1 = MVector(
					 MPoint(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]) - point

					 )
					 .normal();
	// MVector z0 = MVector(m_matrix[2][0],m_matrix[2][1],m_matrix[2][2]).normal()

	MMatrix rotMat = MQuaternion(MVector::zAxis, z1).asMatrix();
	// MMatrix bankMat = MQuaternion(m_bank, m_tangent).asMatrix();
	// MMatrix tiltMat = MQuaternion(tilt, side).asMatrix();

	// MFloatVector axis = (gradient ^ MFloatVector::zAxis).normal();

	// MMatrix rotMat = MQuaternion(mag, axis).asMatrix();

	MMatrix centerMat = MMatrix::identity;
	centerMat[3][0] = -m_matrix[3][0];
	centerMat[3][1] = -m_matrix[3][1];
	centerMat[3][2] = -m_matrix[3][2];

	m_matrix = m_matrix * centerMat * rotMat * centerMat.inverse();
}


void Target::setColor(const MColor &color)
{
	m_color = color;
}
	
const MColor & Target::color() const 
{
	return m_color;
}
