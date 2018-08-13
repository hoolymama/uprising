
#ifndef _brush_
#define _brush_

#include <maya/MDataBlock.h>
#include <maya/MVector.h>
#include "mayaMath.h"


class Brush {
public:

	enum Shape { kFlat, kRound };

	Brush();

	Brush(
	  short id,
	  double width,
	  double retention,
	  Shape shape
	);

	~Brush();


	static std::map<short, Brush>  factory(
	  MArrayDataHandle &ha,
	  MObject &widthAttribute,
	  MObject &retentionAttribute,
	  MObject &shapeAttribute);

	short id;
	double width;
	double retention;
	Shape shape;

};
#endif

