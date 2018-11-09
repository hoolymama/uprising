#include <map>
#include <maya/MDataHandle.h>
#include "paint.h"



std::map<short, Paint> Paint::factory(
  MArrayDataHandle &ha,
  MObject &colorAttribute,
  MObject &opacityAttribute,
  MObject &travelAttribute,
  MObject &customIdAttribute)
{
	MStatus st;
	std::map<short, Paint> result;

	// make a default paint in slot -1;
	result[-1] = Paint();

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

		MFloatVector vc =  hComp.child(colorAttribute).asFloatVector() ;

		double opacity = hComp.child(opacityAttribute).asDouble() ;

		MColor color(vc.x, vc.y, vc.z, opacity);


		double travel = hComp.child(travelAttribute).asDouble() ;
		short customId = hComp.child(customIdAttribute).asShort() ;

		result[index] = Paint(index, color, travel, customId);


		// cerr << result[index] << endl;
	}
	return result;
}


Paint::Paint() :
	id(-1),
	color(1.0, 0.0, 1.0) ,
	travel(10),
	customId(-1)
{}

Paint::Paint(
  short id,
  MColor color,
  double travel,
  short customId) :
	id(id),
	color(color) ,
	travel(travel),
	customId(customId)
{
}

Paint::~Paint() {}

ostream &operator<<(ostream &os, const Paint &p)
{
	os << " id:" << p.id << ",";
	os << " color:" << p.color << ",";
	os << " travel:" << p.travel;
	os << " customId:" << p.customId;

	return os;
}

