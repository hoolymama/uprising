
#ifndef _brush_
#define _brush_

#include <maya/MVector.h>
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
	  double liftBias
	);

	~Brush();

	short id;
	double width;
	double retention;
	double liftLength;
	double liftHeight;
	double liftBias;
};
#endif

