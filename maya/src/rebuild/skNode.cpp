#include <math.h>
#include "skNode.h"


void norm(float &x, float &y)
{
	if (! (x == 0.0 || y == 0.0))
	{
		x *= 0.7;
		y *= 0.7;
	}
}

skNode::skNode(int x, int y, int z)
	: c(x, y, z),
	  neighbors()
{}

skNode::skNode(const coord &rhs)
	: c(rhs),
	  neighbors()
{}

skNode::~skNode() {}



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