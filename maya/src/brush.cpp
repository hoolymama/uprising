#include <maya/MVector.h>
#include <maya/MString.h>

struct brush {

	brush() :
		width(1) ,
		tip(0),
		tcp(),
		name("brush") {}

	brush(
	  double width,
	  double tip,
	  const MVector &tcp,
	  const MString &name) :
		width(width) ,
		tip(tip),
		tcp(tcp),
		name(name)
	{}

	~brush() {}

	double distanceAtElevation(double elevation) const {
		return 1;
	}

	double width;
	double tip;
	MVector tcp;
	MString name;

};
