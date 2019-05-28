
#include "clusterGeom.h"
#include "errorMacros.h"


clusterGeom::clusterGeom(
  short brushId,
  short paintId,
  double travelCutoff,
  clusterGeom::Reason reason):
	m_strokes(),
	m_reason(reason),
	m_brushId(brushId),
	m_paintId(paintId),
	m_travelCutoff(travelCutoff),
	m_travel(0.0)
{}

clusterGeom::~clusterGeom() {}


bool clusterGeom::ranOutOfPaint() const {
	return m_travel > m_travelCutoff;
}

double clusterGeom::travelCutoff() const {
	return m_travelCutoff;
}

void clusterGeom::pushStroke(const Stroke &s, int parentIndex) {
	m_strokes.push_back(s);
	m_strokes.back().setParentId(parentIndex);
	m_travel += s.arcLength() * s.paintFlow();
}


short clusterGeom::paintId() const {
	return m_paintId;
}

short clusterGeom::brushId() const {
	return m_brushId;
}

clusterGeom::Reason clusterGeom::reason() const {
	return m_reason;
}

double clusterGeom::travel() const {
	return m_travel;
}

const std::vector<Stroke> &clusterGeom::strokes() const {
	return m_strokes;
}

void clusterGeom::setDeparture(double approachMid, double approachEnd)
{
	for (auto iter = m_strokes.begin(); iter != m_strokes.end(); iter++) {
		if (std::next(iter) == m_strokes.end()) { // last
			iter->setDeparture(approachEnd);
		}
		else {
			iter->setDeparture(approachMid);
		}
	}
}

void clusterGeom::setArrival(double approachStart, double approachMid,
                             double ptpThresh)
{
	for (auto iter = m_strokes.begin(); iter != m_strokes.end(); iter++) {
		if (iter == m_strokes.begin())
		{
			iter->setArrival(approachStart);
		}
		else {
			iter->setArrival(approachMid, ptpThresh, *(std::prev(iter)));
		}
	}
}



void  clusterGeom::setApproaches(double approachStart, double approachMid,
                                 double approachEnd, double ptpThresh )
{
	setDeparture(approachMid, approachEnd);
	setArrival(approachStart, approachMid, ptpThresh);
}





void clusterGeom::displace( MFnMesh &meshFn, MMeshIsectAccelParams &ap)
{
	std::vector<Stroke>::iterator iter;
	for (iter = m_strokes.begin(); iter != m_strokes.end(); iter++)
	{
		iter->displace(meshFn, ap);
	}
}

void clusterGeom::offsetBrushContact(const Brush &brush)
{
	std::vector<Stroke>::iterator iter;
	for (iter = m_strokes.begin(); iter != m_strokes.end(); iter++)
	{
		iter->offsetBrushContact( brush);
	}
}

