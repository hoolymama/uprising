
#ifndef _stroke_
#define _stroke_

#include <vector>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>

#include <maya/MFnMesh.h>

#include <maya/MObject.h>

#include <mayaMath.h>
#include <attrUtils.h>

#include "strokeRotationSpec.h"
#include "strokeRepeatSpec.h"

#include "brush.h"
#include "paint.h"
#include "target.h"


class Stroke {
public:

	// enum Scope { kStroke, kTravelStroke, kCurve };

	enum DirectionMethod { kForwards, kBackwards, kStartUppermost, kEndUppermost  };

	enum FilterOperator {
		kGreaterThan,
		kLessThan,
		kEqualTo,
		kNotEqualTo,
		kNoOp
	};

	enum SortFilterKey {
		kStrokeId,
		kBrushId,
		kPaintId,
		kRepeatId,
		kLayerId,
		kMapRed,
		kMapGreen,
		kMapBlue
	};


	enum SortDirection { kSortAscending,  kSortDescending};



	/* factory */
	static unsigned create(
	  const MObject &thisObj,
	  const MObject &dCurve,
	  double curveLength,
	  double startDist,
	  double endDist,
	  double entryLength,
	  double exitLength,
	  double pointDensity,
	  const StrokeRotationSpec &rotSpec,
	  const StrokeRepeatSpec &repeatSpec,
	  DirectionMethod strokeDirection,
	  double pivotParam,
	  int strokeId,
	  int brushId,
	  int paintId,
	  int layerId,
	  std::vector<Stroke> *strokes
	) ;


	Stroke();


	Stroke(
	  const MObject &curveObject ,
	  double curveLength,
	  double startDist,
	  double endDist,
	  double entryLength,
	  double exitLength,
	  double density,
	  double pivotParam,
	  int strokeId,
	  int brushId,
	  int paintId,
	  int layerId,
	  int repeatId,
	  bool backstroke);

	void offset(
	  double offset,
	  bool reverse,
	  int repeatId);

	~Stroke();

	void setRotations(   const MObject &thisObj,
	                     const StrokeRotationSpec &rotSpec);

	// void setPivot(
	//   const MObject &curveObject,
	//   double fraction,
	//   double startDist,
	//   double endDist) ;

	unsigned size() const;

	void appendTargets(MMatrixArray &result) const;

	void appendTangents(MVectorArray &result) const;

	void getParams(MDoubleArray &result) const ;

	void getCurveParams(MDoubleArray &result) const ;

	const std::vector<Target> &targets() const;

	const double &arcLength() const;

	const double &entryLength() const;

	const double &exitLength() const;

	const Target &pivot() const ;

	bool backstroke() const;

	int strokeId() const;
	int parentId() const;
	int repeatId() const ;


	void setParentId(int parentId);

	const MIntArray &sortStack() const;

	void clearSortStack();




	int layerId() const;

	int brushId() const ;
	int paintId() const ;

	void setBrushId(int val) ;
	void setPaintId(int val) ;
	void setSortColor(const MFloatVector &color);
	void setFilterColor(const MFloatVector &color);

	void appendStrokeIdToSortStack(bool ascending);

	void appendParentIdToSortStack(bool ascending);

	void appendBrushIdToSortStack(bool ascending);

	void appendPaintIdToSortStack(bool ascending);

	void appendLayerIdToSortStack(bool ascending);

	void appendRepeatIdToSortStack(bool ascending);

	void appendCustomBrushIdToSortStack(bool ascending);

	void appendCustomPaintIdToSortStack(bool ascending);

	void appendMapRedIdToSortStack(bool ascending);
	void appendMapGreenIdToSortStack(bool ascending);
	void appendMapBlueIdToSortStack(bool ascending);

	bool testAgainstValue(int lhs, FilterOperator op, int rhs ) const;

	bool testStrokeId(FilterOperator op, int value) const;
	bool testParentId(FilterOperator op, int value) const;
	bool testBrushId(FilterOperator op, int value) const;
	bool testPaintId(FilterOperator op, int value) const;
	bool testLayerId(FilterOperator op, int value) const;
	bool testRepeatId(FilterOperator op, int value) const;
	bool testCustomBrushId(FilterOperator op, int value) const;
	bool testCustomPaintId(FilterOperator op, int value) const;
	bool testMapRedId(FilterOperator op, int value) const;
	bool testMapGreenId(FilterOperator op, int value) const;
	bool testMapBlueId(FilterOperator op, int value) const;


	void getDirectionMatrices(MMatrixArray &result, double stackHeight) const;

	void getPoints(MFloatPointArray &result, double stackHeight,
	               bool withTraversal = false) const ;
	void transform(const MVector &vec, MFloatVectorArray &result,
	               bool withTraversal = false) const ;
	void getXAxes(MFloatVectorArray &result, bool withTraversal = false) const ;
	void getYAxes(MFloatVectorArray &result, bool withTraversal = false) const;
	void getZAxes(MFloatVectorArray &result, bool withTraversal = false) const ;


	void getBorders(
	  MFloatPointArray &lefts,
	  MFloatPointArray &rights,
	  const Brush &brush,
	  double stackHeight) const;



	void positions(const MMatrix &space, MPointArray &result) const;
	void rotations(
	  const MMatrix &space,
	  MTransformationMatrix::RotationOrder order,
	  MAngle::Unit unit,
	  MVectorArray &result ) const;


	void tangents(const MMatrix &space, MVectorArray &result) const;

	void getHead(MFloatPoint &result,  float stackHeight) const ;


	void setCustomSortData(const Brush &brush,  const Paint &paint);

	void setUV(const MMatrix &inversePlaneMatrix);
	void getUV( float &u, float &v);
	void displace( MFnMesh &meshFn, MMeshIsectAccelParams &ap);




	const Target &departure() const;

	void setDeparture(double offset);
	void setArrival(double offset);
	void setArrival(double offset, double threshold, const Stroke &prev);

private:

	static void reverseArray(MDoubleArray &arr)  ;


	static bool shouldMakeBackstroke(MObject dCurve, double startDist, double endDist,
	                                 Stroke::DirectionMethod strokeDirection) ;

	void setArcLength();

	std::vector<Target> m_targets;  // flat targets with 3d rotations


	double m_arcLength;
	double m_entryLength;
	double m_exitLength;
	int m_repeatId;

	// from  strokeGeom
	int m_strokeId;
	int m_parentId;
	int m_brushId;
	int m_paintId;
	int m_layerId;
	int m_customPaintId;
	int m_customBrushId;

	float m_u;
	float m_v;

	MFloatVector m_sortColor;
	MFloatVector m_filterColor;
	MIntArray m_sortStack;

	bool m_backstroke;

	std::vector<Target> m_arrivals;

	Target m_departure;

	Target m_pivot;

};


#endif

