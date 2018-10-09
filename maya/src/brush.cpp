#include <map>



#include <maya/MString.h>
#include "brush.h"



Brush::Brush() :
	id(-1),
	width(1.0),
	retention(1.0),
	tip(0),
	shape(Brush::kRound)
{}

Brush::Brush(
  short id,
  double width,
  double retention,
  double tip,
  Shape shape) :
	id(id),
	width(width),
	retention(retention),
	tip(tip),
	shape(shape)
{}

Brush::~Brush() {}

std::map<short, Brush> Brush::factory(
  MArrayDataHandle &ha,
  MObject &widthAttribute,
  MObject &retentionAttribute,
  MObject &tipAttribute,
  MObject &shapeAttribute)
{
	MStatus st;
	std::map<short, Brush> result;

	result[-1] = Brush();

	unsigned nPlugs = ha.elementCount();
	for (unsigned i = 0; i < nPlugs; i++, ha.next()) {
		short index = short(ha.elementIndex(&st));
		if (st.error()) {
			continue;
		}
		MDataHandle hComp = ha.inputValue(&st );
		if (st.error()) {
			continue;
		}

		double width =  hComp.child(widthAttribute).asDouble() ;
		double retention = hComp.child(retentionAttribute).asDouble() ;
		double tip = hComp.child(tipAttribute).asDouble() ;

		Shape shape = Shape(hComp.child(shapeAttribute).asShort()) ;

		result[index] = Brush(index, width, retention, tip, shape);
	}
	return result;
}




ostream &operator<<(ostream &os, const Brush &b)
{

	MString shapeStr  =  "flat";
	if (b.shape == Brush::kRound) {
		shapeStr = "round";
	}
	os << " id:" << b.id;
	os << " width:" << b.width;
	os << " retention:" << b.retention;
	os << " tip:" << b.tip;
	os << " shape:" << shapeStr;
	return os;
}

