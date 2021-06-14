
#ifndef _StrokeRepeatSpec_
#define _StrokeRepeatSpec_

#include <maya/MObject.h>
#include <maya/MAngle.h>

class StrokeRepeatSpec
{
public:
	// enum Scope { kStroke, kTravelStroke, kCurve };

	StrokeRepeatSpec(){};
	~StrokeRepeatSpec(){};

	int repeats;
	double offset;
	bool mirror;
	bool oscillate;
	double fan;

	int seed;
	double probability;
	double angleOffset;
	double tangentOffset;
	double normalOffset;

	double calcFan(int j) const
	{

		return (fan * (j + 1)) + (((drand48() * 2) - 1) * angleOffset);
	}

	void calcOffsets(int j, double &tOffset, double &nOffset) const
	{

		tOffset = ((drand48() * 2) - 1) * tangentOffset;

		nOffset = (offset * (j + 1)) + (((drand48() * 2) - 1) * normalOffset);
		return;
	}
};
#endif
