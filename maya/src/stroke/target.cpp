#include <target.h>

#include <maya/MFnNurbsCurve.h>


const double rad_to_deg = (180 / 3.1415927);

Target::Target() : m_matrix(),
				   m_tangent(),
				//    m_param(0.0f),
				//    m_arcLength(0.0f),
				   m_weight(1.0f),
				   m_color(0.0, 0.0, 0.0)
{
}

Target::Target(
	const MFloatMatrix &mat,
	const MFloatVector &tangent,
	// float param,
	// float arcLength,
	float weight)
	: m_matrix(mat),
	  m_tangent(tangent),
	//   m_param(param),
	//   m_arcLength(arcLength),
	  m_weight(weight),
	  m_color(0.0, 0.0, 0.0)
{

}


Target::Target(
	const MFloatMatrix &mat,
	const MFloatVector &tangent,
	float weight,
	const MColor & color)
	: m_matrix(mat),
	  m_tangent(tangent),
	  m_weight(weight),
	  m_color(color)
{}
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
// 	);
// }
 
Target::~Target() {}

void Target::setTangent(const MFloatVector &tangent)
{
	m_tangent = tangent;
}

void Target::applyAxisAngleRotation(const MFloatVector &axis , float angle)
{
	MFloatMatrix rotMat = MFloatMatrix(
		MQuaternion(angle, axis ).asMatrix().matrix);
	MFloatMatrix centerMat;
	centerMat[3][0] = m_matrix[3][0];
	centerMat[3][1] = m_matrix[3][1];
	centerMat[3][2] = m_matrix[3][2];

	m_matrix = m_matrix * centerMat.inverse() * rotMat * centerMat;

}


void Target::applyTilt(float angle)
{
	// apply a relative tilt around the axis which is the cross
	// of the lance (localZ) and the tangent.
	MFloatVector lance = MFloatVector::zAxis * m_matrix;
	MFloatVector axis = (lance ^ m_tangent).normal();
	applyAxisAngleRotation(axis, angle);
}

void Target::applyBank(float angle)
{
	// apply a relative bank around the tangent.
	applyAxisAngleRotation(m_tangent, angle);
}

void Target::applyTwist(float angle)
{
	// apply a relative twist around the lance (local Z)
	MFloatVector lance = MFloatVector::zAxis * m_matrix;
	applyAxisAngleRotation(lance, angle);
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
MFloatMatrix Target::viewMatrix(const MFloatVector& planeNormal, bool backstroke) const
{
	// we will draw arrows
	MFloatVector tangent = backstroke ? -m_tangent : m_tangent;
	// MFloatVector lance = MFloatVector::zAxis * m_matrix;
	MFloatVector side = (planeNormal ^ tangent).normal();
	tangent = (side^planeNormal).normal();
	

	MFloatMatrix res = m_matrix;
	res[0][0] = tangent.x;
	res[0][1] = tangent.y;
	res[0][2] = tangent.z;
	res[0][3] = 0.0;
	res[1][0] = side.x;
	res[1][1] = side.y;
	res[1][2] = side.z;
	res[1][3] = 0.0;
	res[2][0] = planeNormal.x;
	res[2][1] = planeNormal.y;
	res[2][2] = planeNormal.z;
	res[2][3] = 0.0;

	return res;
}

const MFloatMatrix &Target::matrix() const
{
	return m_matrix;
}

const MFloatVector &Target::tangent() const
{
	return m_tangent;
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


void Target::rotate(const MFloatPoint &pivot, const MFloatMatrix &rotation)
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

MFloatVector Target::rotation(
	MTransformationMatrix::RotationOrder order,
	MAngle::Unit unit,
	const MFloatMatrix &space) const
{
	double rotValue[3];
	MTransformationMatrix tMat( MMatrix((m_matrix * space).matrix) );
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
		m_matrix[2][0], m_matrix[2][1],
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

const float &Target::weight() const
{
	return m_weight;
}

void Target::getBorderPoints(
	const MFloatVector & planeNormal,
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

	MVector xOffset = (planeNormal^m_tangent).normal() * m_matrix  * (width * weight);;
	left = p + xOffset;
	right = p - xOffset;
}

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
	return (m_color[0] + m_color[1]+ m_color[2]+ m_color[3]) *0.25;
}

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