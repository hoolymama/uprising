
#ifndef _brush_
#define _brush_

#include <maya/MVector.h>
#include <maya/MString.h>
#include "mayaMath.h"


class Brush {
public:
	Brush();

	Brush(
	  short id,
	  double width,
	  double retention,
	  double liftLength,
	  double liftHeight,
	  double liftBias,
	  const MString &name
	);

	~Brush();

	short id;
	double width;
	double retention;
	double tilt;
	double bank;
	double twist;
	double liftLength;
	double liftHeight;
	double liftBias;
	MString name;

};
#endif

