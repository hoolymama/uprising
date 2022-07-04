
#include <maya/MString.h>
#include "paint.h"


Paint::Paint():
m_color(1.0,0.0,1.0,1.0),
m_travel(1.0),
m_name(),
m_pot(0)
{}

Paint::Paint( const MColor &color, int pot, float travel, const MString &name):
m_color(color),
m_travel(travel),
m_name(name),
m_pot(pot)
{}


Paint::Paint( const MFloatVector &color, float opacity, int pot, float travel, const MString &name):
m_travel(travel),
m_name(name),
m_pot(pot)
{
	m_color.r = color.x;
	m_color.g = color.y;
	m_color.b = color.z;
	m_color.a = opacity;
}


Paint::~Paint() {}

const MColor& Paint::color() const {return m_color;};
float Paint::travel() const  {return m_travel;};
const MString& Paint::name() const  {return m_name;};
int Paint::pot() const {return m_pot;};

ostream &operator<<(ostream &os, const Paint &b)
{
	os << " name:" << b.m_name;
	os << " color:" << b.m_color;
	os << " travel:" << b.m_travel;
	os << " pot:" << b.m_pot;

	return os;
}
