
#ifndef _Target_
#define _Target_

#include <vector>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MFnNurbsCurve.h>

#include <maya/MPoint.h>

#include <maya/MObject.h>
#include <mayaMath.h>

/**
 * @brief A discreet point along a Stroke.
 *
 * A Target is an object that contains information to send a linear or joint move to the robot.
 *
 *
 */
class Target
{
public:
/**
 * Default Constructor.
 *
 * Creates a Target and sets its matrix to the identity.
 *
 * This target's tangent will be the zero vector, so you'l;l need to set it.
 *
 * Other members are given sensible defaults.
 *
 */
	Target();

/**
 * @brief Construct a new Target object.
 *
 * @param mat
 * @param tangent
 * @param param
 * @param arclength
 * @param weight
 */
	Target(
		 const MFloatMatrix &mat,
		 const MFloatVector &tangent,
		 float param,
		 float arclength,
		 float weight = 1.0);

/**
 * @brief Construct a new Target object from a Nurbs curve.
 *
 *
 * @param curveObject An object of type MFn::kNurbsCurve
 * @param lanceAxis The TCP forward axis (Z) and is used to construct the
 * matrix.
 * @param majorAxis The TCP brush wide axis, used for matrix construction. Not used
 * if the stroke is in follow mode.
 * @param weights All the weights. for the stroke. We use the param to
 * interpolate the correct weight.
 * @param arcLength The arc length of this target along the stroke.
 * @param param A parameter value to associate with the target.
 * @param follow This target's major axis should face the stroke direction and
 * turn with it.
 */
	Target(
		 const MObject &curveObject,
		 const MFloatVector &lanceAxis, // Z
		 const MFloatVector &majorAxis, // Y
		 const MFloatArray &weights,
		 double arcLength,
		 double param,
		 bool follow);

	~Target();

 




	void applyTilt(float angle);
	void applyBank(float angle);
	void applyTwist(float angle);
	void applyAxisAngleRotation(
		 const MFloatVector &axis,
		 float angle);

	// void setRotation(
	// 	float tilt,
	// 	float bank,
	// 	float twist,
	// 	mayaMath::axis frontAxis,
	// 	mayaMath::axis upAxis,
	// 	bool follow=false,
	// 	bool backstroke=false);

	// void setRotation(bool follow, bool backstroke);

	void setTangent(const MFloatVector &tangent);

	const MFloatMatrix &matrix() const;

	const MFloatVector &tangent() const;

	// void reverseParam();

	const float &param() const;

	const float &arcLength() const;

	void offsetBy(const MFloatVector &offset);

	void offsetLocalZ(float dist);

	float distanceTo(const Target &other) const;

	MFloatMatrix viewMatrix(const MFloatVector &planeNormal, bool backstroke = false) const;

	MFloatPoint position(
		 const MFloatMatrix &space) const;

	MFloatPoint position() const;

	void setPosition(const MFloatPoint &rhs);

	void setMatrix(const MFloatMatrix &rhs);

	void rotate(const MFloatPoint &pivot, const MFloatMatrix &rotation);
	void rotate(const MFloatMatrix &rotation);

//sdf

	MFloatVector rotation(
		 MTransformationMatrix::RotationOrder order,
		 MAngle::Unit unit,
		 const MFloatMatrix &space = MFloatMatrix()) const;

	MFloatVector transform(const MFloatVector &rhs) const;

	void getBorderPoints(
		 const MFloatVector &planeNormal,
		 MFloatPoint &left,
		 MFloatPoint &right,
		 float width,
		 bool flatBrush,
		 bool displayWeightWidth = true) const;

	void setWeight(float contact);

	const float &weight() const;

	// void setUV(const MFloatMatrix &inversePlaneMatrix);

	// void appendUVsTo(MFloatArray &uVals, MFloatArray &vVals) const;

	// void applyGlobalTilt(const MFloatVector &gradient);

	// void applyGlobalAim(const MPoint &point);

	void setColor(const MColor &rgba);

	const MColor &color() const;

private:
	MFloatVector m_tangent; // tangent of curve that joins targets
	MFloatMatrix m_matrix;
	float m_param;		 // A parameter value for 1D  mapping purposes
	float m_arcLength; // length from start
	float m_weight;	 // used to indicate fraction of brush tip
	MColor m_color;	 // RGBA
							 // double m_tilt;		 // store these when setting rotation, just in case we have to recalc
							 // double m_bank;		 // store these when setting rotation, just in case we have to recalc
							 // double m_twist;		 // store these when setting rotation, just in case we have to recalc
};

#endif
