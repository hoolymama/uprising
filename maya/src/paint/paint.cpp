
#include <maya/MString.h>
#include "paint.h"


Paint::Paint():
m_color(1.0,0.0,1.0,1.0),
m_travel(1.0),
m_pot(0)
{}

Paint::Paint( const MColor &color, float travel, int pot)
:Paint(){
	m_color = color;
	m_travel = travel;
	m_pot = pot;
}

Paint::Paint( const MFloatVector &color, float opacity, float travel , int pot)
:Paint(){
	m_color = MColor(color.x, color.y, color.z, opacity);
	m_travel = travel;
	m_pot = pot;
}

Paint::~Paint() {}

const MColor& Paint::color() const {return m_color;};
int Paint::pot() const  {return m_pot;};
float Paint::travel() const  {return m_travel;};

ostream &operator<<(ostream &os, const Paint &b)
{
	os << " color:" << b.m_color;
	os << " pot:" << b.m_pot;
	os << " travel:" << b.m_travel;
	return os;
}
