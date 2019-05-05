

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
	return m_points.size();
}

const skPoint &skChain::operator[] (const int index) const
{
	return m_points[index];
}


const std::vector< skPoint > &skChain::points() const
{
	return m_points;
}


void skChain::add(const skPoint &pt)
{
	m_points.push_back(pt);
	// cerr << "Adding: " << pt.radius << endl;
	if (pt.radius > m_maxRadius) {
		m_maxRadius = pt.radius;
	}
	if (pt.radius < m_minRadius) {
		m_minRadius = pt.radius;
	}
}


// void skChain::extend(float entryLength, float exitLength)
// {
// 	/*
// 	insert a point at the beginning and at the end.
// 	*/
// 	// int len = m_points.size();
// 	// float dx = m_points[0].x - m_points[1].x;
// 	// float dy = m_points[0].y - m_points[1].y;
// 	// float hyp = sqrt()
// 	skPoint p0, p1;
// 	p1 = m_points.begin(); // 0
// 	p0 = std::next(fromIt); // 1
// 	m_points.insert(skPoint::extrapolate(p0, p1, entryLength));



// 	skPoint toIt = std::prev(m_points.end()); // -1
// 	skPoint fromIt = std::prev(toIt); // -2

// 	m_points.insert(point.begin())




// 	// inserts 3 at front
// 	auto it = vec.insert(vec.begin(), 3);


// }

void skChain::appendPointsTo(MPointArray &positions) const
{
	std::vector< skPoint >::const_iterator iter;
	for (iter = m_points.begin(); iter != m_points.end(); iter++)
	{
		positions.append(MPoint(iter->x, iter->y, 0.0));
	}
}

void skChain::appendPointsTo(MVectorArray &positions) const
{
	std::vector< skPoint >::const_iterator iter;
	for (iter = m_points.begin(); iter != m_points.end(); iter++)
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

void skChain::appendRadiiTo( MDoubleArray &radii) const
{
	std::vector< skPoint >::const_iterator iter;
	for (iter = m_points.begin(); iter != m_points.end(); iter++)
	{
		radii.append(double(iter->radius));
	}
}

void skChain::appendRadiiTo( MFloatArray &radii) const
{
	std::vector< skPoint >::const_iterator iter;
	for (iter = m_points.begin(); iter != m_points.end(); iter++)
	{
		radii.append(iter->radius);
	}
}

void skChain::appendParamsTo( MDoubleArray &params) const
{
	int num = m_points.size();
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

	int num = m_points.size();
	int last = num - 1;
	int nSpans = ceil(float(last) / step);
	int nVerts = nSpans + 1;

	for (int i = 0; i < nVerts; ++i)
	{
		float param = (last * i ) / float(nSpans);
		int lindex = floor(param);
		int rindex = ceil(param);
		float weight = (lindex == rindex) ? 1 :  float(rindex) - param;

		result.add(skPoint::lerp(m_points[lindex], m_points[rindex], weight));
	}


}




// int skChain::getContacts( MDoubleArray &contacts, double maxWidth) const
// {
// 	contacts.clear();
// 	if (maxWidth < 0.0001f) {maxWidth = 0.0001f;}
// 	std::vector< skPoint >::const_iterator iter;

// 	for (iter = m_points.begin(); iter != m_points.end(); iter++)
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
// 	for (iter = m_points.begin(); iter != m_points.end(); iter++)
// 	{
// 		contacts.append( fmin( iter->radius / radius, 1.0));
// 	}
// 	return index;
// }





