
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

	Target(
	  const MPoint &pt,
	  const MVector &tangent,
	  double strokeParam,
	  double curveParam,
	  double contact
	);


	~Target();

	void setRotation(double tilt, double bank, double twist, bool follow,
	                 bool backstroke);

	const MMatrix &matrix() const;

	const MVector &tangent() const ;

	const double &param() const ;

	void reverseParam();

	const double &curveParam() const ;

	void offsetBy(const MVector &offset) ;

	double distanceTo(const Target &other) const;

	MMatrix directionMatrix(bool backstroke) const;

	MPoint position(const MMatrix &space = MMatrix::identity) const;

	void setPosition(const MPoint &rhs);

	void setMatrix(const MMatrix &rhs);

	void rotate(const MPoint &pivot, const MMatrix &rotation);

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

	void setContact(double contact);

	const double &contact() const ;

	// static double Target::interpContact(const MDoubleArray &contacts,
	//                                     const double &uniformParam)

private:
	MVector m_tangent; // tangent on the curve
	MMatrix m_matrix;
	double m_param; // normalised length in stroke
	double m_curveParam; //normalised length in original curve
	double m_contact; // fraction of brush tip


};


#endif

