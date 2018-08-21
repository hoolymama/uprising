
#ifndef _backstroke_
#define _backstroke_

#include <maya/MObject.h>

#include "stroke.h"


class BackStroke: public Stroke {
public:

	BackStroke ();

	~BackStroke();

	void travelStrokeFractions(MDoubleArray &result) const;

	void appendTargets(MMatrixArray &result) const ;

	void appendTangents(MVectorArray &result) const ;

	void appendPoints(MVectorArray &result) const ;

	short direction() const;

	void  getApproachTargets(MMatrix &startApproach, MMatrix &endApproach) const;

private:

};


#endif

