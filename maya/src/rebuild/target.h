
#ifndef _Target_
#define _Target_

#include <vector>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MFnNurbsCurve.h>


#include <maya/MPoint.h>


#include <maya/MObject.h>
#include <mayaMath.h>

class Target {
public:


	Target();

	Target(
	  const MMatrix &mat,
	  const MVector &tangent,
	  double strokeParam,
	  double curveParam);


	Target(
	  const MFnNurbsCurve &curveFn,
	  double dist,
	  double startDist,
	  double strokeRange,
	  double curveLength);

	~Target();

	void setRotation(double tilt, double bank, double twist, bool follow,
	                 bool backstroke);

	const MMatrix &matrix() const;

	const MVector &tangent() const ;

	const double &param() const ;

	const double &curveParam() const ;

	void offsetBy(const MVector &offset) ;

	double distanceTo(const Target &other) const;


	MMatrix directionMatrix(bool backstroke) const;

	MPoint position(const MMatrix &space = MMatrix::identity) const;


	void setPosition(const MPoint &rhs);

	MVector rotation(
	  MTransformationMatrix::RotationOrder order,
	  MAngle::Unit unit,
	  const MMatrix &space = MMatrix::identity) const;

	MVector  transform(const MVector &rhs) const;

	void getBorderPoints(
	  MFloatPoint &left,
	  MFloatPoint &right,
	  double width,
	  bool flat = false) const;

private:
	MVector m_tangent; // tangent on the curve
	MMatrix m_matrix;
	double m_param; // normalised length in stroke
	double m_curveParam; //normalised length in original curve


};

#endif

