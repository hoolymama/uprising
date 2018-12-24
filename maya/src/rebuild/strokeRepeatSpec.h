
#ifndef _StrokeRepeatSpec_
#define _StrokeRepeatSpec_

#include <maya/MObject.h>
#include <maya/MAngle.h>

class StrokeRepeatSpec {
public:

	// enum Scope { kStroke, kTravelStroke, kCurve };


	StrokeRepeatSpec() {};
	~StrokeRepeatSpec() {};

	int repeats;
	double offset;
	bool mirror;
	bool oscillate;
	double fan;

	int count() const {
		return 1 + (mirror ? repeats * 2 : repeats);
	}
};
#endif

