
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
	  const double &curveLength,
	  const double &strokeStartDist,
	  const double &strokeEndDist,
	  double dist
	);

	~Target();



	void followMatrix(const MVector &planeNormal, double height, bool backstroke ,
	                  MMatrix &mat) const;

	void flatMatrix(const MVector &planeNormal, double height, bool backstroke,
	                MMatrix &mat) const  ;

	MMatrix matrix(const MVector &planeNormal, double height, bool backstroke,
	               bool follow) const ;

	MVector tangent() const ;
	MPoint  curvePoint() const ;
	void setCurvePoint(const MPoint &point);
	void offsetBy(const MVector &offset);


	double strokeFraction() const ;
	double curveFraction() const;
	double reverseStrokeFraction() const;
	// void setHeight(double height);
	void setRotation(double tilt, double bank, double twist);

private:
	MVector m_tangent; // tangent on the curve
	MPoint m_curvePoint; // point on the curve
	double m_curveFraction; // normalised length
	double m_strokeFraction; // 0-1
	double m_dist;
	MVector m_brushRotate;	// set from ramps
	// double m_height; // set from ramp
};

#endif

