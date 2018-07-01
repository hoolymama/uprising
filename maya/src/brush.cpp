
#include "brush.h"

Brush::Brush() :
	id(-1),
	width(1.0),
	retention(1.0),
	liftLength(1.0),
	liftHeight(1.0),
	liftBias(0.0)
{}

Brush::Brush(
  short id,
  double width,
  double retention,
  double liftLength,
  double liftHeight,
  double liftBias) :
	id(id),
	width(width),
	retention(retention),
	liftLength(liftLength),
	liftHeight(liftHeight),
	liftBias(liftBias)
{}

Brush::~Brush() {}

