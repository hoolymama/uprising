
#ifndef _stroke_
#define _stroke_

#include <vector>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>

#include <maya/MObject.h>

#include <mayaMath.h>
#include <attrUtils.h>


// #include "brush.h"
#include "target.h"


class Stroke {
public:

	enum Scope { kStroke, kTravelStroke, kCurve };

	Stroke();

	~Stroke();


	void initializeTargets(
	  const MObject &curveObject ,
	  double curveLength,
	  double startDist,
	  double endDist,
	  double density,
	  double liftLength,
	  double liftBias);

	void setHeights( const MObject &thisObj,
	                 const MObject &profileRampAttribute,
	                 double  strokeProfileScaleMin,
	                 double strokeProfileScaleMax,
	                 double liftHeight);

	void setRotations(
	  const MObject &thisObj,
	  const MObject &tiltRamp,
	  const MObject &bankRamp,
	  const MObject &twistRamp,
	  Stroke::Scope rampScope,
	  bool follow
	);

	unsigned length() const;

	double arcLength() const;

	virtual void appendTargets(const MVector &planeNormal, MMatrixArray &result) const;

	virtual void  appendTangents(MVectorArray &result) const;

	virtual void  appendPoints(MVectorArray &result) const;

	virtual void travelStrokeFractions(MDoubleArray &result) const;

	void strokeFractions(MDoubleArray &result) const ;

	void curveFractions(MDoubleArray &result) const ;
	// virtual void initializeTargets(
	//   const MObject &curveObject ,
	//   double curveLength,
	//   double startDist,
	//   double endDist,
	//   double density,
	//   double liftLength,
	//   double liftBias) = 0;







protected:


	std::vector<Target> m_targets;
	MPoint m_pivot;
	double m_arcLength;
	double m_approachDistStart;
	double m_approachDistEnd;
	bool m_follow;
};


#endif





// const MObject &curveObject,
// const MObject &node,
// const MVector &startEndDist,
// const MVector &planeNormal,
// const MVector &lift,
// const MObject &tiltRampAttr,
// const MObject &bankRampAttr,
// const MObject &twistRampAttr,
// Stroke::Scope brushRampScope,
// const MObject &profileRampAttr,
// const double &curveLength,
// double density,
// double pivotFraction,
// bool follow


// void setHeights();



// double m_startDist;
// double m_endDist;
// double m_liftLength;
// double m_liftHeight;
// double m_liftBias;
// bool m_isBackstroke;
// bool m_follow;


// const MMatrixArray &targets() const;

// const MVectorArray &tangents() const;

// const Brush &brush() const;

// const Paint &paint() const;

// double arcLength() const;

// bool follow() const;

// bool forceDip() const;

// unsigned curveId() const;

// double rotation() const;

// double translation() const;

// const MPoint &pivot() const;

// const MVector &brushRotate() const;

// bool overlapsPlane(const MMatrix &inversePlaneMatrix) const;

// void getPivotUVs(const MMatrix &inversePlaneMatrix, float &u, float &v) const ;

// void rotate(float rotation, const MVector &axis);

// void translate(const MFloatVector &translation, const MVector &planeNormal);

// void setApproach(double start, double end)  ;

// double approachStart() const;

// double approachEnd() const ;

// bool isBackstroke() const ;



