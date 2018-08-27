#ifndef _strokeGeom
#define _strokeGeom


#include <vector>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>
#include "stroke.h"

class strokeGeom
{
public:

	strokeGeom(const Stroke &src);

	~strokeGeom();

	const MMatrix &startApproach() const ;
	const MMatrix &endApproach() const ;
	const MMatrixArray &targets() const ;
	const MVectorArray &tangents() const ;
	const MVector &planeNormal() const;
	double arcLength() const ;
	short direction() const ;  // 1 or -1

	/* These functions return data used for drawing. */
	void getPoints(MFloatPointArray &result, double stackHeight = 0.0) const;
	void getXAxes(MFloatVectorArray &result) const;
	void getYAxes(MFloatVectorArray &result) const;
	void getZAxes(MFloatVectorArray &result) const;
	void getBorders(MFloatPointArray &lefts, MFloatPointArray &rights,
	                double brushWidth, bool withLift = false, double stackHeight = 0.0) const;

	void  getApproaches(MFloatPointArray &startApproachPoints,
	                    MFloatPointArray &endApproachPoints, double stackHeight = 0.0) const;

	void getFullPath(MFloatPointArray &points, double stackHeight = 0.0) const;

	friend ostream &operator<<(ostream &os, const strokeGeom &geom);

private:

	MMatrix m_startApproach;
	MMatrix m_endApproach;
	MMatrixArray m_targets;
	MVectorArray m_tangents;
	double m_arcLength;
	short m_direction; // 1 or -1
	MVector m_planeNormal;

};


#endif