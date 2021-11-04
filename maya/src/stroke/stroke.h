
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

#include "strokeRotationSpec.h"
#include "strokeRepeatSpec.h"

#include "brush.h"
#include "paint.h"
#include "target.h"
#include "enums.h"

class Stroke
{
public:
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

	enum TranslationDirection
	{
		kConstant,
		kAlternateRepeat,
		kAlternateAll
	};

	enum DirectionMethod
	{
		kForwards,
		kBackwards,
		kStartUppermost,
		kEndUppermost,
		kRadialIn,
		kRadialOut
	};

	enum FilterOperator
	{
		kGreaterThan,
		kLessThan,
		kEqualTo,
		kNotEqualTo,
		kNoOp
	};

	enum SortFilterKey
	{
		kStrokeId,
		kBrushId,
		kPaintId,
		kRepeatId,
		kLayerId,
		kParentId,
		kTargetCount,
		kMapRed,
		kMapGreen,
		kMapBlue,
	};

	enum SortDirection
	{
		kSortAscending,
		kSortDescending
	};

	static float interpFloat(const MFloatArray &values, float param);

	Stroke();

	/**
 * @brief Construct a new Stroke object with an artray of points and an initial
 * rotation matrix. 
 
 All targets will have this matrix initially.
 View tangents will be calculated from points.
 *
 * @param points An array of points.
 * @param rotationMat A matrix, which is expected to be unit scale. 
 */
	Stroke(
		const MFloatPointArray &points,
		const MFloatMatrix &rotationMat);

	Stroke(
		const std::vector<MFloatMatrix> &matrices);

	Stroke(
		const MFloatPointArray &points,
		const MFloatArray &weights,
		const MFloatMatrix &rotationMat);

	Stroke(
		const MFloatPointArray &points,
		const MColorArray &colors,
		const MFloatMatrix &rotationMat);



	Stroke(
		const std::vector<MFloatMatrix> &matrices,
		const MFloatArray &weights);

	/**
	 * @brief Construct a new Stroke from a subsection of the input stroke.
	 * 
	 * @param instroke The stroke to create a new stroke from.
	 * @param start The target index to start at
	 * @param count The number of targets.
	 */
	Stroke(
		const Stroke &instroke,
		unsigned start,
		unsigned count);

	~Stroke();

	void setStrokeId(unsigned rhs);

	void resetTangents();

	void calculateTangents(
		const MFloatPointArray &points,
		MFloatVectorArray &tangents) const;

	void calculateTangents(
		const std::vector<MFloatMatrix> &matrices,
		MFloatVectorArray &tangents) const;

	void setCoil(float rhs);

	float coil() const;

	unsigned valid() const;

	unsigned size(bool withTraversal = false) const;

	// void appendTangents(MVectorArray &result) const;

	void getParams(MFloatArray &result) const;

	const std::vector<Target> &targets() const;

	float calculateArcLength() const;

	void calculateParams(MFloatArray &result) const;

	const float &paintFlow() const;

	const Target &pivot() const;

	int strokeId() const;
	int parentId() const;
	int repeatId() const;

	void setParentId(int parentId);
	void setRepeatId(int rhs);

	const MIntArray &sortStack() const;

	void clearSortStack();

	void setLayerId(int rhs);
	int layerId() const;
	int customBrushId() const;

	int brushId() const;
	int paintId() const;

	void setBrushId(int val);
	// void setCustomBrushId(int val);

	void setPaintId(int val);

	int potId() const;
	void setPotId(int val);

	void setSortColor(const MFloatVector &color);
	void setFilterColor(const MFloatVector &color);

	void appendStrokeIdToSortStack(bool ascending);
	void appendParentIdToSortStack(bool ascending);

	void appendBrushIdToSortStack(bool ascending);

	void appendPaintIdToSortStack(bool ascending);

	void appendLayerIdToSortStack(bool ascending);

	void appendRepeatIdToSortStack(bool ascending);
	void appendTargetCountToSortStack(bool ascending);

	// void appendCustomBrushIdToSortStack(bool ascending);

	void appendMapRedIdToSortStack(bool ascending);
	void appendMapGreenIdToSortStack(bool ascending);
	void appendMapBlueIdToSortStack(bool ascending);

	bool testAgainstValue(int lhs, FilterOperator op, int rhs) const;

	bool testStrokeId(FilterOperator op, int value) const;
	bool testParentId(FilterOperator op, int value) const;
	bool testBrushId(FilterOperator op, int value) const;
	bool testPaintId(FilterOperator op, int value) const;
	bool testLayerId(FilterOperator op, int value) const;
	bool testRepeatId(FilterOperator op, int value) const;
	bool testTargetCount(FilterOperator op, int value) const;

	bool testMapRedId(FilterOperator op, int value) const;
	bool testMapGreenId(FilterOperator op, int value) const;
	bool testMapBlueId(FilterOperator op, int value) const;

	void getPoints(MFloatPointArray &result /*, float stackHeight = 0.0f*/,
				   bool withTraversal = false) const;

	void transform(const MFloatVector &vec, MFloatVectorArray &result,
				   bool withTraversal = false) const;

	void getXAxes(MFloatVectorArray &result, bool withTraversal = false) const;
	void getYAxes(MFloatVectorArray &result, bool withTraversal = false) const;
	void getZAxes(MFloatVectorArray &result, bool withTraversal = false) const;

	void getBorders(
		const MFloatVector &planeNormal,
		const Brush &brush,

		MFloatPointArray &lefts,
		MFloatPointArray &rights,
		bool scaleWidthByWeight,
		int maxSegments = -1) const;

	void getBorderLoop(
		const MFloatVector &planeNormal,
		const Brush &brush,

		MFloatPointArray &result,
		bool scaleWidthByWeight,
		int maxSegments = -1) const;

	void getTriangleStrip(
		const MFloatVector &planeNormal,
		const Brush &brush,

		MFloatPointArray &result,
		bool scaleWidthByWeight,
		int maxSegments = -1) const;

	void getTargetBorderColors(
		MColorArray &result,
		int maxSegments,
		PaintingEnums::TargetColorsDisplay displayMode) const;

	void positions(
		const MFloatMatrix &space,
		MFloatPointArray &result) const;

	void rotations(
		const MFloatMatrix &space,
		MTransformationMatrix::RotationOrder order,
		MAngle::Unit unit,
		MFloatVectorArray &result) const;

	void drawTangents(
		const MFloatMatrix &space,
		MFloatVectorArray &result) const;

	void colors(MColorArray &result) const;

	MFloatPoint getHead(
		const MFloatVector &planeNormal /*, float stackHeight = 0.0f*/) const;

	const Target &departure() const;

	void setDeparture(const MFloatVector &offset);
	void setArrival(const MFloatVector &offset);
	void setArrival(
		const MFloatVector &offset,
		float threshold,
		const Stroke &prev);

	void arrivalPositions(
		const MFloatMatrix &space,
		MFloatPointArray &result) const;

	void arrivalRotations(
		const MFloatMatrix &space,
		MTransformationMatrix::RotationOrder order,
		MAngle::Unit unit,
		MFloatVectorArray &result) const;

	void departurePosition(
		const MFloatMatrix &space,
		MFloatPoint &result) const;

	void departureRotation(
		const MFloatMatrix &space,
		MTransformationMatrix::RotationOrder order,
		MAngle::Unit unit,
		MFloatVector &result) const;

	float linearSpeed() const;
	void setLinearSpeed(float val);

	float angularSpeed(MAngle::Unit unit = MAngle::kRadians) const;
	void setAngularSpeed(float val);

	float approximationDistance() const;
	void setApproximationDistance(float val);

	

	void smoothTargets(int neighbors, bool doPositions, bool doWeights);

	friend bool operator<(const Stroke &a, const Stroke &b);
	friend ostream &operator<<(ostream &os, const Stroke &s);

private:
	static void reverseArray(MDoubleArray &arr);
	static void reverseArray(
		const MDoubleArray &arr,
		MDoubleArray &result);

	static bool shouldMakeBackstroke(MObject dCurve, double startDist, double endDist,
									 Stroke::DirectionMethod strokeDirection);

	void setTransitionContact();

	std::vector<Target> m_targets; // flat targets with 3d rotations
	Target m_pivot;
	std::vector<Target> m_arrivals;
	Target m_departure;

	int m_strokeId;
	int m_brushId;
	int m_paintId;
	int m_potId;
	int m_layerId;
	int m_parentId;
	int m_customBrushId;
	int m_repeatId;
	MFloatVector m_sortColor;
	MFloatVector m_filterColor;
	MIntArray m_sortStack;
	float m_linearSpeed;
	float m_angularSpeed;
	float m_approximationDistance;

	float m_coil;
};

inline float Stroke::interpFloat(const MFloatArray &values, float param)
{
	int len = values.length();
	int last = (len - 1);
	if (param >= 1.0f)
	{
		return values[last];
	}
	else if (param <= 0.0f)

	{
		return values[0];
	}
	float t = param * last;
	float r = t - floor(t);
	int lindex = int(t);
	return (values[lindex] * (1 - r)) + (values[(lindex + 1)] * (r));
}

#endif
