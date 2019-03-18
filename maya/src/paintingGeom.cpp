
#include <set>
#include "paintingGeom.h"
#include "errorMacros.h"

const double epsilon = .001;

paintingGeom::paintingGeom()
	: m_clusters(),
	  m_paints(),
	  m_brushes()
{}

paintingGeom::~paintingGeom() {}



void paintingGeom::setPaints(const std::map<int, Paint> &paints) {
	m_paints = paints;
}

void paintingGeom::setBrushes(const std::map<int, Brush> &brushes) {
	m_brushes = brushes;
}

const std::vector<clusterGeom> &paintingGeom::clusters() const
{
	return m_clusters;
}

const std::map<int, Paint>    &paintingGeom::paints() const
{
	return m_paints;
}

const std::map<int, Brush>    &paintingGeom::brushes() const
{
	return m_brushes;
}

const Brush &paintingGeom::brushFromId(int id) const {

	std::map<int, Brush>::const_iterator iter = m_brushes.find(id);
	// const Brush *p;
	if (iter != m_brushes.end()) {
		return (iter->second);
	}
	return ((m_brushes.find(-1))->second);

}

const Paint &paintingGeom::paintFromId(int id) const {
	std::map<int, Paint>::const_iterator iter = m_paints.find(id);
	// const Paint *p;
	if (iter != m_paints.end()) {
		return (iter->second);
	}
	return ((m_paints.find(-1))->second);
}


double paintingGeom::travelCutoff(  int brushId, int paintId) const {
	const  Brush &b = brushFromId(brushId);
	const  Paint &p = paintFromId(paintId);
	double cutoff =   b.retention() * p.travel;
	if (cutoff < epsilon) {
		cutoff = epsilon;
	}
	return cutoff;
}


clusterGeom &paintingGeom::prepCluster(
  int brushId,
  int physicalId,
  int paintId)
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


	int lastBrushId = back.brushId();
	int lastPaintId = back.paintId();

	// CHANGE PAINT OR BRUSH (tool change)
	if (! (lastBrushId == brushId && lastPaintId == paintId)) {
		double cutoff = travelCutoff(brushId, paintId);
		clusterGeom::Reason reason = clusterGeom::kTool;
		if (lastPaintId == paintId) { // brush Id changed - but is it the same physical brush?
			int lastPhysicalId =  brushFromId(lastBrushId).physicalId();
			if (lastPhysicalId == physicalId) {
				reason = clusterGeom::kTcp;
			}
		}
		m_clusters.push_back(
		  clusterGeom(brushId, paintId, cutoff, reason)
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

	if (back.ranOutOfPaint()) {
		// cerr << "RAN OUT PAINT  (dip only)" << endl;
		m_clusters.push_back( clusterGeom(
		                        back.brushId(),
		                        back.paintId(),
		                        back.travelCutoff(),
		                        clusterGeom::kDip)
		                    );
	}

	return m_clusters.back();
}


void paintingGeom::setApproaches(double approachStart, double approachMid,
                                 double approachEnd, double ptpThresh)
{
	std::vector<clusterGeom>::iterator iter;
	for (iter = m_clusters.begin(); iter != m_clusters.end(); iter++)
	{
		iter->setApproaches(approachStart, approachMid, approachEnd, ptpThresh);
	}
}


void paintingGeom::dipCombinations(MIntArray &result) const
{
	std::set<std::pair <int, int> > combos;
	std::vector<clusterGeom>::const_iterator iter;
	for (iter = m_clusters.begin(); iter != m_clusters.end(); iter++)
	{
		std::pair <int, int> combo (iter->brushId(), iter->paintId()); ;
		combos.insert(combo);
	}
	for (std::set<std::pair <int, int> >::const_iterator citer = combos.begin();
	     citer != combos.end(); citer++)
	{
		result.append(citer->first);
		result.append(citer->second);
	}
}

// void paintingGeom::addStrokeCurve(const strokeCurveGeom &strokeCurve) {

// 	const std::vector<Stroke> &strokes = strokeCurve.strokes();

// 	std::vector<Stroke>::const_iterator citer;

// 	unsigned i = 0;
// 	for (citer = strokes.begin(); citer != strokes.end(); citer++) {
// 		short paintId = citer->paintId();
// 		short brushId = citer->brushId();
// 		bool force = (citer == strokes.begin()) && strokeCurve.forceDip();
// 		clusterGeom &g = prepCluster(force, brushId , paintId );
// 		g.pushStroke(*citer);
// 	}
// }


void paintingGeom::addStroke(const Stroke &stroke, int parentIndex) {

	// short paintId = stroke.paintId();
	int brushId = stroke.brushId();
	const  Brush &b = brushFromId(brushId);
	// short physicalId = b.physicalId;
	// bool force = stroke.forceDip();
	clusterGeom &g = prepCluster(brushId , b.physicalId(), stroke.paintId() );
	g.pushStroke(stroke, parentIndex);

}



void paintingGeom::displace( MFnMesh &meshFn,  MMeshIsectAccelParams &ap )
{
	std::vector<clusterGeom>::iterator iter;
	for (iter = m_clusters.begin(); iter != m_clusters.end(); iter++)
	{
		iter->displace(meshFn, ap);
	}
}


void paintingGeom::offsetBrushContact()
{
	std::vector<clusterGeom>::iterator iter;
	for (iter = m_clusters.begin(); iter != m_clusters.end(); iter++)
	{
		const Brush &brush =  brushFromId(iter->brushId());
		iter->offsetBrushContact(brush);
	}
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





// ostream &operator<<(ostream &os, const paintingGeom &g)
// {
// 	os << "--------PAINTING GEOM---------------------\n" ;
// 	os << g.m_brushes.size() << "BRUSHES: [\n" ;
// 	for (auto const &brush : g.m_brushes) {
// 		os << brush.first << ": " << brush.second << ",\n";
// 	}
// 	os << "],\n";
// 	os << g.m_paints.size() << " PAINTS: [\n" ;
// 	for (auto const &paint : g.m_paints) {
// 		os << paint.first << ":" << paint.second << ",\n";
// 	}
// 	os << "],\n";
// 	os  << g.m_clusters.size() << " CLUSTERS: [\n" ;
// 	for (auto const &cluster : g.m_clusters) {
// 		os << cluster << ",\n";
// 	}
// 	os << "],\n";
// 	os << endl;
// 	// std::map<int, Brush>::const_iterator cbiter = g.m_brushes.begin();
// 	// for (; cbiter !=  g.m_brushes.end(), cbiter++) {
// 	// 	// os << *cbiter;
// 	// }
// 	// for (auto const &brush : g.m_brushes)
// 	// {
// 	// 	os << *brush;
// 	// }

// 	// for (auto const &paint : g.m_paints)
// 	// {
// 	// 	os << *paint;
// 	// }

// 	return os;
// }

