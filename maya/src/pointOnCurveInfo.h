#ifndef pointOnCurveInfo_H
#define pointOnCurveInfo_H

#include <maya/MIOStream.h>
#include <math.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>


#include <maya/MFnNurbsCurve.h>
#include <maya/MFnMesh.h>


#include "errorMacros.h"
#include "mayaMath.h"

using namespace std;


class pointOnCurveInfo {
public:
	// pointOnCurveInfo(const MPoint &pt, const MVector &tangent,  double param,  double dist,
	//                  bool inside);

	pointOnCurveInfo(
	  const MFnNurbsCurve &curveFn,
	  MFnMesh &meshFn,
	  MMeshIsectAccelParams &ap,
	  double dist);

	~pointOnCurveInfo();


	const MPoint &point() const ;
	const MVector &tangent() const ;
	const double 	&distance() const ;
	const double 	&param() const ;
	bool inside() const ;

	MFloatVector asFloatVector() const;

	// void 	setPoint(const MPoint &rhs) ;
	// void 	setTangent(const MVector &rhs) ;
	// void 	setDistance(const double &rhs) ;
	// void 	setParam(const double &rhs) ;

	// void 	setInside(bool rhs) ;


private:

	MPoint m_point;

	MVector m_tangent;

	double m_distance;

	double m_param;

	bool  m_inside;

	bool m_valid;

};

#endif

