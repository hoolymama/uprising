
#ifndef _Target_
#define _Target_

#include <vector>
#include <maya/MVector.h>
#include <maya/MMatrix.h>

#include <maya/MPoint.h>


#include <maya/MObject.h>
#include <mayaMath.h>

class Target {
public:


	Target(
	  const MObject &curveObject,
	  double startDist,
	  double endDist,
	  double dist,
	  double curveLength
	);


	~Target();

	void setRotation(double tilt, double bank, double twist, bool follow,
	                 bool backstroke);

	const MMatrix &matrix() const;

	const MVector &tangent() const ;

	const double &param() const ;

	const double &curveParam() const ;

	void offsetBy(const MVector &offset) ;

	double distanceTo(const Target &other) const;


	MPoint position() const;

	MVector rotation(
	  MTransformationMatrix::RotationOrder order,
	  MAngle::Unit unit) const;



private:
	MVector m_tangent; // tangent on the curve
	MMatrix m_matrix;
	double m_param; // normalised length in stroke
	double m_curveParam; //normalised length in original curve


};

#endif

