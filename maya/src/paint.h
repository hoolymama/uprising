
#ifndef _paint_
#define _paint_
#include <map>
#include <maya/MColor.h>
#include <maya/MString.h>
#include <maya/MVector.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MObject.h>

class Paint {

public:

	static std::map<short, Paint> factory(
	  MArrayDataHandle &ha,
	  MObject &colorAttribute,
	  MObject &opacityAttribute,
	  MObject &travelAttribute);

	Paint();

	Paint( short id,
	       MColor color,
	       double travel);

	~Paint();

	friend ostream &operator<<(ostream &os, const Paint &p);

	short id;
	MColor color;
	double travel;
};

#endif