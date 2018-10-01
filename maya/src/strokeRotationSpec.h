
#ifndef _StrokeRotationSpec_
#define _StrokeRotationSpec_

#include <maya/MObject.h>
#include <maya/MAngle.h>

class StrokeRotationSpec {
public:

	enum Scope { kStroke, kTravelStroke, kCurve };


	StrokeRotationSpec() {};
	~StrokeRotationSpec() {};

	MObject tiltRampAtt;
	MObject bankRampAtt;
	MObject twistRampAtt;
	double tiltRampMin;
	double tiltRampMax;
	double bankRampMin;
	double bankRampMax;
	double twistRampMin;
	double twistRampMax;
	Scope rampScope;
	bool followStroke;

};
#endif

