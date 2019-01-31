#include <math.h>
#include "skNode.h"

const float sqrt_2 = 1.41421356237;

void norm(float &x, float &y)
{
	if (! (x == 0.0 || y == 0.0))
	{
		x *= 0.7;
		y *= 0.7;
	}
}

skNode::skNode(int x, int y, int z, float radius)
	: c(x, y, z),
	  radius(radius),
	  seen(false),
	  neighbors()
{}

skNode::skNode(const coord &rhs, float radius)
	: c(rhs),
	  radius(radius),
	  seen(false),
	  neighbors()
{}

skNode::~skNode() {}

bool skNode::isEnd() const {
	return neighbors.size() == 1;
}

int skNode::unseenNeighborCount() const {

	int num_unseen = 0;
	std::map<coord, skNode *>::const_iterator citer;
	for (citer = neighbors.begin(); citer != neighbors.end(); citer++)
	{
		// cerr << "In unseenNeighborCount loop over neighbors" << endl;
		// cerr << "citer->second->seen" << citer->second->seen << endl;
		if (! citer->second->seen)
		{
			num_unseen++;
		}
	}
	return num_unseen;
}

int skNode::getUnseenNeighbors(std::vector<skNode *> &result) const {

	result.clear();
	std::map<coord, skNode *>::const_iterator citer;
	for (citer = neighbors.begin(); citer != neighbors.end(); citer++)
	{
		// cerr <<  "unseenNeighbors citer->second " << citer->second << endl;
		// cerr <<  "unseenNeighbors citer->second->seen " << citer->second->seen << endl;
		if (! citer->second->seen)
		{
			result.push_back(citer->second);
		}
	}
	// cerr << "result.size(): " << result.size() << endl;

	return result.size();
}



float skNode::facing(const skNode *other1, const skNode *other2) const
{
	float x1 = (other1->c.x - c.x);
	float y1 = (other1->c.y - c.y);
	float x2 = (other2->c.x - c.x);
	float y2 = (other2->c.y - c.y);
	norm(x1, y1);
	norm(x2, y2);
	return (x1 * x2) + (y1 * y2) ;
}

// float skNode::distanceTo(const skNode *other) const
// {
// /*
// 	This is
// */

// 	float x = (other->c.x - c.x);
// 	float y = (other->c.y - c.y);
// 	if (x == 0 && y == 0) {
// 		return 0.0;
// 	}
// 	if (! (x == 0.0 || y == 0.0))
// 	{
// 		// diagonal
// 		return sqrt_2;
// 	}
// 	// orthogonal
// 	return 1.0;
// }