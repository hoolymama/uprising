
#ifndef _backstroke_
#define _backstroke_

#include <maya/MObject.h>

#include "stroke.h"


class BackStroke: public Stroke {
public:

	BackStroke ();

	~BackStroke();

	void getTravelStrokeFractions(MDoubleArray &result) const override;

	void appendTargets(MMatrixArray &result) const override;

	void appendTangents(MVectorArray &result) const override;

	void appendPoints(MVectorArray &result) const override;

	short direction() const override;

	void  getApproachTargets(MMatrix &startApproach, MMatrix &endApproach) const override;

private:

};


#endif

