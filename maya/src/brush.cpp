#include <map>


#include "brush.h"



Brush::Brush() :
	id(-1),
	width(1.0),
	retention(1.0),
	shape(Brush::kRound)
{}

Brush::Brush(
  short id,
  double width,
  double retention,
  Shape shape) :
	id(id),
	width(width),
	retention(retention),
	shape(shape)
{}

Brush::~Brush() {}

std::map<short, Brush> Brush::factory(
  MArrayDataHandle &ha,
  MObject &widthAttribute,
  MObject &retentionAttribute,
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
		Shape shape = Shape(hComp.child(shapeAttribute).asShort()) ;

		result[index] = Brush(index, width, retention, shape);
	}
	return result;
}
