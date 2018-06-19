
#include "brush.h"

Brush::Brush() :
	id(-1),
	width(1.0),
	retention(1.0),
	tilt(0.0),
	bank(0.0),
	twist(0.0),
	liftLength(1.0),
	liftHeight(1.0),
	liftBias(0.0),
	name("unassigned")
{}

Brush::Brush(
  short id,
  double width,
  double retention,
  double liftLength,
  double liftHeight,
  double liftBias,
  const MString &name) :
	id(id),
	width(width),
	retention(retention),
	liftLength(liftLength),
	liftHeight(liftHeight),
	liftBias(liftBias),
	name(name)
{}

Brush::~Brush() {}

