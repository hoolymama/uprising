
#include <algorithm>    // std::max

#include "cImgUtils.h"


void cImgUtils::toImageCoords(
  const MFloatPoint &point,
  const MFloatMatrix &inverseProjection,
  int w, int h,  float &x, float &y, bool clamp)
{
	// puts p  in -1 -> 1
	MFloatPoint p = point * inverseProjection;

	x = ((p.x * 0.5f) + 0.5f) * w;
	y = (1.0f - ((p.y * 0.5f) + 0.5f)) * h;
	if (clamp) {
		x = std::max(0.0f, std::min(x, float(w)));
		y = std::max(0.0f, std::min(y, float(h)));
	}
}






void cImgUtils::toImageCoords(float u, float v, int w, int h,  float &x, float &y)
{
	u = std::max(0.0f, std::min(u, 1.0f));
	v = std::max(0.0f, std::min(v, 1.0f));
	x = u * w;
	y = (1.0 - v) * h;
}








