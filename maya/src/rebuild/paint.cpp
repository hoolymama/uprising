#include <map>
#include <maya/MDataHandle.h>
#include "paint.h"



std::map<int, Paint> Paint::factory(
  MArrayDataHandle &ha,
  MObject &colorAttribute,
  MObject &opacityAttribute,
  MObject &travelAttribute)
{
	MStatus st;
	std::map<int, Paint> result;

	// make a default paint in slot -1;
	result[-1] = Paint();

	unsigned nPlugs = ha.elementCount();
	for (unsigned i = 0; i < nPlugs; i++, ha.next()) {
		int index = ha.elementIndex(&st);
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
		// short customId = hComp.child(customIdAttribute).asShort() ;

		result[index] = Paint(index, color, travel);


		// cerr << result[index] << endl;
	}
	return result;
}


Paint::Paint() :
	id(-1),
	color(1.0, 0.0, 1.0) ,
	travel(10)
{}

Paint::Paint(
  int id,
  MColor color,
  double travel) :
	id(id),
	color(color) ,
	travel(travel)
{
}

Paint::~Paint() {}

ostream &operator<<(ostream &os, const Paint &p)
{
	os << " id:" << p.id << ",";
	os << " color:" << p.color << ",";
	os << " travel:" << p.travel;

	return os;
}

