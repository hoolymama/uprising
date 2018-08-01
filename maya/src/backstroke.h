
#ifndef _backstroke_
#define _backstroke_

#include <maya/MObject.h>

#include "stroke.h"


class BackStroke: public Stroke {
public:
	BackStroke ();
	~BackStroke();

	void travelStrokeFractions(MDoubleArray &result) const;

	void appendTargets(const MVector &planeNormal, MMatrixArray &result) const ;

	void appendTangents(MVectorArray &result) const ;

	void appendPoints(MVectorArray &result) const ;

private:

};


#endif

