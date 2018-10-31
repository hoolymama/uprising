
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

void clusterGeom::pushStroke(const strokeGeom &s) {
	m_strokes.push_back(s);
	m_travel += s.arcLength();
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

const std::vector<strokeGeom> &clusterGeom::strokes() const {
	return m_strokes;
}

void clusterGeom::setPreStops(double threshold)
{
	std::vector<strokeGeom>::iterator iter;
	iter = m_strokes.begin();
	MMatrix lastA = iter->endApproach();
	MPoint lastPoint(lastA[3][0], lastA[3][1], lastA[3][2]);
	iter++;
	for (; iter != m_strokes.end(); iter++)
	{
		const MMatrix &currentA = iter->startApproach();


		MPoint currentPoint(currentA[3][0], currentA[3][1], currentA[3][2]);

		double dist = lastPoint.distanceTo(currentPoint);

		if (dist > threshold) {
			int num_inbetweens = int(dist / threshold);
			for (int i = 0; i < num_inbetweens; ++i)
			{
				double fraction = (i + 1) / double(num_inbetweens + 1);
				MPoint newPoint((lastPoint * (1.0 - fraction)) + (currentPoint * fraction));
				MMatrix newMat(lastA);
				if (fraction > 0.5) {
					newMat = MMatrix(currentA);
				}
				newMat[3][0] = newPoint.x;
				newMat[3][1] = newPoint.y;
				newMat[3][2] = newPoint.z;
				iter->addPreStop(newMat);
			}
		}
		lastA = iter->endApproach();
		lastPoint = MPoint(lastA[3][0], lastA[3][1], lastA[3][2]);
	}
}




/* Is this assignment the same as default. If so remove it. */
/*clusterGeom &clusterGeom::operator=( const clusterGeom &other )
{
	if ( &other != this ) {
		m_strokes.clear();
		m_strokes = other.strokes();
		m_reason = other.reason();
		m_paintId = other.paintId();
		m_brushId = other.brushId();
		m_travelCutoff = other.travelCutoff();
		m_travel = other.travel();
	}
	return *this;
}
*/
ostream &operator<<(ostream &os, const clusterGeom &g)
{
	MString reason;
	if (g.m_reason == clusterGeom::kTool) {
		reason = "Tool";
	}
	else if (g.m_reason == clusterGeom::kDip) {
		reason = "Dip";
	} if (g.m_reason == clusterGeom::kTcp) {
		reason = "Tcp";
	}
	else {
		reason = "NONE!!";
	}

	os << " Reason:" << reason ;
	os << " TravelCutoff:" <<  g.m_travelCutoff;
	os << " Travel:" <<  g.m_travel;
	os << " Brush Id:" <<  g.m_brushId;
	os << " Paint Id:" <<  g.m_paintId << "\n";

	os << " " << g.m_strokes.size() << " Strokes: [\n" ;
	for (auto const &stroke : g.m_strokes) {
		os << " " << stroke << ",\n";
	}
	os << " ]\n";
	return os;
}

