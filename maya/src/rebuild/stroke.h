
#ifndef _stroke_
#define _stroke_

#include <vector>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>

#include <maya/MObject.h>

#include <mayaMath.h>
#include <attrUtils.h>

#include "strokeRotationSpec.h"
#include "strokeRepeatSpec.h"

#include "target.h"


class Stroke {
public:

	// enum Scope { kStroke, kTravelStroke, kCurve };

	enum DirectionMethod { kForwards, kBackwards, kStartUppermost, kEndUppermost  };



	enum StrokeFilterOperator {
		kGreaterThan,
		kLessThan,
		kEqualTo,
		kNotEqualTo,
		kNoOp
	};

	/* factory */
	unsigned create(
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
	  std::vector<Stroke> &strokes
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
	  int repeatId);


	// copy ctor can also apply an offset and/or
	// reverse the direction.
	Stroke(
	  const Stroke &other,
	  double offset = 0.0,
	  bool reverse = false,
	  int repeatId = 0);

	~Stroke();

	void setRotations(  const MObject &thisObj,
	                    const StrokeRotationSpec &rotSpec,
	                    bool backstroke = false);

	void setPivot(
	  const MObject &curveObject,
	  double fraction,
	  double startDist,
	  double endDist) ;

	unsigned size() const;

	void appendTargets(MMatrixArray &result) const;

	void appendTangents(MVectorArray &result) const;

	void getParams(MDoubleArray &result) const ;

	void getCurveParams(MDoubleArray &result) const ;

	const std::vector<Target> &targets() const;

	const double &arcLength() const;

	const double &entryLength() const;

	const double &exitLength() const;

	const MPoint &pivot() const ;


	int id() const;
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

	void appendIdToSortStack(bool ascending);

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

	bool testAgainstValue(int lhs, StrokeFilterOperator op, int rhs ) const;

	bool testId(StrokeFilterOperator op, int value) const;
	bool testParentId(StrokeFilterOperator op, int value) const;
	bool testBrushId(StrokeFilterOperator op, int value) const;
	bool testPaintId(StrokeFilterOperator op, int value) const;
	bool testLayerId(StrokeFilterOperator op, int value) const;
	bool testRepeatId(StrokeFilterOperator op, int value) const;
	bool testCustomBrushId(StrokeFilterOperator op, int value) const;
	bool testCustomPaintId(StrokeFilterOperator op, int value) const;
	bool testMapRedId(StrokeFilterOperator op, int value) const;
	bool testMapGreenId(StrokeFilterOperator op, int value) const;
	bool testMapBlueId(StrokeFilterOperator op, int value) const;



private:

	void reverseArray(MDoubleArray &arr) const ;


	bool shouldMakeBackstroke(MObject dCurve, double startDist, double endDist,
	                          Stroke::DirectionMethod strokeDirection) const;

	void setArcLength();

	std::vector<Target> m_targets;  // flat targets with 3d rotations

	MPoint m_pivot;
	double m_arcLength;
	double m_entryLength;
	double m_exitLength;
	int m_repeatId;

	// from  strokeGeom
	int m_id;
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

};


#endif

