#ifndef _skNode_H
#define _skNode_H

#include <map>
#include <vector>

#include <maya/MVector.h>

struct coord {
	int x, y, z;

	coord(int ox, int oy = 0, int oz = 0)
		: x(ox), y(oy), z(oz) {	}

	bool operator==(const coord &o) const
	{
		return x == o.x && y == o.y && z == o.z;
	}

	bool operator<(const coord &o) const
	{
		if (z < o.z) { return true; }
		if (z > o.z) { return false; }
		if (y < o.y) { return true; }
		if (y > o.y) { return false; }
		if (x < o.x) { return true; }
		return false;
	}

	bool valid(int w, int h) const
	{
		if (x < 0)   { return false; }
		if (x >= w)  { return false; }
		if (y < 0)   { return false; }
		if (y >= h ) { return false; }
		return true;
	}

	coord offset(const coord &other) const
	{
		return coord( x + other.x,  y + other.y);
	}


	MVector toUV(int w, int h) const
	{
		return MVector(
		         (x + 0.5) / double(w),
		         (1.0 - ((y + 0.5) / double(h))),
		         double(z)
		       );
	}



};


class skNode
{
public:
	coord c;
	std::map<coord, skNode * > neighbors;
	skNode(int x, int y, int z = 0);
	skNode(const coord &rhs);
	~skNode();

	float radius;


	float facing(const skNode *other1, const skNode *other2) const;
};


#endif