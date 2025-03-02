/**
 * @file stroke.h
 * @brief Defines the Stroke class for representing stroke geometry in Maya.
 * 
 * This file contains the Stroke class which represents a sequence of Target objects
 * forming a stroke. It provides comprehensive functionality for creating, manipulating,
 * and querying strokes, including methods for transformation, interpolation, sorting,
 * filtering, and visualization. Strokes are the core data structure used throughout
 * the stroke-related nodes and commands in Maya.
 */

#ifndef _stroke_
#define _stroke_

#include <vector>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MColorArray.h>

#include <maya/MFnMesh.h>

#include <maya/MObject.h>

#include <mayaMath.h>
#include <attrUtils.h>

// #include "strokeRotationSpec.h"
#include "brushStrokeSpec.h"

#include "brush.h"
#include "paint.h"
#include "target.h"
#include "enums.h"

/**
 * @class Stroke
 * @brief Represents a sequence of Target objects forming a stroke.
 * 
 * The Stroke class encapsulates a sequence of Target objects that form a stroke.
 * It provides comprehensive functionality for creating, manipulating, and querying
 * strokes, including methods for transformation, interpolation, sorting, filtering,
 * and visualization. Strokes can have various properties such as IDs, coil values,
 * and brush/paint specifications.
 */
class Stroke
{
public:
	/// @brief Iterator types for targets. This facilitates looping over targets.
	using target_iterator = typename std::vector<Target>::iterator;
	using const_target_iterator = typename std::vector<Target>::const_iterator;

	target_iterator targets_begin() { return m_targets.begin(); }
	target_iterator targets_end() { return m_targets.end(); }
	const_target_iterator targets_begin() const { return m_targets.begin(); }
	const_target_iterator targets_end() const { return m_targets.end(); }

	target_iterator arrivals_begin() { return m_arrivals.begin(); }
	target_iterator arrivals_end() { return m_arrivals.end(); }
	const_target_iterator arrivals_begin() const { return m_arrivals.begin(); }
	const_target_iterator arrivals_end() const { return m_arrivals.end(); }

	/**
	 * @enum DirectionMethod
	 * @brief Defines methods for determining stroke direction.
	 */
	enum DirectionMethod
	{
		kForwards,       /**< Use the forward direction. */
		kBackwards,      /**< Use the backward direction. */
		kStartUppermost, /**< Start from the uppermost point. */
		kEndUppermost,   /**< End at the uppermost point. */
		kRadialIn,       /**< Direction points radially inward. */
		kRadialOut       /**< Direction points radially outward. */
	};

	/**
	 * @enum FilterOperator
	 * @brief Defines operators for filtering strokes.
	 */
	enum FilterOperator
	{
		kGreaterThan, /**< Greater than comparison. */
		kLessThan,    /**< Less than comparison. */
		kEqualTo,     /**< Equal to comparison. */
		kNotEqualTo,  /**< Not equal to comparison. */
		kNoOp         /**< No operation. */
	};

	/**
	 * @enum SortFilterKey
	 * @brief Defines keys for sorting and filtering strokes.
	 */
	enum SortFilterKey
	{
		kStrokeId,     /**< Sort/filter by stroke ID. */
		kBrushId,      /**< Sort/filter by brush ID. */
		kPaintId,      /**< Sort/filter by paint ID. */
		kRepeatId,     /**< Sort/filter by repeat ID. */
		kLayerId,      /**< Sort/filter by layer ID. */
		kParentId,     /**< Sort/filter by parent ID. */
		kTargetCount,  /**< Sort/filter by target count. */
		kMapRed,       /**< Sort/filter by red map value. */
		kMapGreen,     /**< Sort/filter by green map value. */
		kMapBlue,      /**< Sort/filter by blue map value. */
		kBrushModelId, /**< Sort/filter by brush model ID. */
		kBrushShape,   /**< Sort/filter by brush shape. */
		kSegmentId     /**< Sort/filter by segment ID. */
	};

	/**
	 * @enum SortDirection
	 * @brief Defines directions for sorting strokes.
	 */
	enum SortDirection
	{
		kSortAscending,  /**< Sort in ascending order. */
		kSortDescending  /**< Sort in descending order. */
	};

	/**
	 * @brief Default constructor.
	 * 
	 * Creates an empty stroke with default properties.
	 */
	Stroke();

	/**
	 * @brief Construct a new Stroke object with points and rotation matrix.
	 * 
	 * Creates a stroke from an array of points and an initial rotation matrix.
	 * All targets will have this matrix initially, and view tangents will be
	 * calculated from the points.
	 *
	 * @param points An array of points defining the stroke path.
	 * @param rotationMat A matrix defining the initial orientation (expected to be unit scale).
	 */
	Stroke(const MFloatPointArray &points,
		   const MFloatMatrix &rotationMat);

	/**
	 * @brief Construct a new Stroke object with matrices.
	 * 
	 * Creates a stroke from an array of transformation matrices.
	 *
	 * @param matrices An array of matrices defining the stroke targets.
	 */
	Stroke(const std::vector<MFloatMatrix> &matrices);

	/**
	 * @brief Construct a new Stroke from a subsection of another stroke.
	 * 
	 * Creates a new stroke containing a subset of targets from an existing stroke.
	 *
	 * @param instroke The source stroke to create a new stroke from.
	 * @param start The target index to start at.
	 * @param count The number of targets to include.
	 */
	Stroke(const Stroke &instroke,
		   unsigned start,
		   unsigned count);

	/**
	 * @brief Destructor.
	 */
	~Stroke();

	/**
	 * @brief Sets the creator information for this stroke.
	 * @param creatorName The name of the creator.
	 * @param creatorId The ID of the creator.
	 */
	void setCreator(const MString &creatorName, int creatorId);

	/**
	 * @brief Gets the creator name of this stroke.
	 * @return The creator name.
	 */
	const MString &creatorName() const;

	/**
	 * @brief Gets the creator ID of this stroke.
	 * @return The creator ID.
	 */
	int creatorId() const;

	/**
	 * @brief Gets the dither probability of this stroke.
	 * @return The dither probability value.
	 */
	float ditherProbability() const;

	/**
	 * @brief Sets the dither probability of this stroke.
	 * @param probability The new dither probability value.
	 */
	void setDitherProbability(float probability);

	/**
	 * @brief Sets the stroke ID of this stroke.
	 * @param rhs The new stroke ID.
	 */
	void setStrokeId(unsigned rhs);

	/**
	 * @brief Resets the tangents of all targets in this stroke.
	 */
	void resetTangents();

	/**
	 * @brief Calculates tangents from points.
	 * @param points Array of points to calculate tangents from.
	 * @param tangents Output array to store the calculated tangents.
	 */
	void calculateTangents(
		const MFloatPointArray &points,
		MFloatVectorArray &tangents) const;

	/**
	 * @brief Calculates tangents from matrices.
	 * @param matrices Array of matrices to calculate tangents from.
	 * @param tangents Output array to store the calculated tangents.
	 */
	void calculateTangents(
		const std::vector<MFloatMatrix> &matrices,
		MFloatVectorArray &tangents) const;

	/**
	 * @brief Sets the coil value of this stroke.
	 * @param rhs The new coil value.
	 */
	void setCoil(float rhs);

	/**
	 * @brief Gets the coil value of this stroke.
	 * @return The coil value.
	 */
	float coil() const;

	/**
	 * @brief Gets the number of valid targets in this stroke.
	 * @return The number of valid targets.
	 */
	unsigned valid() const;

	/**
	 * @brief Gets the total size of this stroke.
	 * @param withTraversal Whether to include traversal targets (default: false).
	 * @return The total number of targets.
	 */
	unsigned size(bool withTraversal = false) const;

	/**
	 * @brief Gets the targets in this stroke.
	 * @return Constant reference to the vector of targets.
	 */
	const std::vector<Target> &targets() const;

	/**
	 * @brief Calculates the arc length of this stroke.
	 * @return The arc length.
	 */
	float calculateArcLength() const;

	/**
	 * @brief Calculates parameter values along the stroke.
	 * @param result Output array to store the calculated parameters.
	 */
	void calculateParams(MFloatArray &result) const;

	/**
	 * @brief Gets a point at a specific parameter value along the stroke.
	 * @param param Parameter value (0.0 to 1.0).
	 * @param result Output parameter to store the point.
	 */
	void getPointAtParam(float param, MFloatPoint &result) const;

	/**
	 * @brief Sets the brush stroke specification for this stroke.
	 * @param rhs The new brush stroke specification.
	 */
	void setBrushStrokeSpec(const BrushStrokeSpec &rhs);

	/**
	 * @brief Gets the brush stroke specification of this stroke.
	 * @return The brush stroke specification.
	 */
	const BrushStrokeSpec &brushStrokeSpec() const;

	/**
	 * @brief Gets the paint flow value of this stroke.
	 * @return The paint flow value.
	 */
	const float &paintFlow() const;

	/**
	 * @brief Gets the pivot target of this stroke.
	 * @return The pivot target.
	 */
	const Target &pivot() const;

	/**
	 * @brief Sets the pivot position of this stroke.
	 * @param rhs The new pivot position.
	 */
	void setPivotPosition(const MFloatPoint &rhs);

	/**
	 * @brief Sets the pivot matrix of this stroke.
	 * @param rhs The new pivot matrix.
	 */
	void setPivotMatrix(const MFloatMatrix &rhs);

	/**
	 * @brief Gets the stroke ID of this stroke.
	 * @return The stroke ID.
	 */
	int strokeId() const;

	/**
	 * @brief Gets the segment ID of this stroke.
	 * @return The segment ID.
	 */
	int segmentId() const;

	/**
	 * @brief Gets the parent ID of this stroke.
	 * @return The parent ID.
	 */
	int parentId() const;

	/**
	 * @brief Gets the repeat ID of this stroke.
	 * @return The repeat ID.
	 */
	int repeatId() const;

	/**
	 * @brief Gets the maximum radius of this stroke.
	 * @return The maximum radius.
	 */
	float maxRadius() const;

	/**
	 * @brief Sets the parent ID of this stroke.
	 * @param parentId The new parent ID.
	 */
	void setParentId(int parentId);

	/**
	 * @brief Sets the segment ID of this stroke.
	 * @param segmentId The new segment ID.
	 */
	void setSegmentId(int segmentId);

	/**
	 * @brief Sets the repeat ID of this stroke.
	 * @param rhs The new repeat ID.
	 */
	void setRepeatId(int rhs);

	/**
	 * @brief Sets the maximum radius of this stroke.
	 * @param rhs The new maximum radius.
	 */
	void setMaxRadius(float rhs);

	/**
	 * @brief Gets the sort stack of this stroke.
	 * @return The sort stack.
	 */
	const MIntArray &sortStack() const;

	/**
	 * @brief Clears the sort stack of this stroke.
	 */
	void clearSortStack();

	/**
	 * @brief Sets the layer ID of this stroke.
	 * @param rhs The new layer ID.
	 */
	void setLayerId(int rhs);

	/**
	 * @brief Gets the layer ID of this stroke.
	 * @return The layer ID.
	 */
	int layerId() const;

	/**
	 * @brief Gets the brush ID of this stroke.
	 * @return The brush ID.
	 */
	int brushId() const;

	/**
	 * @brief Gets the paint ID of this stroke.
	 * @return The paint ID.
	 */
	int paintId() const;

	/**
	 * @brief Sets the brush ID of this stroke.
	 * @param val The new brush ID.
	 */
	void setBrushId(int val);

	/**
	 * @brief Sets the brush model ID of this stroke.
	 * @param rhs The new brush model ID.
	 */
	void setBrushModelId(int rhs);

	/**
	 * @brief Gets the brush model ID of this stroke.
	 * @return The brush model ID.
	 */
	int brushModelId() const;

	/**
	 * @brief Sets the paint ID of this stroke.
	 * @param val The new paint ID.
	 */
	void setPaintId(int val);

	/**
	 * @brief Sets the sort color of this stroke.
	 * @param color The new sort color.
	 */
	void setSortColor(const MFloatVector &color);

	/**
	 * @brief Sets the filter color of this stroke.
	 * @param color The new filter color.
	 */
	void setFilterColor(const MFloatVector &color);

	// Sort stack methods
	void appendStrokeIdToSortStack(bool ascending);
	void appendSegmentIdToSortStack(bool ascending);
	void appendParentIdToSortStack(bool ascending);
	void appendBrushIdToSortStack(bool ascending);
	void appendPaintIdToSortStack(bool ascending);
	void appendLayerIdToSortStack(bool ascending);
	void appendRepeatIdToSortStack(bool ascending);
	void appendTargetCountToSortStack(bool ascending);
	void appendBrushModelIdToSortStack(bool ascending);
	void appendBrushShapeToSortStack(bool ascending);
	void appendMapRedIdToSortStack(bool ascending);
	void appendMapGreenIdToSortStack(bool ascending);
	void appendMapBlueIdToSortStack(bool ascending);

	// Filter test methods
	bool testAgainstValue(int lhs, FilterOperator op, int rhs) const;
	bool testStrokeId(FilterOperator op, int value) const;
	bool testSegmentId(FilterOperator op, int value) const;
	bool testParentId(FilterOperator op, int value) const;
	bool testBrushId(FilterOperator op, int value) const;
	bool testPaintId(FilterOperator op, int value) const;
	bool testLayerId(FilterOperator op, int value) const;
	bool testRepeatId(FilterOperator op, int value) const;
	bool testTargetCount(FilterOperator op, int value) const;
	bool testBrushModelId(FilterOperator op, int value) const;
	bool testBrushShape(FilterOperator op, int value) const;
	bool testMapRedId(FilterOperator op, int value) const;
	bool testMapGreenId(FilterOperator op, int value) const;
	bool testMapBlueId(FilterOperator op, int value) const;

	/**
	 * @brief Gets the points of this stroke.
	 * @param result Output array to store the points.
	 * @param withTraversal Whether to include traversal points (default: false).
	 */
	void getPoints(MFloatPointArray &result,
				   bool withTraversal = false) const;

	/**
	 * @brief Transforms vectors by the targets' matrices.
	 * @param vec The vector to transform.
	 * @param result Output array to store the transformed vectors.
	 * @param withTraversal Whether to include traversal targets (default: false).
	 */
	void transform(const MFloatVector &vec, MFloatVectorArray &result,
				   bool withTraversal = false) const;

	/**
	 * @brief Gets the X axes of all targets in this stroke.
	 * @param result Output array to store the X axes.
	 * @param withTraversal Whether to include traversal targets (default: false).
	 */
	void getXAxes(MFloatVectorArray &result, bool withTraversal = false) const;

	/**
	 * @brief Gets the Y axes of all targets in this stroke.
	 * @param result Output array to store the Y axes.
	 * @param withTraversal Whether to include traversal targets (default: false).
	 */
	void getYAxes(MFloatVectorArray &result, bool withTraversal = false) const;

	/**
	 * @brief Gets the Z axes of all targets in this stroke.
	 * @param result Output array to store the Z axes.
	 * @param withTraversal Whether to include traversal targets (default: false).
	 */
	void getZAxes(MFloatVectorArray &result, bool withTraversal = false) const;

	/**
	 * @brief Gets the border points of this stroke.
	 * @param planeNormal The normal vector of the plane.
	 * @param brush The brush to use for calculating borders.
	 * @param lefts Output array to store the left border points.
	 * @param rights Output array to store the right border points.
	 * @param scaleWidthByWeight Whether to scale the width by target weights.
	 * @param maxSegments Maximum number of segments to include (-1 for all).
	 */
	void getBorders(
		const MFloatVector &planeNormal,
		const Brush &brush,
		MFloatPointArray &lefts,
		MFloatPointArray &rights,
		bool scaleWidthByWeight,
		int maxSegments = -1) const;

	/**
	 * @brief Gets a loop of border points for this stroke.
	 * @param planeNormal The normal vector of the plane.
	 * @param brush The brush to use for calculating borders.
	 * @param result Output array to store the border loop points.
	 * @param scaleWidthByWeight Whether to scale the width by target weights.
	 * @param maxSegments Maximum number of segments to include (-1 for all).
	 */
	void getBorderLoop(
		const MFloatVector &planeNormal,
		const Brush &brush,
		MFloatPointArray &result,
		bool scaleWidthByWeight,
		int maxSegments = -1) const;

	/**
	 * @brief Gets a triangle strip for this stroke.
	 * @param planeNormal The normal vector of the plane.
	 * @param brush The brush to use for calculating the strip.
	 * @param result Output array to store the triangle strip points.
	 * @param scaleWidthByWeight Whether to scale the width by target weights.
	 * @param maxSegments Maximum number of segments to include (-1 for all).
	 */
	void getTriangleStrip(
		const MFloatVector &planeNormal,
		const Brush &brush,
		MFloatPointArray &result,
		bool scaleWidthByWeight,
		int maxSegments = -1) const;

	/**
	 * @brief Gets the border colors for targets in this stroke.
	 * @param result Output array to store the colors.
	 * @param maxSegments Maximum number of segments to include.
	 * @param displayMode The display mode for target colors.
	 */
	void getTargetBorderColors(
		MColorArray &result,
		int maxSegments,
		PaintingEnums::TargetColorsDisplay displayMode) const;

	/**
	 * @brief Gets the positions of targets in a specific space.
	 * @param space The transformation matrix defining the space.
	 * @param result Output array to store the positions.
	 */
	void positions(
		const MFloatMatrix &space,
		MFloatPointArray &result) const;

	/**
	 * @brief Gets the rotations of targets in a specific space.
	 * @param space The transformation matrix defining the space.
	 * @param order The rotation order.
	 * @param unit The angle unit.
	 * @param result Output array to store the rotations.
	 */
	void rotations(
		const MFloatMatrix &space,
		MTransformationMatrix::RotationOrder order,
		MAngle::Unit unit,
		MFloatVectorArray &result) const;

	/**
	 * @brief Gets the draw tangents of targets in a specific space.
	 * @param space The transformation matrix defining the space.
	 * @param result Output array to store the draw tangents.
	 */
	void drawTangents(
		const MFloatMatrix &space,
		MFloatVectorArray &result) const;

	/**
	 * @brief Gets the draw tangents of targets in world space.
	 * @param result Output array to store the draw tangents.
	 */
	void drawTangents(
		MFloatVectorArray &result) const;

	/**
	 * @brief Gets the colors of all targets in this stroke.
	 * @param result Output array to store the colors.
	 */
	void colors(MColorArray &result) const;

	/**
	 * @brief Gets the head point of this stroke on a plane.
	 * @param planeNormal The normal vector of the plane.
	 * @return The head point.
	 */
	MFloatPoint getHead(
		const MFloatVector &planeNormal) const;

	/**
	 * @brief Gets the departure target of this stroke.
	 * @return The departure target.
	 */
	const Target &departure() const;

	/**
	 * @brief Sets the departure offset of this stroke.
	 * @param offset The offset vector.
	 */
	void setDeparture(const MFloatVector &offset);

	/**
	 * @brief Sets the arrival offset of this stroke.
	 * @param offset The offset vector.
	 */
	void setArrival(const MFloatVector &offset);

	/**
	 * @brief Sets the arrival offset of this stroke based on a previous stroke.
	 * @param offset The offset vector.
	 * @param threshold The threshold distance.
	 * @param prev The previous stroke.
	 */
	void setArrival(
		const MFloatVector &offset,
		float threshold,
		const Stroke &prev);

	/**
	 * @brief Gets the arrival positions in a specific space.
	 * @param space The transformation matrix defining the space.
	 * @param result Output array to store the arrival positions.
	 */
	void arrivalPositions(
		const MFloatMatrix &space,
		MFloatPointArray &result) const;

	/**
	 * @brief Gets the arrival rotations in a specific space.
	 * @param space The transformation matrix defining the space.
	 * @param order The rotation order.
	 * @param unit The angle unit.
	 * @param result Output array to store the arrival rotations.
	 */
	void arrivalRotations(
		const MFloatMatrix &space,
		MTransformationMatrix::RotationOrder order,
		MAngle::Unit unit,
		MFloatVectorArray &result) const;

	/**
	 * @brief Gets the departure position in a specific space.
	 * @param space The transformation matrix defining the space.
	 * @param result Output parameter to store the departure position.
	 */
	void departurePosition(
		const MFloatMatrix &space,
		MFloatPoint &result) const;

	/**
	 * @brief Gets the departure rotation in a specific space.
	 * @param space The transformation matrix defining the space.
	 * @param order The rotation order.
	 * @param unit The angle unit.
	 * @param result Output parameter to store the departure rotation.
	 */
	void departureRotation(
		const MFloatMatrix &space,
		MTransformationMatrix::RotationOrder order,
		MAngle::Unit unit,
		MFloatVector &result) const;

	/**
	 * @brief Gets the linear speed of this stroke.
	 * @return The linear speed value.
	 */
	float linearSpeed() const;

	/**
	 * @brief Sets the linear speed of this stroke.
	 * @param val The new linear speed value.
	 */
	void setLinearSpeed(float val);

	/**
	 * @brief Gets the angular speed of this stroke.
	 * @param unit The angle unit (default: radians).
	 * @return The angular speed value.
	 */
	float angularSpeed(MAngle::Unit unit = MAngle::kRadians) const;

	/**
	 * @brief Sets the angular speed of this stroke.
	 * @param val The new angular speed value.
	 */
	void setAngularSpeed(float val);

	/**
	 * @brief Gets the approximation distance of this stroke.
	 * @return The approximation distance value.
	 */
	float approximationDistance() const;

	/**
	 * @brief Sets the approximation distance of this stroke.
	 * @param val The new approximation distance value.
	 */
	void setApproximationDistance(float val);

	/**
	 * @brief Smooths the targets in this stroke.
	 * @param neighbors Number of neighboring targets to consider.
	 * @param doPositions Whether to smooth positions.
	 * @param doWeights Whether to smooth weights.
	 */
	void smoothTargets(int neighbors, bool doPositions, bool doWeights);

	/**
	 * @brief Less-than operator for comparing strokes.
	 * @param a First stroke.
	 * @param b Second stroke.
	 * @return True if a should be ordered before b.
	 */
	friend bool operator<(const Stroke &a, const Stroke &b);

	/**
	 * @brief Output stream operator for strokes.
	 * @param os The output stream.
	 * @param s The stroke to output.
	 * @return The output stream.
	 */
	friend ostream &operator<<(ostream &os, const Stroke &s);

private:
	/**
	 * @brief Reverses an array in place.
	 * @param arr The array to reverse.
	 */
	static void reverseArray(MDoubleArray &arr);

	/**
	 * @brief Reverses an array and stores the result in another array.
	 * @param arr The array to reverse.
	 * @param result Output array to store the reversed array.
	 */
	static void reverseArray(
		const MDoubleArray &arr,
		MDoubleArray &result);

	/**
	 * @brief Determines whether to make a backstroke.
	 * @param dCurve The curve object.
	 * @param startDist The start distance.
	 * @param endDist The end distance.
	 * @param strokeDirection The direction method.
	 * @return True if a backstroke should be made.
	 */
	static bool shouldMakeBackstroke(MObject dCurve, double startDist, double endDist,
									 Stroke::DirectionMethod strokeDirection);

	/**
	 * @brief Sets the transition contact for this stroke.
	 */
	void setTransitionContact();

	std::vector<Target> m_targets;    /**< The list of targets. */
	Target m_pivot;                   /**< The pivot target. */
	std::vector<Target> m_arrivals;   /**< List of targets describing the path to the start of the stroke. */
	Target m_departure;               /**< Target after the last stroke target. */

	int m_strokeId;                   /**< Stroke ID. */
	int m_segmentId;                  /**< Segment ID. */
	int m_brushId;                    /**< Brush ID. */
	int m_paintId;                    /**< Paint ID. */
	int m_layerId;                    /**< Layer ID. */
	int m_parentId;                   /**< Parent ID. */
	int m_repeatId;                   /**< Repeat ID. */
	int m_brushModelId;               /**< Brush model ID. */

	float m_maxRadius;                /**< Maximum radius value. */

	MFloatVector m_sortColor;         /**< Color used for sorting. */
	MFloatVector m_filterColor;       /**< Color used for filtering. */
	MIntArray m_sortStack;            /**< Stack of sort operations. */

	float m_linearSpeed;              /**< Linear speed value (cm/sec). */
	float m_angularSpeed;             /**< Angular speed value (per sec). */
	float m_approximationDistance;    /**< Approximation distance value (cm). */

	float m_coil;                     /**< Coil value. */

	MString m_creatorName;            /**< Name of the creator. */
	int m_creatorId;                  /**< ID of the creator. */

	float m_ditherProbability;        /**< Dither probability value. */

	BrushStrokeSpec m_brushStrokeSpec; /**< Brush stroke specification. */
};

#endif
