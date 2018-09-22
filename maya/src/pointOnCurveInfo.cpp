#include <maya/MFloatVector.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MFloatPoint.h>
#include "pointOnCurveInfo.h"
#include "errorMacros.h"
// const double TIMESTEP = 1.0f / 24.0f;
// const double EPSILON = 0.0000001;

pointOnCurveInfo::pointOnCurveInfo(
  const MFnNurbsCurve &curveFn,
  MFnMesh &meshFn,
  MMeshIsectAccelParams &ap,
  double dist)
	: m_distance(dist),
	  m_inside(false),
	  m_valid(false),
	  m_point(),
	  m_tangent(),
	  m_param()
{
	MStatus st;
	MFloatVector rayDirection(MFloatVector::zAxis);
	bool hit;
	MFloatPointArray hitPoints;
	bool inside = false;
	m_param = curveFn.findParamFromLength(m_distance, &st);
	if (st.error()) {
		return;
	}

	curveFn.getPointAtParam(m_param, m_point, MSpace::kWorld);
	m_tangent = curveFn.tangent(m_param).normal();
	MFloatPoint rayOrigin(m_point);
	hit = meshFn.allIntersections(
	        rayOrigin, rayDirection,
	        0, 0, false, MSpace::kWorld, 99999999.0f, false,
	        &ap, false, hitPoints, 0, 0, 0, 0, 0);
	if (hit) {
		m_inside = ((hitPoints.length() % 2) == 1);
	}
}



pointOnCurveInfo::~pointOnCurveInfo() {}

const MPoint   &pointOnCurveInfo::point() const {return m_point; }
MFloatVector  pointOnCurveInfo::asFloatVector() const {
	return MFloatVector(float(m_point.x), float(m_point.y), float(m_point.z));
}

const MVector &pointOnCurveInfo::tangent() const {return m_tangent; }
const double 	&pointOnCurveInfo::distance() const {return m_distance; }
const double 	&pointOnCurveInfo::param() const {return m_param; }
bool pointOnCurveInfo::inside() const {return m_inside; }


