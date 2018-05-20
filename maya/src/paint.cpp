#include <maya/MColor.h>
#include <maya/MString.h>

struct paint {


	paint() :
		color() ,
		viscosity(1),
		name("paint")
	{}

	paint(MColor color,
	      double viscosity,
	      MString name) :
		color(color) ,
		viscosity(viscosity),
		name(name)
	{}

	~paint() {}

	MColor color;
	double viscosity;
	MString name;
};
