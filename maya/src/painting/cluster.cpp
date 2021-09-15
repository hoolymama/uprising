
#include "cluster.h"
#include "errorMacros.h"

Cluster::Cluster(
	short brushId,
	short paintId,
	short potId,
	float travelCutoff,
	Cluster::Reason reason)
	: m_strokes(),
	  m_reason(reason),
	  m_brushId(brushId),
	  m_paintId(paintId),
	  m_potId(potId),
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
short Cluster::potId() const
{
	return m_potId;
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
