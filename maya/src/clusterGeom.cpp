
#include "clusterGeom.h"
#include "errorMacros.h"


// clusterGeom::clusterGeom():
// 	m_strokes(),
// 	m_reason(clusterGeom::kNone),
// 	m_name(),
// 	m_paint(0),
// 	m_brush(0)
// {}

clusterGeom::clusterGeom(
  short brushId,
  short paintId,
  double travelCutoff,
  clusterGeom::Reason reason):
	m_strokes(),
	m_reason(reason),
	m_name(),
	m_brushId(brushId),
	m_paintId(paintId),
	m_travelCutoff(travelCutoff),
	m_travel(0.0)
{}

clusterGeom::clusterGeom(
  const clusterGeom &other):
	m_strokes(),
	m_reason(clusterGeom::kDip),
	m_name(),
	m_brushId(other.brushId()),
	m_paintId(other.paintId()),
	m_travelCutoff(other.travelCutoff()),
	m_travel(0.0)
{}

clusterGeom::~clusterGeom() {}


bool clusterGeom::ranOutOfPaint() const {
	return m_travel > m_travelCutoff;
}

double clusterGeom::travelCutoff() const {
	return m_travelCutoff;
}

// void clusterGeom::setPaint(Paint *p) {
// 	m_paint = p;
// }

// void clusterGeom::setBrush( Brush *p) {
// 	m_brush = p;
// }

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


/* Is this assignment the same as default. If so remove it. */
clusterGeom &clusterGeom::operator=( const clusterGeom &other )
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




ostream &operator<<(ostream &os, const clusterGeom &g)
{
	MString reason;
	if (g.m_reason == clusterGeom::kTool) {
		reason = "Tool";
	}
	else if (g.m_reason == clusterGeom::kDip) {
		reason = "Dip";
	}
	else {
		reason = "NONE!!";
	}

	os << "Reason: " <<  g.m_reason << "\n";
	os << "TravelCutoff: " <<  g.m_travelCutoff << "\n";
	os << "Travel: " <<  g.m_travel << "\n";
	os << "Brush Id: " <<  g.m_brushId << "\n";
	os << "Paint Id: " <<  g.m_paintId << "\n";

	os << g.m_strokes.size() << " Strokes\n" ;
	for (auto const &stroke : g.m_strokes) {
		os << stroke;
	}

	return os;
}






// double clusterGeom::maxTravel() const {
// 	return
// }


// const MMatrix &clusterGeom::startApproach() const {
// 	return m_startApproach;
// }
// const MMatrix &clusterGeom::endApproach() const {
// 	return m_endApproach;
// }
// const MMatrixArray &clusterGeom::targets() const {
// 	return m_targets;
// }
// const MVectorArray &clusterGeom::tangents() const {
// 	return m_tangents;
// }
// double clusterGeom::arcLength() const {
// 	return m_arcLength;
// }
// short clusterGeom::direction() const {
// 	return m_direction;
// }
