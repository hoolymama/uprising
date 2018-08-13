#ifndef _strokeGeom
#define _strokeGeom


#include <vector>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>
#include "stroke.h"

class strokeGeom
{
public:
	strokeGeom();

	strokeGeom(
	  const Stroke &src,
	  const MVector &planeNormal);

	~strokeGeom();

	const MMatrix &startApproach() const ;
	const MMatrix &endApproach() const ;
	const MMatrixArray &targets() const ;
	const MVectorArray &tangents() const ;
	double arcLength() const ;
	short direction() const ;  // 1 or -1

	strokeGeom &operator=( const strokeGeom &other );

	friend ostream &operator<<(ostream &os, const strokeGeom &geom);

private:

	MMatrix m_startApproach;
	MMatrix m_endApproach;
	MMatrixArray m_targets;
	MVectorArray m_tangents;
	double m_arcLength;
	short m_direction; // 1 or -1

};


#endif