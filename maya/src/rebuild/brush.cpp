#include <map>



#include <maya/MString.h>
#include "brush.h"



Brush::Brush() :
	id(-1),
	physicalId(-1),
	width(1.0),
	retention(1.0),
	tip(0),
	shape(Brush::kRound),
	customId(-1)
{}

Brush::Brush(
  short id,
  short physicalId,
  double width,
  double retention,
  double tip,
  Shape shape,
  short customId,
  double transHeight,
  double transPower) :
	id(id),
	physicalId(physicalId),
	width(width),
	retention(retention),
	tip(tip),
	shape(shape),
	customId(customId),
	transHeight(transHeight * tip),
	transPower(transPower)

{}

Brush::~Brush() {}

std::map<short, Brush> Brush::factory(
  MArrayDataHandle &ha,
  MObject &widthAttribute,
  MObject &retentionAttribute,
  MObject &tipAttribute,
  MObject &physicalIdAttribute,
  MObject &shapeAttribute,
  MObject &customIdAttribute,
  MObject &transHeightAttribute ,
  MObject &transPowerAttribute)
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
		short physicalId = hComp.child(physicalIdAttribute).asShort();
		double width =  hComp.child(widthAttribute).asDouble() ;
		double retention = hComp.child(retentionAttribute).asDouble() ;
		double tip = hComp.child(tipAttribute).asDouble() ;
		Shape shape = Shape(hComp.child(shapeAttribute).asShort()) ;
		short customId = hComp.child(customIdAttribute).asShort();

		double transHeight = hComp.child(transHeightAttribute).asDouble();
		double transPower = hComp.child(transPowerAttribute).asDouble();

		result[index] = Brush(
		                  index,
		                  physicalId,
		                  width,
		                  retention,
		                  tip,
		                  shape,
		                  customId,
		                  transHeight,
		                  transPower);
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
	os << " physicalId:" << b.physicalId;
	os << " customId:" << b.physicalId;

	os << " shape:" << shapeStr;
	return os;
}

