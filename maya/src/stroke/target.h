
#ifndef _Target_
#define _Target_

#include <vector>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MFnNurbsCurve.h>

#include <maya/MPoint.h>

#include <maya/MObject.h>
#include <mayaMath.h>
#include "enums.h"

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
 * This target's tangent will be the zero vector, so you'll need to set it.
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
 * @param weight
 */
	Target(
		 const MFloatMatrix &mat,
		 const MFloatVector &tangent,
		 float weight = 1.0);

	Target(
		 const MFloatMatrix &mat,
		 const MFloatVector &tangent,
		 float weight,
		 const MColor & color);

	Target(
		 const MFloatMatrix &mat,
		 float weight = 1.0);
	
	Target(
		const MFloatMatrix &mat,
		float weight,
		const MColor &color);


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
 * @param follow This target's major axis should face the stroke direction and
 * turn with it.
 */
	Target(
		 const MObject &curveObject,
		 const MFloatVector &lanceAxis, // Z
		 const MFloatVector &majorAxis, // Y
		 const MFloatArray &weights,
		 bool follow);

	~Target();

 

	MFloatVector xAxis() const;
	MFloatVector yAxis() const;
	MFloatVector zAxis() const;


	void applyTilt(float angle);
	void applyBank(float angle);
	void applyTwist(float angle);
	void applyTiltBankTwist(float tilt, float bank, float twist, PaintingEnums::BrushRotateOrder order);
	
	void applyAxisAngleRotation(
		 const MFloatVector &axis,
		 float angle);

	void setDrawTangent(const MFloatVector &tangent);

	const MFloatMatrix &matrix() const;

	const MFloatVector &drawTangent() const;


	void offsetBy(const MFloatVector &offset);

	void offsetLocalZ(float dist);

	float distanceTo(const Target &other) const;

	MFloatMatrix viewMatrix(const MFloatVector &viewNormal) const;

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

	void setWeight(float weight);
	const float &weight() const;
	
	void setRadius(float radius);
	const float &radius() const;
	


	void setWait(float wait);
	const float &wait() const;
 

	void setColor(const MColor &rgba);

	const MColor &color() const;

 	float luminance() const;


private:

	MFloatVector m_drawTangent; // tangent to use for drawing.
	MFloatMatrix m_matrix;
	float m_weight;	 // used to indicate fraction of brush tip
	float m_radius;	 // used to indicate fraction of brush tip


	MColor m_color;	 // RGBA
	float m_wait;


};

#endif

