
#include "strokeCurveGeom.h"
#include "errorMacros.h"

strokeCurveGeom::strokeCurveGeom():
	m_strokes()
	// m_forceDip(false)

	// ,
	// m_brushId(0),
	// m_paintId(0)
{}

strokeCurveGeom::~strokeCurveGeom() {}

const std::vector<strokeGeom> &strokeCurveGeom::strokes() const {
	return m_strokes;
}

// short strokeCurveGeom::brushId() const {
// 	return m_brushId;
// }
// short strokeCurveGeom::paintId() const {
// 	return m_paintId;
// }

// bool strokeCurveGeom::forceDip() const {
// 	return m_forceDip;
// }



void strokeCurveGeom::create(
  const std::vector<std::unique_ptr<Stroke> > &strokes,
  bool forceDip,
  short brushId,
  short paintId)
{
	// m_forceDip = forceDip;
	bool first = true;
	for (auto &citer : strokes)
	{
		bool doForce = forceDip && first;
		m_strokes.push_back(strokeGeom(*citer, brushId, paintId, doForce));
	}
}

// ostream &operator<<(ostream &os, const strokeCurveGeom &g)
// {
// 	return os;
// }

