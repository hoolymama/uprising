

#include "pearlChain.h"




pearlChain::pearlChain()
	: m_maxRadius(0.0),
	  m_minRadius(99999999.0)
{

}


pearlChain::~pearlChain()
{

}


int pearlChain::size() const
{
	return m_points.size();
}

const pearl &pearlChain::operator[] (const int index) const
{
	return m_points[index];
}


const std::vector< pearl > &pearlChain::points() const
{
	return m_points;
}


void pearlChain::add(const pearl &pt)
{
	m_points.push_back(pt);

	if (pt.radius > m_maxRadius) {
		m_maxRadius = pt.radius;
	}
	if (pt.radius < m_minRadius) {
		m_minRadius = pt.radius;
	}
}

void pearlChain::appendPointsTo(MPointArray &positions) const
{
	std::vector< pearl >::const_iterator iter;
	for (iter = m_points.begin(); iter != m_points.end(); iter++)
	{
		positions.append(MPoint(iter->point));
	}
}

void pearlChain::appendPointsTo(MVectorArray &positions) const
{
	std::vector< pearl >::const_iterator iter;
	for (iter = m_points.begin(); iter != m_points.end(); iter++)
	{
		positions.append(MVector(iter->point));
	}
}

const float &pearlChain::maxRadius() const
{
	return m_maxRadius;
}

const float &pearlChain::minRadius() const
{
	return m_minRadius;
}

void pearlChain::appendRadiiTo( MDoubleArray &radii) const
{
	std::vector< pearl >::const_iterator iter;
	for (iter = m_points.begin(); iter != m_points.end(); iter++)
	{
		radii.append(double(iter->radius));
	}
}

void pearlChain::appendRadiiTo( MFloatArray &radii) const
{
	std::vector< pearl >::const_iterator iter;
	for (iter = m_points.begin(); iter != m_points.end(); iter++)
	{
		radii.append(iter->radius);
	}
}

void pearlChain::appendParamsTo( MDoubleArray &params) const
{
	int num = m_points.size();
	double last = num - 1.0;
	for (int i = 0; i < num; ++i)
	{
		params.append(i / last);
	}
}

void pearlChain::interpolate(int step, pearlChain &result) const
{

	step = std::max(1, step);
	if (step == 1) {	result = pearlChain(*this);}

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

		result.add(pearl::lerp(m_points[lindex], m_points[rindex], weight));
	}


}


 