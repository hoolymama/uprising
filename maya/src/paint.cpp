#include "paint.h"

Paint::Paint() :
	id(-1),
	color(1.0, 0.0, 1.0) ,
	opacity(1),
	name("unassigned")
{}

Paint::Paint(
  short id,
  MColor color,
  double opacity,
  MString name) :
	id(id),
	color(color) ,
	opacity(opacity),
	name(name)
{
}

Paint::~Paint() {}