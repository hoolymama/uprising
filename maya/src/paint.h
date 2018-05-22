
#ifndef _paint_
#define _paint_

#include <maya/MColor.h>
#include <maya/MString.h>
#include <maya/MVector.h>

class Paint {

public:
	Paint();

	Paint( short id,
	       MColor color,
	       double opacity,
	       MString name);

	~Paint();

	// MVector colorAsVector() const;

	short id;
	MColor color;
	double opacity;
	MString name;
};

#endif