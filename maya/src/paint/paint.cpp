
#include <maya/MString.h>
#include "paint.h"


Paint::Paint():
m_color(1.0,0.0,1.0,1.0),
m_travel(1.0)
{}

Paint::Paint( const MColor &color, float travel)
:Paint(){
	m_color = color;
	m_travel = travel;

}

Paint::Paint( const MFloatVector &color, float opacity, float travel)
:Paint(){
	m_color = MColor(color.x, color.y, color.z, opacity);
	m_travel = travel;

}

Paint::~Paint() {}

const MColor& Paint::color() const {return m_color;};
float Paint::travel() const  {return m_travel;};

ostream &operator<<(ostream &os, const Paint &b)
{
	os << " color:" << b.m_color;
	os << " travel:" << b.m_travel;
	return os;
}
