
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
		kCustomBrushId,
		kMapRed,
		kMapGreen,
		kMapBlue,
	};

	enum SortDirection
	{
		kSortAscending,
		kSortDescending
	};

	static double interpContact(const MDoubleArray &contacts, const double &uniformParam);

	/* factory */
	// static unsigned create(
	// 	const MObject &thisObj,
	// 	const MObject &dCurve,
	// 	const MDoubleArray &contacts,
	// 	bool localContact,
	// 	double curveLength,
	// 	double startDist,
	// 	double endDist,
	// 	double entryLength,
	// 	double exitLength,
	// 	TransitionBlendMethod transBlendMethod,
	// 	double pointDensity,
	// 	int minimumPoints,
	// 	const StrokeRotationSpec &rotSpec,
	// 	const StrokeRepeatSpec &repeatSpec,
	// 	DirectionMethod strokeDirection,
	// 	double pivotParam,
	// 	double paintFlow,
	// 	int strokeId,
	// 	int brushId,
	// 	int paintId,
	// 	int layerId,
	// 	int customBrushId,
	// 	std::vector<Stroke> *strokes);

	Stroke();

	// Stroke(
	// 	const MObject &curveObject,
	// 	const MDoubleArray &contacts,
	// 	bool localContact,
	// 	double curveLength,
	// 	double startDist,
	// 	double endDist,
	// 	double entryLength,
	// 	double exitLength,
	// 	TransitionBlendMethod transBlendMethod,
	// 	double density,
	// 	int minimumPoints,
	// 	double pivotParam,
	// 	double paintFlow,
	// 	int strokeId,
	// 	int brushId,
	// 	int paintId,
	// 	int layerId,
	// 	int customBrushId,
	// 	int repeatId,
	// 	bool backstroke);

	Stroke(
		 const MObject &curveObject,
		 const MFloatVector &lanceAxis, // Z
		 const MFloatVector &majorAxis, // Y
		 const MFloatArray &weights,
		 bool localContact,
		 double startDist,
		 double endDist,
		 float entryLength,
		 float exitLength,
		 double density,
		 int minimumPoints,
		 double pivotParam,
		 float paintFlow,
		 int strokeId,
		 int brushId,
		 int paintId,
		 int layerId,
		 int customBrushId,
		 bool follow,
		 bool backstroke);

	/**
 * @brief Construct a new Stroke object with an artray of points and an initial
 * rotation matrix. All targets will have this matrix initially.
 *
 * @param points An array of points.
 * @param rotationMat A matrix, which is expected to be unit scale. 
 * @param index The stroke ID
 */
	Stroke(const MFloatPointArray &points, const MFloatMatrix &rotationMat, unsigned index);

	/**
	 * @brief Construct a new Stroke from a subsection of the input stroke.
	 * 
	 * @param instroke The stroke to create a new stroke from.
	 * @param start The target index to start at
	 * @param count The number of targets.
	 * @param index The stroke id of the new stroke.
	 */
	Stroke(
	const Stroke &instroke,
	unsigned start,
	unsigned count,
	unsigned index) ;


 
	~Stroke();

	// MStatus  buildCubicStroke(const MObject &curveObject, const MDoubleArray
	//    &contacts, double curveLength, double startDist, double endDist, double
	//    density, int minimumPoints, double pivotParam);

	// MStatus  buildLinearStroke(
	// 	const MObject &curveObject,
	// 	const MDoubleArray &contacts,
	// 	double curveLength,
	// 	double startDist,
	// 	double endDist,
	// 	double density,
	// 	int minimumPoints,
	// 	double pivotParam);

	void offset(
		 float offset,
		 const MFloatVector &planeNormal,
		 bool reverse,
		 int repeatId);

	void offset(
		 float tangentOffset,
		 float normalOffset,
		 const MFloatVector &planeNormal,
		 bool reverse,
		 int repeatId);

	// void setRotations(const MObject &thisObj,
	// 				  const StrokeRotationSpec &rotSpec);

	// void setPivot(
	//   const MObject &curveObject,
	//   double fraction,
	//   double startDist,
	//   double endDist) ;

	unsigned size(bool withTraversal = false) const;

	// void appendTargets(MMatrixArray &result) const;

	void appendTangents(MVectorArray &result) const;

	void getParams(MFloatArray &result) const;

	// void getArcLengths(MFloatArray &result) const;

	const std::vector<Target> &targets() const;

	// const float calculateArcLength() const;
	float calculateArcLength() const ;

	void  calculateParams(MFloatArray & result) const;
	// const float &entryLength() const;

	// const float &exitLength() const;

	const float &paintFlow() const;

	const Target &pivot() const;

	bool backstroke() const;

	int strokeId() const;
	int parentId() const;
	int repeatId() const;

	void setParentId(int parentId);

	const MIntArray &sortStack() const;

	void clearSortStack();

	int layerId() const;
	int customBrushId() const;

	int brushId() const;
	int paintId() const;

	void setBrushId(int val);
	void setCustomBrushId(int val);

	void setPaintId(int val);
	void setSortColor(const MFloatVector &color);
	void setFilterColor(const MFloatVector &color);

	void appendStrokeIdToSortStack(bool ascending);
	void appendParentIdToSortStack(bool ascending);

	void appendBrushIdToSortStack(bool ascending);

	void appendPaintIdToSortStack(bool ascending);

	void appendLayerIdToSortStack(bool ascending);

	void appendRepeatIdToSortStack(bool ascending);
	void appendTargetCountToSortStack(bool ascending);

	void appendCustomBrushIdToSortStack(bool ascending);

	// void appendCustomPaintIdToSortStack(bool ascending);

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

	bool testCustomBrushId(FilterOperator op, int value) const;
	// bool testCustomPaintId(FilterOperator op, int value) const;
	bool testMapRedId(FilterOperator op, int value) const;
	bool testMapGreenId(FilterOperator op, int value) const;
	bool testMapBlueId(FilterOperator op, int value) const;

	// void getDirectionMatrices(
	// 	const MFloatVector &planeNormal,
	// 	std::vector<MFloatMatrix> &result,
	// 	float stackHeight=0.0f) const;

	// void getDirectionMatrices(
	// 	MMatrixArray &result, double stackHeight) const;

	void getPoints(MFloatPointArray &result, float stackHeight = 0.0f,
						bool withTraversal = false) const;

	// void getPoints(MPointArray &result, float stackHeight=0.0f,
	// 			   bool withTraversal = false) const;

	void transform(const MFloatVector &vec, MFloatVectorArray &result,
						bool withTraversal = false) const;
	// void transform(const MVector &vec, MVectorArray &result,
	// 			   bool withTraversal = false) const;

	void getXAxes(MFloatVectorArray &result, bool withTraversal = false) const;
	void getYAxes(MFloatVectorArray &result, bool withTraversal = false) const;
	void getZAxes(MFloatVectorArray &result, bool withTraversal = false) const;

	// void getXAxes(MVectorArray &result, bool withTraversal = false) const;
	// void getYAxes(MVectorArray &result, bool withTraversal = false) const;
	// void getZAxes(MVectorArray &result, bool withTraversal = false) const;

	void getBorders(
		 const MFloatVector &planeNormal,
		 const Brush &brush,
		 float stackHeight,
		 MFloatPointArray &lefts,
		 MFloatPointArray &rights,
		 bool displayContactWidth,
		 int maxSegments = -1) const;

	void getBorderLoop(
		 const MFloatVector &planeNormal,
		 const Brush &brush,
		 float stackHeight,
		 MFloatPointArray &result,
		 bool displayContactWidth,
		 int maxSegments = -1) const;

	void getTriangleStrip(
		 const MFloatVector &planeNormal,
		 const Brush &brush,
		 float stackHeight,
		 MFloatPointArray &result,
		 bool displayContactWidth,
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

	void tangents(
		 const MFloatMatrix &space,
		 MFloatVectorArray &result) const;

	void colors(MColorArray &result) const;

	MFloatPoint getHead(
		 const MFloatVector &planeNormal,
		 float stackHeight = 0.0f) const;

	// MObject generateNurbsCurve(
	// 	const MPointArray &points, MStatus *st = 0) const;

	// void setCustomSortData(const Brush &brush,  const Paint &paint);

	// void setUV(const MFloatMatrix &inversePlaneMatrix);
	// void getUV(float &u, float &v) const;

	// void assignTargetUVs(const MFloatMatrix &inversePlaneMatrix);
	// void appendTargetUVsTo(MFloatArray &uVals, MFloatArray &vVals) const;

	// int applyGlobalTilt(const MFloatVectorArray &gradients, int index);
	// void applyGlobalAim(const MPoint &point);

	// void displace(MFnMesh &meshFn, MMeshIsectAccelParams &ap);

	void offsetBrushContact(const Brush &brush);

	// void applyBiases(const Brush &brush, float mult);
	// void applyForwardBias(const Brush &brush, float mult);
	// void applyGravityBias(const Brush &brush, float mult);

	const Target &departure() const;

	void setDeparture(double offset);
	void setArrival(double offset);
	void setArrival(
		 double offset,
		 double threshold,
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

	void setLinearSpeed(float val);
	void setAngularSpeed(float val);

	float linearSpeed() const;
	float angularSpeed() const;

	// void rotate(float rotation);
	// void translate(const MVector &translation, bool transformPivot = false);

	// void setTargetColors(const MFloatVectorArray &colors, const MFloatArray & whites, unsigned index );

	friend bool operator<(const Stroke &a, const Stroke &b);
	friend ostream &operator<<(ostream &os, const Stroke &s);

private:
	static void reverseArray(MDoubleArray &arr);
	static void reverseArray(
		 const MDoubleArray &arr,
		 MDoubleArray &result);

	static bool shouldMakeBackstroke(MObject dCurve, double startDist, double endDist,
												Stroke::DirectionMethod strokeDirection);

	// void setArcLengths();

	void setTransitionContact();

	std::vector<Target> m_targets; // flat targets with 3d rotations
	Target m_pivot;
	std::vector<Target> m_arrivals;
	Target m_departure;

	bool m_localContact;
	// float m_arcLength;
	float m_entryLength;
	float m_exitLength;
	float m_paintFlow;
	int m_strokeId;
	int m_brushId;
	int m_paintId;
	int m_layerId;
	int m_parentId;
	int m_customBrushId;
	int m_repeatId;
	bool m_backstroke;
	float m_linearSpeed;
	float m_angularSpeed;
	MFloatVector m_sortColor;
	MFloatVector m_filterColor;
	MIntArray m_sortStack;
	// float m_u;
	// float m_v;

	// TransitionBlendMethod m_transitionBlendMethod;

	// from  strokeGeom
	// bool m_follow;
	// int m_customPaintId;
	// int m_customBrushId;

	// int m_degree;

	// UV at the pivot

	// Travel in the opposite direction

	// Approach targets
};

#endif
