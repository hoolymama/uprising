#include <math.h>
#include "tNode.h"

tNode::tNode(const tcoord &c, const MPoint &p)
	: c(c),
	  point(p),
	  seen(false),
	  neighbors()
{
}

tNode::~tNode() {}

bool tNode::isEnd() const
{
	return neighbors.size() == 1;
}

int tNode::neighborCount() const
{
	return neighbors.size();
}



int tNode::unseenNeighborCount() const
{

	int num_unseen = 0;
	std::map<tcoord, tNode *>::const_iterator citer;
	for (citer = neighbors.begin(); citer != neighbors.end(); citer++)
	{
		if (!citer->second->seen)
		{
			num_unseen++;
		}
	}
	return num_unseen;
}

int tNode::getUnseenNeighbors(std::vector<tNode *> &result) const
{

	result.clear();
	std::map<tcoord, tNode *>::const_iterator citer;
	for (citer = neighbors.begin(); citer != neighbors.end(); citer++)
	{
		if (!citer->second->seen)
		{
			result.push_back(citer->second);
		}
	}
	return result.size();
}

float tNode::straightness(const tNode *a, const tNode *b) const
{
	// returns a value between -1 and 1 where -1 is fully facing
	return  float(((
			(a->point - point).normal() * (point - b->point).normal()
		)+1.0)*0.5);
}


ostream &operator<<(ostream &os, const  tNode &t)
{
	os <<  "C:" << t.c << " P:" << t.point << " seen:"<< t.seen <<  " neighbors:"<< t.neighborCount() << endl;
    
	return os;
}