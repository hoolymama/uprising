
#ifndef _brush_
#define _brush_

#include <map>
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
	  double tip,
	  Shape shape
	);

	~Brush();


	static std::map<short, Brush>  factory(
	  MArrayDataHandle &ha,
	  MObject &widthAttribute,
	  MObject &retentionAttribute,
	  MObject &tipAttribute,
	  MObject &shapeAttribute);


	friend ostream &operator<<(ostream &os, const Brush &b);


	short id;
	double width;
	double retention;
	Shape shape;
	double tip;

};
#endif

