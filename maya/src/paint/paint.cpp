
#include <maya/MString.h>
#include "paint.h"


Paint::Paint():
m_color(1.0,0.0,1.0,1.0),
m_travel(1.0),
m_name()
{}

Paint::Paint( const MColor &color, float travel, const MString &name)
:Paint(){
	m_color = color;
	m_travel = travel;
	m_name = name;
	

}

Paint::Paint( const MFloatVector &color, float opacity, float travel, const MString &name)
:Paint(){
	m_color = MColor(color.x, color.y, color.z, opacity);
	m_travel = travel;
	m_name = name;
}

Paint::~Paint() {}

const MColor& Paint::color() const {return m_color;};
float Paint::travel() const  {return m_travel;};
const MString& Paint::name() const  {return m_name;};


ostream &operator<<(ostream &os, const Paint &b)
{
	os << " name:" << b.m_name;
	os << " color:" << b.m_color;
	os << " travel:" << b.m_travel;

	return os;
}
