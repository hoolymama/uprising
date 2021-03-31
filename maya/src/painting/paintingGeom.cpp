
#include <set>
#include "paintingGeom.h"
#include "errorMacros.h"

const float epsilon = 0.0001;

paintingGeom::paintingGeom()
	: m_clusters(),
	  m_paints(),
	  m_brushes()
{
}

paintingGeom::~paintingGeom() {}

void paintingGeom::setPaints(const std::map<int, Paint> &paints)
{
	m_paints = paints;
}

void paintingGeom::setBrushes(const std::map<int, Brush> &brushes)
{
	m_brushes = brushes;
}

const std::vector<Cluster> &paintingGeom::clusters() const
{
	return m_clusters;
}

const std::map<int, Paint> &paintingGeom::paints() const
{
	return m_paints;
}

const std::map<int, Brush> &paintingGeom::brushes() const
{
	return m_brushes;
}

const Brush &paintingGeom::brushFromId(int id) const
{

	std::map<int, Brush>::const_iterator iter = m_brushes.find(id);

	if (iter != m_brushes.end())
	{
		return (iter->second);
	}
	return ((m_brushes.find(-1))->second);
}

const Paint &paintingGeom::paintFromId(int id) const
{
	std::map<int, Paint>::const_iterator iter = m_paints.find(id);

	if (iter != m_paints.end())
	{
		return (iter->second);
	}
	return ((m_paints.find(-1))->second);
}

float paintingGeom::travelCutoff(int brushId, int paintId) const
{
	const Brush &b = brushFromId(brushId);
	const Paint &p = paintFromId(paintId);
	float cutoff = b.retention() * p.travel;
	if (cutoff < epsilon)
	{
		cutoff = epsilon;
	}
	return cutoff;
}


void paintingGeom::addStroke(const Stroke &stroke, int parentIndex)
{
	int brushId = stroke.brushId();
	int paintId = stroke.paintId();
	
	const Brush &b = brushFromId(brushId);
	Cluster &g = prepCluster(brushId, b.physicalId(), paintId);
	g.pushStroke(stroke, parentIndex);
}


Cluster &paintingGeom::prepCluster(
	int brushId,
	int physicalId,
	int paintId)
{

	// FIRST CLUSTER
	if (m_clusters.empty())
	{
		float cutoff = travelCutoff(brushId, paintId);
		m_clusters.push_back(
			Cluster(brushId, paintId, cutoff, Cluster::kBrush));
		return m_clusters.back();
	}

	// We got here, so there's at least one Cluster and
	// therefore safe to call back()
	const Cluster &back = m_clusters.back();

	int lastBrushId = back.brushId();
	int lastPaintId = back.paintId();

	// CHANGE PAINT OR BRUSH (tool change)
	if (!(lastBrushId == brushId && lastPaintId == paintId))
	{
		float cutoff = travelCutoff(brushId, paintId);
		Cluster::Reason reason = Cluster::kBrush;
		if (lastPaintId == paintId)
		{ // brush Id changed - but is it the same physical brush?
			int lastPhysicalId = brushFromId(lastBrushId).physicalId();
			if (lastPhysicalId == physicalId)
			{
				reason = Cluster::kTcp;
			}
		}
		m_clusters.push_back(
			Cluster(brushId, paintId, cutoff, reason));
		return m_clusters.back();
	}

	// RAN OUT PAINT (dip only)
	// FORCE DIP ON FIRST STROKE OF CURVE
	/*
	Since the blocks above have taken care of cases where paint or brush
	changed, we know the paint and brush didn't change here, so we can
	instantiate a cluster from the last cluster in the array.
	*/

	if (back.ranOutOfPaint())
	{
		// cerr << "RAN OUT PAINT  (dip only)" << endl;
		m_clusters.push_back(Cluster(
			back.brushId(),
			back.paintId(),
			back.travelCutoff(),
			Cluster::kPaint));
	}

	return m_clusters.back();
}

void paintingGeom::setApproaches(float approachStart, float approachMid,
								 float approachEnd, float ptpThresh)
{
	std::vector<Cluster>::iterator iter;
	for (iter = m_clusters.begin(); iter != m_clusters.end(); iter++)
	{
		iter->setApproaches(approachStart, approachMid, approachEnd, ptpThresh);
	}
}

void paintingGeom::dipCombinations(MIntArray &result) const
{
	std::set<std::pair<int, int>> combos;
	std::vector<Cluster>::const_iterator iter;
	for (iter = m_clusters.begin(); iter != m_clusters.end(); iter++)
	{
		std::pair<int, int> combo(iter->brushId(), iter->paintId());
		;
		combos.insert(combo);
	}
	for (std::set<std::pair<int, int>>::const_iterator citer = combos.begin();
		 citer != combos.end(); citer++)
	{
		result.append(citer->first);
		result.append(citer->second);
	}
}

void paintingGeom::offsetBrushContact()
{
	std::vector<Cluster>::iterator curr_cluster = m_clusters.begin();
	MFloatVector offset;
	for (; curr_cluster != m_clusters.end(); curr_cluster++)
	{
		const Brush &brush = brushFromId(curr_cluster->brushId());
		float height = brush.transitionHeight();
		float power = brush.contactPower();
		Cluster::stroke_iterator curr_stroke= curr_cluster->strokes_begin();
		for (;curr_stroke != curr_cluster->strokes_end(); curr_stroke++)
		{
			Stroke::target_iterator curr_target = curr_stroke->targets_begin();
			for (;curr_target != curr_stroke->targets_end(); curr_target++)
			{
				float dist_from_ground = (1.0 - curr_target->weight());
				dist_from_ground = (dist_from_ground > epsilon) ? pow(dist_from_ground, power) * height : 0;
				
				offset = curr_target->zAxis() * -dist_from_ground;
				curr_target->offsetBy(offset);
			}
		}

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
// 		cluster &g = prepCluster(force, brushId , paintId );
// 		g.pushStroke(*citer);
// 	}
// }

	// short paintId = stroke.paintId();
	// short physicalId = b.physicalId;
	// bool force = stroke.forceDip();

// void paintingGeom::displace(MFnMesh &meshFn, MMeshIsectAccelParams &ap)
// {
// 	std::vector<Cluster>::iterator iter;
// 	for (iter = m_clusters.begin(); iter != m_clusters.end(); iter++)
// 	{
// 		iter->displace(meshFn, ap);
// 	}
// }


// void paintingGeom::applyBiases(float mult)
// {
// 	std::vector<Cluster>::iterator iter;
// 	for (iter = m_clusters.begin(); iter != m_clusters.end(); iter++)
// 	{
// 		const Brush &brush = brushFromId(iter->brushId());
// 		iter->applyBiases(brush, mult);
// 	}
// }

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
