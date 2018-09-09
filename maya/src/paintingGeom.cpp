
#include "paintingGeom.h"
#include "errorMacros.h"

const double epsilon = .001;

paintingGeom::paintingGeom()
	: m_clusters(),
	  m_paints(),
	  m_brushes()
{}

paintingGeom::~paintingGeom() {}

void paintingGeom::setPaints(const std::map<short, Paint> &paints) {
	m_paints = paints;
}

void paintingGeom::setBrushes(const std::map<short, Brush> &brushes) {
	m_brushes = brushes;
}

const std::vector<clusterGeom> &paintingGeom::clusters() const
{
	return m_clusters;
}

const std::map<short, Paint>    &paintingGeom::paints() const
{
	return m_paints;
}

const std::map<short, Brush>    &paintingGeom::brushes() const
{
	return m_brushes;
}

const Brush &paintingGeom::brushFromId(short id) const {

	std::map<short, Brush>::const_iterator iter = m_brushes.find(id);
	const Brush *p;
	if (iter != m_brushes.end()) {
		return (iter->second);
	}
	return ((m_brushes.find(-1))->second);

}

const Paint &paintingGeom::paintFromId(short id) const {
	std::map<short, Paint>::const_iterator iter = m_paints.find(id);
	// const Paint *p;
	if (iter != m_paints.end()) {
		return (iter->second);
	}
	return ((m_paints.find(-1))->second);
}


double paintingGeom::travelCutoff(  short brushId, short paintId) const {
	const  Brush &b = brushFromId(brushId);
	const  Paint &p = paintFromId(paintId);
	double cutoff =   b.retention * p.travel;
	if (cutoff < epsilon) {
		cutoff = epsilon;
	}
	return cutoff;
}


clusterGeom &paintingGeom::prepCluster(
  bool force,
  short brushId,
  short paintId)
{

	// FIRST CLUSTER
	if (m_clusters.empty()) {
		double cutoff = travelCutoff(brushId, paintId);
		m_clusters.push_back(
		  clusterGeom(brushId, paintId, cutoff, clusterGeom::kTool)
		);
		return m_clusters.back();
	}

	// We got here, so there's at least one cluster and
	// therefore safe to call back()
	const clusterGeom &back = m_clusters.back();

	// CHANGE PAINT OR BRUSH (tool change)
	if (! (back.brushId() == brushId && back.paintId() == paintId)) {
		// cerr << "CHANGE PAINT OR BRUSH (tool change)" << endl;

		double cutoff = travelCutoff(brushId, paintId);
		m_clusters.push_back(
		  clusterGeom(brushId, paintId, cutoff, clusterGeom::kTool)
		);
		return m_clusters.back();
	}

	// RAN OUT PAINT (dip only)
	// FORCE DIP ON FIRST STROKE OF CURVE
	/*
	Since the blocks above have taken care of cases where paint or brush
	changed, we know the paint and brush didn't change here, so we can
	instantiate a cluster from the last cluster in the array.
	*/

	if (back.ranOutOfPaint() or force) {
		// cerr << "RAN OUT PAINT OR FORCE (dip only)" << endl;
		m_clusters.push_back( clusterGeom(
		                        back.brushId(),
		                        back.paintId(),
		                        back.travelCutoff(),
		                        clusterGeom::kDip)
		                    );
	}

	return m_clusters.back();
}

// void paintingGeom::addStrokeCurve(const strokeCurveGeom &strokeCurve) {

// 	const std::vector<strokeGeom> &strokes = strokeCurve.strokes();

// 	std::vector<strokeGeom>::const_iterator citer;

// 	unsigned i = 0;
// 	for (citer = strokes.begin(); citer != strokes.end(); citer++) {
// 		short paintId = citer->paintId();
// 		short brushId = citer->brushId();
// 		bool force = (citer == strokes.begin()) && strokeCurve.forceDip();
// 		clusterGeom &g = prepCluster(force, brushId , paintId );
// 		g.pushStroke(*citer);
// 	}
// }


void paintingGeom::addStroke(const strokeGeom &stroke) {

	short paintId = stroke.paintId();
	short brushId = stroke.brushId();
	bool force = stroke.forceDip();
	clusterGeom &g = prepCluster(force, brushId , paintId );
	g.pushStroke(stroke);

}


// void paintingGeom::addStrokes(const strokeCurveGeom &strokeCurve) {}




// // /* Is this assignment the same as default. If so remove it. */
// paintingGeom &paintingGeom::operator=( const paintingGeom &other )
// {
// 	if ( &other != this ) {
// 		// m_clusters.clear();
// 		// m_paints.clear();
// 		// m_brushes.clear();
// 		// m_clusters = other.clusters();
// 		// m_paints = other.paints();
// 		// m_brushes = other.brushes();
// 		// m_dummies = other.dummies();
// 	}
// 	return *this;
// }





ostream &operator<<(ostream &os, const paintingGeom &g)
{
	os << "--------PAINTING GEOM---------------------\n" ;
	os << g.m_brushes.size() << "BRUSHES: [\n" ;
	for (auto const &brush : g.m_brushes) {
		os << brush.first << ": " << brush.second << ",\n";
	}
	os << "],\n";
	os << g.m_paints.size() << " PAINTS: [\n" ;
	for (auto const &paint : g.m_paints) {
		os << paint.first << ":" << paint.second << ",\n";
	}
	os << "],\n";
	os  << g.m_clusters.size() << " CLUSTERS: [\n" ;
	for (auto const &cluster : g.m_clusters) {
		os << cluster << ",\n";
	}
	os << "],\n";
	os << endl;
	// std::map<short, Brush>::const_iterator cbiter = g.m_brushes.begin();
	// for (; cbiter !=  g.m_brushes.end(), cbiter++) {
	// 	// os << *cbiter;
	// }
	// for (auto const &brush : g.m_brushes)
	// {
	// 	os << *brush;
	// }

	// for (auto const &paint : g.m_paints)
	// {
	// 	os << *paint;
	// }

	return os;
}

