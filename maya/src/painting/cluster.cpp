
#include "cluster.h"
#include "errorMacros.h"

Cluster::Cluster(
	short brushId,
	short paintId,
	float travelCutoff,
	Cluster::Reason reason)
	: m_strokes(),
	  m_reason(reason),
	  m_brushId(brushId),
	  m_paintId(paintId),
	  m_travelCutoff(travelCutoff),
	  m_travel(0.0)
{
}

Cluster::~Cluster() {}

bool Cluster::ranOutOfPaint() const
{
	return m_travel > m_travelCutoff;
}

float Cluster::travelCutoff() const
{
	return m_travelCutoff;
}

void Cluster::pushStroke(const Stroke &s, int parentIndex)
{
	m_strokes.push_back(s);
	if (parentIndex > -1)
	{
		m_strokes.back().setParentId(parentIndex);
	}
	m_travel += s.calculateArcLength();
}

short Cluster::paintId() const
{
	return m_paintId;
}

short Cluster::brushId() const
{
	return m_brushId;
}

Cluster::Reason Cluster::reason() const
{
	return m_reason;
}

float Cluster::travel() const
{
	return m_travel;
}

const std::vector<Stroke> &Cluster::strokes() const
{
	return m_strokes;
}

void Cluster::setDeparture(float approachMid, float approachEnd)
{
	for (auto iter = m_strokes.begin(); iter != m_strokes.end(); iter++)
	{
		if (std::next(iter) == m_strokes.end())
		{ 
			iter->setDeparture(approachEnd);
		}
		else
		{
			iter->setDeparture(approachMid);
		}
	}
}

void Cluster::setArrival(float approachStart, float approachMid,
						 float ptpThresh)
{
	for (auto iter = m_strokes.begin(); iter != m_strokes.end(); iter++)
	{
		if (iter == m_strokes.begin())
		{
			iter->setArrival(approachStart);
		}
		else
		{
			iter->setArrival(approachMid, ptpThresh, *(std::prev(iter)));
		}
	}
}

void Cluster::setApproaches(
	float approachStart, 
	float approachMid,
	float approachEnd, 
	float ptpThresh)
{
	setDeparture(approachMid, approachEnd);
	setArrival(approachStart, approachMid, ptpThresh);
}

// void Cluster::offsetBrushContact(const Brush &brush)
// {
// 	std::vector<Stroke>::iterator iter;
// 	for (iter = m_strokes.begin(); iter != m_strokes.end(); iter++)
// 	{
// 		iter->offsetBrushContact(brush);
// 	}
// }

// void Cluster::displace(MFnMesh &meshFn, MMeshIsectAccelParams &ap)
// {
// 	std::vector<Stroke>::iterator iter;
// 	for (iter = m_strokes.begin(); iter != m_strokes.end(); iter++)
// 	{
// 		iter->displace(meshFn, ap);
// 	}
// }

// void Cluster::applyBiases(const Brush &brush, float mult)
// {
// 	std::vector<Stroke>::iterator iter;
// 	for (iter = m_strokes.begin(); iter != m_strokes.end(); iter++)
// 	{
// 		iter->applyBiases(brush, mult);
// 	}
// }
