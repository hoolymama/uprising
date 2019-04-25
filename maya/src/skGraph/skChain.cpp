

#include "skChain.h"




skChain::skChain()
	: m_maxRadius(0.0),
	  m_minRadius(99999999.0)
{

}


skChain::~skChain()
{

}


int skChain::size() const
{
	return points.size();
}

const skPoint &skChain::operator[] (const int index) const
{
	return points[index];
}


void skChain::add(const skPoint &pt)
{
	points.push_back(pt);
	// cerr << "Adding: " << pt.radius << endl;
	if (pt.radius > m_maxRadius) {
		m_maxRadius = pt.radius;
	}
	if (pt.radius < m_minRadius) {
		m_minRadius = pt.radius;
	}
}



void skChain::appendPointsTo(MPointArray &positions) const
{
	std::vector< skPoint >::const_iterator iter;
	for (iter = points.begin(); iter != points.end(); iter++)
	{
		positions.append(MPoint(iter->x, iter->y, 0.0));
	}
}

void skChain::appendPointsTo(MVectorArray &positions) const
{
	std::vector< skPoint >::const_iterator iter;
	for (iter = points.begin(); iter != points.end(); iter++)
	{
		positions.append(MVector(iter->x, iter->y));
	}
}

const float &skChain::maxRadius() const
{
	return m_maxRadius;
}

const float &skChain::minRadius() const
{
	return m_minRadius;
}

// int skChain::getContacts( MDoubleArray &contacts, double maxWidth) const
// {
// 	contacts.clear();
// 	if (maxWidth < 0.0001f) {maxWidth = 0.0001f;}
// 	std::vector< skPoint >::const_iterator iter;

// 	for (iter = points.begin(); iter != points.end(); iter++)
// 	{

// 		contacts.append( fmin( iter->radius / maxWidth, 1.0));
// 	}
// 	return 0;
// }


/*

	We have some brush widths, sorted largest to smallest and want to select
	the most appropriate.

	If the stroke's widest point is bigger than the widest brush, we
	just use the widest brush. The contact will be clamped to 1.
*/

// int skChain::getContacts(
//   MDoubleArray &contacts,
//   const std::vector< std::pair<int, float> > &radii) const
// {
// 	int index = -1;
// 	float radius =  1.0;

// 	if (radii.size())
// 	{
// 		std::vector< std::pair<int, float> >::const_iterator iter;
// 		for (iter = radii.begin(); iter != radii.end(); iter++)
// 		{

// 			if (iter->second < maxRadius && (iter != radii.begin() ))
// 			{
// 				break;
// 			}
// 			index = iter->first;
// 			radius = iter->second;
// 		}
// 	}

// 	std::vector< skPoint >::const_iterator iter;
// 	for (iter = points.begin(); iter != points.end(); iter++)
// 	{
// 		contacts.append( fmin( iter->radius / radius, 1.0));
// 	}
// 	return index;
// }




void skChain::appendRadiiTo( MDoubleArray &radii) const
{
	std::vector< skPoint >::const_iterator iter;
	for (iter = points.begin(); iter != points.end(); iter++)
	{
		radii.append(double(iter->radius));
	}
}

void skChain::appendRadiiTo( MFloatArray &radii) const
{
	std::vector< skPoint >::const_iterator iter;
	for (iter = points.begin(); iter != points.end(); iter++)
	{
		radii.append(iter->radius);
	}
}

void skChain::appendParamsTo( MDoubleArray &params) const
{
	int num = points.size();
	double last = num - 1.0;
	for (int i = 0; i < num; ++i)
	{
		params.append(i / last);
	}
}

void skChain::interpolate(int step, skChain &result) const
{

	step = std::max(1, step);
	if (step == 1) {	result = skChain(*this);}

	int num = points.size();
	int last = num - 1;
	int nSpans = ceil(float(last) / step);
	int nVerts = nSpans + 1;

	for (int i = 0; i < nVerts; ++i)
	{
		float param = (last * i ) / float(nSpans);
		int lindex = floor(param);
		int rindex = ceil(param);
		float weight = (lindex == rindex) ? 1 :  float(rindex) - param;

		result.add(skPoint::lerp(points[lindex], points[rindex], weight));
	}


}

