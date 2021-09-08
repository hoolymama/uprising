

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


 