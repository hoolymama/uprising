
#include "strokeCurveGeom.h"
#include "errorMacros.h"

strokeCurveGeom::strokeCurveGeom():
	m_strokes(),
	m_forceDip(false)

	// ,
	// m_brushId(0),
	// m_paintId(0)
{}

strokeCurveGeom::~strokeCurveGeom() {}

const std::vector<strokeGeom> &strokeCurveGeom::strokes() const {
	return m_strokes;
}

short strokeCurveGeom::brushId() const {
	return m_brushId;
}
short strokeCurveGeom::paintId() const {
	return m_paintId;
}

bool strokeCurveGeom::forceDip() const {
	return m_forceDip;
}



void strokeCurveGeom::create(
  const std::vector<std::unique_ptr<Stroke> > &strokes,
  bool forceDip,
  short brushId,
  short paintId)
{
	m_forceDip = forceDip;
	// m_brushId = brushId;
	// m_paintId = paintId;
	for (auto &citer : strokes)
	{
		m_strokes.push_back(strokeGeom(*citer, brushId, paintId));
	}
}


/* Is this assignment the same as default. If so remove it. */
// strokeCurveGeom &strokeCurveGeom::operator=( const strokeCurveGeom &other )
// {
// 	if ( &other != this ) {
// 		m_strokes = other.strokes();
// 		m_forceDip = other.forceDip();
// 		m_brushId = other.brushId();
// 		m_paintId = other.paintId();
// 	}
// 	return *this;
// }



ostream &operator<<(ostream &os, const strokeCurveGeom &g)
{

	os << "---------  strokeCurveGeom\n";
	os << "Force dip: " <<  g.m_forceDip << "\n";

	os << "Brush Id: " <<  g.m_brushId << "\n";
	os << "Paint Id: " <<  g.m_paintId << "\n";

	return os;
}

