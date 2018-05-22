
#ifndef _brush_
#define _brush_

#include <maya/MVector.h>
#include <maya/MString.h>

class Brush {
public:
	Brush();

	Brush(
	  short id,
	  double width,
	  double tip,
	  const MVector &tcp,
	  const MString &name);

	~Brush();
	double width;
	double tip;
	MVector tcp;
	MString name;
	short id;

};
#endif