#include <target.h>

#include <maya/MFnNurbsCurve.h>

const double rad_to_deg = (180 / 3.1415927);

MFloatVector getSide(const MFloatVector &v1, const MFloatVector &v2)
{
	MFloatVector v = v1 ^ v2;

	if (v.isEquivalent(MFloatVector::zero))
	{
		if (v1.x > v1.y && v1.x > v1.z) // X biggest
		{
			v = v1 ^ MFloatVector::yAxis;
		}
		else if (v1.y > v1.x && v1.y > v1.z) // Y biggest
		{
			v = v1 ^ MFloatVector::xAxis;
		}
		else // Z biggest
		{
			v = v1 ^ MFloatVector::xAxis;
		}
	}
	v.normalize();
	return v;
}


Target::Target() : m_matrix(),
				   m_drawTangent(),
				   m_weight(1.0f),
				   m_radius(0.0f),
				   m_color(0.0, 0.0, 0.0, 0.0),
				   m_wait(0.0f)
{
}

Target::Target(
	const MFloatMatrix &mat,
	const MFloatVector &tangent,
	float weight)
	: Target()
{
	m_matrix = mat;
	m_drawTangent = tangent;
	m_weight = weight;
}

Target::Target(
	const MFloatMatrix &mat,
	float weight)
	: Target()
{
	m_matrix = mat;
	m_weight = weight;
}

Target::Target(
	const MFloatMatrix &mat,
	float weight,
	const MColor &color)
	: Target()
{
	m_matrix = mat;
	m_weight = weight;
	m_color = color;
}

Target::Target(
	const MFloatMatrix &mat,
	const MFloatVector &tangent,
	float weight,
	const MColor &color)
	: Target()
{
	m_drawTangent = tangent;
	m_weight = weight;
	m_color = color;
}

Target::~Target() {}

void Target::setDrawTangent(const MFloatVector &tangent)
{
	m_drawTangent = tangent;
}

MFloatVector Target::xAxis() const
{
	return MFloatVector(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2]);
}

MFloatVector Target::yAxis() const
{
	return MFloatVector(m_matrix[1][0], m_matrix[1][1], m_matrix[1][2]);
}

MFloatVector Target::zAxis() const
{
	return MFloatVector(m_matrix[2][0], m_matrix[2][1], m_matrix[2][2]);
}

void Target::applyAxisAngleRotation(const MFloatVector &axis, float angle)
{
	MFloatMatrix rotMat = MFloatMatrix(
		MQuaternion(angle, axis).asMatrix().matrix);
	MFloatMatrix centerMat;
	centerMat[3][0] = m_matrix[3][0];
	centerMat[3][1] = m_matrix[3][1];
	centerMat[3][2] = m_matrix[3][2];
	m_matrix = m_matrix * centerMat.inverse() * rotMat * centerMat;
}

void Target::applyTilt(float angle)
{
	MFloatVector axis = MFloatVector::xAxis * m_matrix;
	applyAxisAngleRotation(axis, angle);
}

void Target::applyBank(float angle)
{
	MFloatVector axis = MFloatVector::yAxis * m_matrix;
	applyAxisAngleRotation(axis, angle);
}

void Target::applyTwist(float angle)
{
	MFloatVector axis = MFloatVector::zAxis * m_matrix;
	applyAxisAngleRotation(axis, angle);
}

void Target::applyTiltBankTwist(float tilt, float bank, float twist, PaintingEnums::BrushRotateOrder order)
{
	switch (order)
	{
	case PaintingEnums::kTiltBankTwist:
		applyTilt(tilt);
		applyBank(bank);
		applyTwist(twist);
		break;
	case PaintingEnums::kTiltTwistBank:
		applyTilt(tilt);
		applyTwist(twist);
		applyBank(bank);
		break;
	case PaintingEnums::kTwistBankTilt:
		applyTwist(twist);
		applyBank(bank);
		applyTilt(tilt);
		break;
	case PaintingEnums::kTwistTiltBank:
		applyTwist(twist);
		applyTilt(tilt);
		applyBank(bank);
		break;
	case PaintingEnums::kBankTiltTwist:
		applyBank(bank);
		applyTilt(tilt);
		applyTwist(twist);
		break;
	case PaintingEnums::kBankTwistTilt:
		applyBank(bank);
		applyTwist(twist);
		applyTilt(tilt);
	}
}

float Target::distanceTo(const Target &other) const
{
	MFloatMatrix otherMat = other.matrix();
	return MFloatPoint(
			   m_matrix[3][0], m_matrix[3][1], m_matrix[3][2])
		.distanceTo(
			MFloatPoint(otherMat[3][0], otherMat[3][1], otherMat[3][2]));
}

// For Arrow Drawing
MFloatMatrix Target::viewMatrix(
	const MFloatVector &viewNormal) const
{

	MFloatVector side = getSide(viewNormal, m_drawTangent);

	MFloatVector tangent = (side ^ viewNormal).normal();

	MFloatMatrix res = m_matrix;
	res[0][0] = tangent.x;
	res[0][1] = tangent.y;
	res[0][2] = tangent.z;
	res[0][3] = 0.0;
	res[1][0] = side.x;
	res[1][1] = side.y;
	res[1][2] = side.z;
	res[1][3] = 0.0;
	res[2][0] = viewNormal.x;
	res[2][1] = viewNormal.y;
	res[2][2] = viewNormal.z;
	res[2][3] = 0.0;

	return res;
}

void Target::getBorderPoints(
	const MFloatVector &planeNormal,
	MFloatPoint &left,
	MFloatPoint &right,
	float width,
	bool flatBrush,
	bool displayWeightWidth) const
{
	float weight = m_weight;

	if (flatBrush || (!displayWeightWidth))
	{
		weight = 1.0f;
	}
	MPoint p = position();

    MFloatVector side = getSide(planeNormal, m_drawTangent); 
	MVector xOffset = side * width * weight;
	left = p + xOffset;
	right = p - xOffset;
}

const MFloatMatrix &Target::matrix() const
{
	return m_matrix;
}

const MFloatVector &Target::drawTangent() const
{
	return m_drawTangent;
}

MFloatPoint Target::position(const MFloatMatrix &space) const
{
	return MFloatPoint(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]) * space;
}

MFloatPoint Target::position() const
{
	return MFloatPoint(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);
}




void Target::setPosition(const MFloatPoint &rhs)
{
	m_matrix[3][0] = rhs.x;
	m_matrix[3][1] = rhs.y;
	m_matrix[3][2] = rhs.z;
}

void Target::setMatrix(const MFloatMatrix &rhs)
{
	m_matrix = rhs;
}

void Target::rotate(const MFloatMatrix &rotation)
{
	const MFloatPoint &p = this->position();
	this->setPosition(MFloatPoint::origin);
	m_matrix = m_matrix * rotation;
	this->setPosition(p);
}

MFloatVector Target::rotation(
	MTransformationMatrix::RotationOrder order,
	MAngle::Unit unit,
	const MFloatMatrix &space) const
{
	double rotValue[3];
	MTransformationMatrix tMat(MMatrix((m_matrix * space).matrix));
	tMat.reorderRotation(order);

	MTransformationMatrix::RotationOrder throwAway;
	tMat.getRotation(rotValue, throwAway);
	if (unit == MAngle::kDegrees)
	{
		rotValue[0] *= rad_to_deg;
		rotValue[1] *= rad_to_deg;
		rotValue[2] *= rad_to_deg;
	}
	return MFloatVector(rotValue[0], rotValue[1], rotValue[2]);
}

void Target::offsetBy(const MFloatVector &offset)
{
	m_matrix[3][0] = m_matrix[3][0] + offset.x;
	m_matrix[3][1] = m_matrix[3][1] + offset.y;
	m_matrix[3][2] = m_matrix[3][2] + offset.z;
}

/* This is like sliding in or out along the brush axis*/
void Target::offsetLocalZ(float dist)
{

	MFloatVector offset = MFloatVector(
							  m_matrix[2][0],
							  m_matrix[2][1],
							  m_matrix[2][2])
							  .normal() *
						  dist;

	m_matrix[3][0] = m_matrix[3][0] + offset.x;
	m_matrix[3][1] = m_matrix[3][1] + offset.y;
	m_matrix[3][2] = m_matrix[3][2] + offset.z;
}

MFloatVector Target::transform(const MFloatVector &rhs) const
{
	return rhs * m_matrix;
}

void Target::setWeight(float weight)
{
	if (weight < 0.0f)
	{
		m_weight = 0.0f;
	}
	else if (weight > 1.0f)
	{
		m_weight = 1.0f;
	}
	else
	{
		m_weight = weight;
	}
}

const float & Target::weight() const
{
	return m_weight;
}
 

void Target::setRadius(float radius)
{
	if (radius < 0.0f)
	{
		m_radius = 0.0f;
	}
	else
	{
		m_radius = radius;
	}
}

const float & Target::radius() const
{
	return m_radius;
}
 

void Target::setWait(float wait)
{
	m_wait = fmax(wait, 0.0f);
}

const float &Target::wait() const
{
	return m_wait;
}

void Target::setColor(const MColor &color)
{
	m_color = color;
}

const MColor &Target::color() const
{
	return m_color;
}

float Target::luminance() const
{
	return (m_color[0] + m_color[1] + m_color[2] + m_color[3]) * 0.25;
}

// Target::Target(
//  	const MObject &curveObject,
// 	const MFloatVector & lanceAxis, // Z
// 	const MFloatVector & majorAxis, // Y
// 	const MFloatArray &weights,
// 	// double arcLength,
// 	double param,
// 	bool follow
//  )
// 	: m_matrix(),
// 	  m_tangent(),
// 	//   m_param(param),
// 	//   m_arcLength(arcLength),
// 	  m_weight(),
// 	  m_color(0.0, 0.0, 0.0)
// {
// 	MFnNurbsCurve curveFn(curveObject);
// 	double uniformParam = curveFn.findParamFromLength(arcLength);
// 	MFloatVector tangent = MFloatVector(
// 		curveFn.tangent(uniformParam));

// 	m_tangent = tangent.normal();

// 	m_weight = mayaMath::interp(weights, float(uniformParam));

// 	MPoint pt;
// 	curveFn.getPointAtParam(
// 		uniformParam, pt, MSpace::kObject);
// 	MFloatVector mAxis = follow ? m_tangent : majorAxis;

// 	m_matrix =  mayaMath::matFromAim(
// 		MFloatVector(pt.x, pt.y, pt.z),
// 		mAxis,
// 		lanceAxis,
// 		mayaMath::yAxis,
// 		mayaMath::zAxis
// 	);å
// }

// void Target::rotate(const MFloatPoint &pivot, const MFloatMatrix &rotation)
// {
// 	m_matrix[3][0] = m_matrix[3][0] - pivot.x;
// 	m_matrix[3][1] = m_matrix[3][1] - pivot.y;
// 	m_matrix[3][2] = m_matrix[3][2] - pivot.z;

// 	m_matrix = m_matrix * rotation;

// 	m_matrix[3][0] = m_matrix[3][0] + pivot.x;
// 	m_matrix[3][1] = m_matrix[3][1] + pivot.y;
// 	m_matrix[3][2] = m_matrix[3][2] + pivot.z;

// 	// m_tangent = m_tangent * rotation;
// }

// const float &Target::param() const
// {
// 	return m_param;
// }

// void Target::reverseParam()
// {
// 	m_param = 1.0 - m_param;
// }

// const float &Target::arcLength() const
// {
// 	return m_arcLength;
// }

// void Target::setUV(
// 	const MFloatMatrix &inversePlaneMatrix)
// {
// 	MFloatPoint p = ((this->position() * inversePlaneMatrix) * 0.5) + MFloatVector(0.5f, 0.5f, 0.0f);
// 	m_u = fmax(fmin(p.x, 1.0f), 0.0f);
// 	m_v = fmax(fmin(p.y, 1.0f), 0.0f);
// }

// void Target::appendUVsTo(MFloatArray &uVals, MFloatArray &vVals) const
// {
// 	uVals.append(m_u);
// 	vVals.append(m_v);
// }

// Target::Target(
// 	const MFnNurbsCurve &curveFn,
// 	double dist,
// 	double startDist,
// 	double strokeRange,
// 	double curveLength) : m_weight(1.0),
// 						  m_u(0.0f),
// 						  m_v(0.0f)
// {
// 	m_param = (dist - startDist) / strokeRange;
// 	m_curveParam = dist / curveLength;
// 	double prm = curveFn.findParamFromLength(dist);

// 	m_tangent = curveFn.tangent(prm);
// 	m_tangent.z = 0;
// 	m_tangent.normalize();

// 	MPoint pt;
// 	curveFn.getPointAtParam(prm, pt, MSpace::kObject);
// 	MMatrix mat;
// 	mat[3][0] = pt.x;
// 	mat[3][1] = pt.y;
// 	mat[3][2] = pt.z;

// 	m_matrix = mat;
// }

// void Target::applyRotation(
// 		float tilt,
// 		float bank,
// 		float twist,
// 		mayaMath::axis eFrontAxis,
// 		mayaMath::axis eUpAxis,
// 		bool follow,
// 		bool backstroke)
// {

// 	// apply a relative tilt around the axis which is the cross
// 	// of the lance (localZ) and the tangent.

// 	// apply a relative bank around the tangent.

// 	// apply a relative twist around the lance (local Z)

// 		//FRONT  mayaMath::yAxis, NOT EXACTLY SURE YET WHY
// 		//UP  mayaMath::zAxisNeg); BECAUSE Z points down into the painting.

// 	if (backstroke)
// 	{
// 		tilt = -tilt;
// 	}
// 	MFloatVector pos(
// 		m_matrix[3][0],
// 		m_matrix[3][1],
// 		m_matrix[3][2]);

// 	MFloatVector

// 	if (follow)
// 	{
// 		MFloatMatrix mat = mayaMath::matFromAim(
// 			pos,
// 			m_tangent,
// 			MFloatVector::zAxis,
// 			eFrontAxis,
// 			eUpAxis);

// 		MTransformationMatrix tmat = MTransformationMatrix::identity;
// 		double3 rot;
// 		rot[0] = tilt;
// 		rot[1] = bank;
// 		rot[2] = twist;
// 		tmat.setRotation(rot, MTransformationMatrix::kZYX);
// 		m_matrix = tmat.asMatrix() * mat;
// 	}
// 	else
// 	{

// 		MMatrix mat = mayaMath::matFromAim(
// 			MVector::zero,
// 			MVector::xNegAxis,
// 			MVector::zAxis,
// 			eFrontAxis,
// 			eUpAxis);
// 		MVector side((MVector::zAxis ^ m_tangent).normal());

// 		MMatrix twistMat = MQuaternion(twist, MVector::zAxis).asMatrix();
// 		MMatrix bankMat = MQuaternion(bank, m_tangent).asMatrix();
// 		MMatrix tiltMat = MQuaternion(tilt, side).asMatrix();

// 		mat = mat * twistMat * bankMat * tiltMat;
// 		mat[3][0] = pos.x;
// 		mat[3][1] = pos.y;
// 		mat[3][2] = pos.z;
// 		m_matrix = mat;
// 	}
// }

// void Target::applyGlobalTilt(const MFloatVector &gradient)
// {
// 	float mag = gradient.length();
// 	if (mag < 0.000001)
// 	{
// 		return;
// 	}
// 	MFloatVector axis = (gradient ^ MFloatVector::zAxis).normal();

// 	MMatrix rotMat = MQuaternion(mag, axis).asMatrix();

// 	MMatrix centerMat = MMatrix::identity;
// 	centerMat[3][0] = -m_matrix[3][0];
// 	centerMat[3][1] = -m_matrix[3][1];

// 	m_matrix = m_matrix * centerMat * rotMat * centerMat.inverse();
// }

// void Target::applyGlobalAim(const MPoint &point)
// {

// 	MVector z1 = MVector(
// 					 MPoint(
// 						 m_matrix[3][0],
// 						 m_matrix[3][1],
// 						 m_matrix[3][2]) - point
// 					 ).normal();

// 	MVector lance(
// 	m_matrix[2][0],
// 	m_matrix[2][1],
// 	m_matrix[2][2]) // zaxis

// 	MMatrix rotMat = MQuaternion(lance, z1).asMatrix();

// 	MMatrix centerMat = MMatrix::identity;
// 	centerMat[3][0] = -m_matrix[3][0];
// 	centerMat[3][1] = -m_matrix[3][1];
// 	centerMat[3][2] = -m_matrix[3][2];

// 	m_matrix = m_matrix * centerMat * rotMat * centerMat.inverse();
// }

// void Target::getBorderPoints(
// 	MFloatPoint &left,
// 	MFloatPoint &right,
// 	float width,
// 	bool flatBrush,
// 	bool displayWeightWidth) const
// {

// 	double weight = m_weight;

// 	if (flatBrush || (!displayWeightWidth))
// 	{
// 		weight = 1.0;
// 	}
// 	MPoint p = position();
// 	MVector xOffset = MVector::yAxis * m_matrix  * (width * weight);;
// 	// if (flatBrush)
// 	// {
// 	// 	side = MVector::yAxis * m_matrix;
// 	// 	xOffset = (((MVector::xAxis * m_matrix) ^ MVector::zAxis) ^
// 	// 			   MVector::zAxis)
// 	// 				  .normal();
// 	// }
// 	// else
// 	// {
// 	// 	side = MVector::yAxis * m_matrix;

// 	// 	xOffset = (m_tangent ^ MVector::zAxis).normal();
// 	// }
// 	// xOffset = (MVector::yAxis * m_matrix);

// 	// xOffset *= (width * weight);
// 	left = p + xOffset;
// 	right = p - xOffset;
// }