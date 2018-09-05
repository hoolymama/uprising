
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

	Stroke(	const Stroke 	&other	)	;

	~Stroke();



	static unsigned factory(
	  const MObject &thisObj,
	  const MObject &dCurve,
	  const MMatrix &inversePlaneMatrix,
	  const MVector &planeNormal,
	  double curveLength,
	  double startDist,
	  double endDist,
	  double pointDensity,
	  double liftLength,
	  double liftHeight,
	  double liftBias,
	  const MObject &profileRampAttribute,
	  double strokeProfileScaleMin,
	  double strokeProfileScaleMax,
	  const MObject &tiltRamp,
	  const MObject &bankRamp,
	  const MObject &twistRamp,
	  Stroke::Scope brushRampScope,
	  bool follow,
	  bool backstroke,
	  int repeats,
	  double repeatOffset,
	  bool repeatMirror,
	  bool repeatOscillate,
	  double pivotFraction,
	  short brushId,
	  short paintId,
	  std::vector<std::unique_ptr<Stroke> > &strokes

	) ;

	// unsigned  generateRepeats(
	//   unsigned repeats,
	//   double repeatOffset,
	//   bool repeatMirror,
	//   bool repeatOscillate,
	//   const MVector &planeNormal,
	//   const MMatrix &inversePlaneMatrix,
	//   std::vector<std::unique_ptr<Stroke> > &strokes) const ;


	void offsetFrom(
	  const Stroke &other,
	  double offset);


	void initialize(
	  const MObject &curveObject ,
	  const MVector &planeNormal,
	  double curveLength,
	  double startDist,
	  double endDist,
	  double density,
	  double liftLength,
	  double liftBias,
	  short brushId,
	  short paintId);

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

	void setPivot(
	  const MObject &curveObject,
	  double fraction,
	  double startDist,
	  double endDist) ;

	unsigned length() const;


	// virtual bool  shouldMakeBackstroke( bool oscillate,  int index ) const  ;

	virtual void appendTargets(MMatrixArray &result) const;

	virtual void  appendTangents(MVectorArray &result) const;

	virtual void  appendPoints(MVectorArray &result) const;

	virtual void getTravelStrokeFractions(MDoubleArray &result) const;

	void getStrokeFractions(MDoubleArray &result) const ;

	void getCurveFractions(MDoubleArray &result) const ;


	bool overlapsPlane(const MMatrix &inversePlaneMatrix) const;


	const std::vector<Target> &targets() const;

	const MVector &planeNormal() const ;

	short brushId() const;
	short paintId() const;


	const MDoubleArray &profile() const;

	double arcLength() const;

	const MPoint &pivot() const ;

	void setApproach(double start, double end) ;


	// double approachDistStart() const;

	// double approachDistEnd() const ;

	bool follow() const;

	virtual short direction() const ;

	virtual	void  getApproachTargets( MMatrix &startApproach, MMatrix &endApproach) const;


protected:
	void setArcLength();

	std::vector<Target> m_targets;
	MDoubleArray m_profile;
	MPoint m_pivot;
	bool m_follow;
	double m_arcLength;
	double m_approachDistStart;
	double m_approachDistEnd;
	MVector m_planeNormal;
	short m_brushId;
	short m_paintId;

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



