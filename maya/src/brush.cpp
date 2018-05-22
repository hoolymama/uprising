
#include "brush.h"

Brush::Brush() :
	id(-1),
	width(1) ,
	tip(0),
	tcp(),
	name("unassigned") {}

Brush::Brush(
  short id,
  double width,
  double tip,
  const MVector &tcp,
  const MString &name) :
	id(id),
	width(width) ,
	tip(tip),
	tcp(tcp),
	name(name)
{}

Brush::~Brush() {}

