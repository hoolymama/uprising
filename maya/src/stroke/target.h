/**
 * @file target.h
 * @brief Defines the Target class for representing discrete points along a Stroke.
 * 
 * This file contains the Target class which represents a discrete point along a Stroke.
 * Each Target contains position, orientation, weight, color, and other properties
 * needed for stroke rendering and robot movement planning. Targets can be manipulated
 * with various transformations and provide methods for accessing their properties.
 */

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
 * It includes a transformation matrix defining its position and orientation, as well as
 * additional properties like weight, radius, color, and wait time. Targets can be manipulated
 * with various transformations and provide methods for accessing their properties.
 */
class Target
{
public:
/**
 * @brief Default Constructor.
 *
 * Creates a Target and sets its matrix to the identity.
 * This target's tangent will be the zero vector, so you'll need to set it.
 * Other members are given sensible defaults.
 */
	Target();

/**
 * @brief Construct a new Target object with matrix, tangent, and weight.
 *
 * @param mat The transformation matrix for the target.
 * @param tangent The tangent vector at this target.
 * @param weight The weight value for this target (default: 1.0).
 */
	Target(
		 const MFloatMatrix &mat,
		 const MFloatVector &tangent,
		 float weight = 1.0);

/**
 * @brief Construct a new Target object with matrix, tangent, weight, and color.
 *
 * @param mat The transformation matrix for the target.
 * @param tangent The tangent vector at this target.
 * @param weight The weight value for this target.
 * @param color The color for this target.
 */
	Target(
		 const MFloatMatrix &mat,
		 const MFloatVector &tangent,
		 float weight,
		 const MColor & color);

/**
 * @brief Construct a new Target object with matrix and weight.
 *
 * @param mat The transformation matrix for the target.
 * @param weight The weight value for this target (default: 1.0).
 */
	Target(
		 const MFloatMatrix &mat,
		 float weight = 1.0);
	
/**
 * @brief Construct a new Target object with matrix, weight, and color.
 *
 * @param mat The transformation matrix for the target.
 * @param weight The weight value for this target.
 * @param color The color for this target.
 */
	Target(
		const MFloatMatrix &mat,
		float weight,
		const MColor &color);


/**
 * @brief Construct a new Target object from a Nurbs curve.
 *
 * @param curveObject An object of type MFn::kNurbsCurve
 * @param lanceAxis The TCP forward axis (Z) and is used to construct the matrix.
 * @param majorAxis The TCP brush wide axis, used for matrix construction. Not used
 * if the stroke is in follow mode.
 * @param weights All the weights for the stroke. We use the param to
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

/**
 * @brief Destructor.
 */
	~Target();

/**
 * @brief Gets the X axis of the target's orientation.
 * @return The X axis vector.
 */
	MFloatVector xAxis() const;

/**
 * @brief Gets the Y axis of the target's orientation.
 * @return The Y axis vector.
 */
	MFloatVector yAxis() const;

/**
 * @brief Gets the Z axis of the target's orientation.
 * @return The Z axis vector.
 */
	MFloatVector zAxis() const;

/**
 * @brief Applies a tilt rotation to the target.
 * @param angle The tilt angle in radians.
 */
	void applyTilt(float angle);

/**
 * @brief Applies a bank rotation to the target.
 * @param angle The bank angle in radians.
 */
	void applyBank(float angle);

/**
 * @brief Applies a twist rotation to the target.
 * @param angle The twist angle in radians.
 */
	void applyTwist(float angle);

/**
 * @brief Applies tilt, bank, and twist rotations to the target.
 * @param tilt The tilt angle in radians.
 * @param bank The bank angle in radians.
 * @param twist The twist angle in radians.
 * @param order The rotation order to apply the rotations.
 */
	void applyTiltBankTwist(float tilt, float bank, float twist, PaintingEnums::BrushRotateOrder order);
	
/**
 * @brief Applies an axis-angle rotation to the target.
 * @param axis The axis of rotation.
 * @param angle The angle of rotation in radians.
 */
	void applyAxisAngleRotation(
		 const MFloatVector &axis,
		 float angle);

/**
 * @brief Sets the draw tangent for this target.
 * @param tangent The tangent vector to set.
 */
	void setDrawTangent(const MFloatVector &tangent);

/**
 * @brief Gets the transformation matrix of this target.
 * @return The transformation matrix.
 */
	const MFloatMatrix &matrix() const;

/**
 * @brief Gets the draw tangent of this target.
 * @return The draw tangent vector.
 */
	const MFloatVector &drawTangent() const;

/**
 * @brief Offsets the target's position by a vector.
 * @param offset The offset vector to apply.
 */
	void offsetBy(const MFloatVector &offset);

/**
 * @brief Offsets the target along its local Z axis.
 * @param dist The distance to offset.
 */
	void offsetLocalZ(float dist);

/**
 * @brief Calculates the distance to another target.
 * @param other The other target to calculate distance to.
 * @return The distance between this target and the other target.
 */
	float distanceTo(const Target &other) const;

/**
 * @brief Creates a view matrix for this target.
 * @param viewNormal The normal vector for the view.
 * @return The view matrix.
 */
	MFloatMatrix viewMatrix(const MFloatVector &viewNormal) const;

/**
 * @brief Gets the position of this target in a specific space.
 * @param space The transformation matrix defining the space.
 * @return The position in the specified space.
 */
	MFloatPoint position(
		 const MFloatMatrix &space) const;

/**
 * @brief Gets the position of this target in world space.
 * @return The position in world space.
 */
	MFloatPoint position() const;

/**
 * @brief Sets the position of this target.
 * @param rhs The new position.
 */
	void setPosition(const MFloatPoint &rhs);

/**
 * @brief Sets the transformation matrix of this target.
 * @param rhs The new transformation matrix.
 */
	void setMatrix(const MFloatMatrix &rhs);

/**
 * @brief Rotates this target around a pivot point.
 * @param pivot The pivot point for rotation.
 * @param rotation The rotation matrix.
 */
	void rotate(const MFloatPoint &pivot, const MFloatMatrix &rotation);

/**
 * @brief Rotates this target in place.
 * @param rotation The rotation matrix.
 */
	void rotate(const MFloatMatrix &rotation);

/**
 * @brief Gets the rotation of this target in a specific space.
 * @param order The rotation order.
 * @param unit The angle unit.
 * @param space The transformation matrix defining the space (default: identity).
 * @return The rotation as a vector of Euler angles.
 */
	MFloatVector rotation(
		 MTransformationMatrix::RotationOrder order,
		 MAngle::Unit unit,
		 const MFloatMatrix &space = MFloatMatrix()) const;

/**
 * @brief Transforms a vector by this target's matrix.
 * @param rhs The vector to transform.
 * @return The transformed vector.
 */
	MFloatVector transform(const MFloatVector &rhs) const;

/**
 * @brief Gets the border points of this target on a plane.
 * @param planeNormal The normal vector of the plane.
 * @param left Output parameter for the left border point.
 * @param right Output parameter for the right border point.
 * @param width The width to use for the border.
 * @param flatBrush Whether the brush is flat.
 * @param displayWeightWidth Whether to scale the width by the target's weight (default: true).
 */
	void getBorderPoints(
		 const MFloatVector &planeNormal,
		 MFloatPoint &left,
		 MFloatPoint &right,
		 float width,
		 bool flatBrush,
		 bool displayWeightWidth = true) const;

/**
 * @brief Sets the weight of this target.
 * @param weight The new weight value.
 */
	void setWeight(float weight);

/**
 * @brief Gets the weight of this target.
 * @return The weight value.
 */
	const float &weight() const;
	
/**
 * @brief Sets the radius of this target.
 * @param radius The new radius value.
 */
	void setRadius(float radius);

/**
 * @brief Gets the radius of this target.
 * @return The radius value.
 */
	const float &radius() const;

/**
 * @brief Sets the wait time of this target.
 * @param wait The new wait time value.
 */
	void setWait(float wait);

/**
 * @brief Gets the wait time of this target.
 * @return The wait time value.
 */
	const float &wait() const;

/**
 * @brief Sets the color of this target.
 * @param rgba The new color value.
 */
	void setColor(const MColor &rgba);

/**
 * @brief Gets the color of this target.
 * @return The color value.
 */
	const MColor &color() const;

/**
 * @brief Calculates the luminance of this target's color.
 * @return The luminance value.
 */
 	float luminance() const;

private:
	MFloatVector m_drawTangent; /**< Tangent vector used for drawing. */
	MFloatMatrix m_matrix;      /**< Transformation matrix defining position and orientation. */
	float m_weight;	            /**< Weight value indicating fraction of brush tip. */
	float m_radius;	            /**< Radius value for paint brushes. */
	MColor m_color;	            /**< RGBA color value. */
	float m_wait;               /**< Wait time value. */
};

#endif

