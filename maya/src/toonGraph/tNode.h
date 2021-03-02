#ifndef _tNode_H
#define _tNode_H

#include <map>
#include <vector>

#include <maya/MPoint.h>


struct tcoord {
	int vid, layer;

	tcoord(int v, int l = 0)
		: vid(v), layer(l) {	}

	bool operator==(const tcoord &other) const
	{
		return (vid == other.vid) && (layer == other.layer);
	}

	bool operator<(const tcoord &other) const
	{
		if (layer < other.layer) { return true; }
		if (layer > other.layer) { return false; }
		if (vid < other.vid) { return true; }
		return false;
	}
 
	tcoord offset(const tcoord &other) const
	{
		return tcoord( vid + other.vid);
	}
 
	friend ostream &operator<<(ostream &os, const tcoord &c)
	{
		os << "[" << c.vid << ", " << c.layer <<"]" ;
		return os;
	}
};


class tNode
{
public:
	tcoord c;
	std::map<tcoord, tNode * > neighbors;
	tNode(const tcoord &rhs, const MFloatPoint &p);
	~tNode();

	MFloatPoint point;
	bool seen;
	bool isEnd() const;
	int neighborCount() const;
	float straightness(const tNode *a, const tNode *b) const;
 	int  unseenNeighborCount() const;

	int getUnseenNeighbors(std::vector<tNode *> &result) const ;
	friend ostream &operator<<(ostream &os, const tNode &t);

};


#endif