#ifndef _skNode_H
#define _skNode_H

#include <map>
#include <vector>


#include <maya/MFloatMatrix.h>
#include <maya/MFloatVector.h>


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

	MFloatVector operator*(const MFloatMatrix &m) {
		return   MFloatPoint(x, y) * m;
	}

	friend ostream &operator<<(ostream &os, const coord &c)
	{
		os << "[" << c.x << ", " << c.y << ", " << c.z << "]" ;
		return os;
	}
};


class skNode
{
public:
	coord c;
	std::map<coord, skNode * > neighbors;
	skNode(int x, int y, int z, float radius);
	skNode(const coord &rhs, float radius);
	~skNode();

	float radius;
	bool seen;
	bool isEnd() const;

	float facing(const skNode *other1, const skNode *other2) const;
	// float distanceTo(const skNode *other) const;
	int  unseenNeighborCount() const;

	int getUnseenNeighbors(std::vector<skNode *> &result) const ;

};


typedef std::pair < skNode *, int > TWIG;
typedef std::vector < TWIG  > TWIG_CLUSTER;
typedef std::map <   skNode *, TWIG_CLUSTER > CLUSTERS;


#endif