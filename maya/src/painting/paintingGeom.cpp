
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

void paintingGeom::setPaints(const std::map<int, Paint> &palette)
{
	m_paints = palette;
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
	float cutoff = b.retention() * p.travel();
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
	int potId = stroke.potId();
	

	const Brush &b = brushFromId(brushId);
	Cluster &g = prepCluster(brushId, paintId, potId);
	g.pushStroke(stroke, parentIndex);
}

Cluster &paintingGeom::prepCluster(
	int brushId,
	int paintId,
	int potId
	)
{

	// FIRST CLUSTER
	if (m_clusters.empty())
	{
		float cutoff = travelCutoff(brushId, paintId);
		m_clusters.push_back(
			Cluster(brushId, paintId, potId, cutoff, Cluster::kBrush));
		return m_clusters.back();
	}

	// We got here, so there's at least one Cluster and
	// therefore safe to call back()
	const Cluster &back = m_clusters.back();
	int lastBrushId = back.brushId();
	int lastPotId = back.potId();
	int lastPaintId = back.paintId();
	

	// CHANGE POT OR BRUSH (tool change)
	if (!(lastBrushId == brushId && lastPotId == potId  && lastPaintId == paintId))
	{
		float cutoff = travelCutoff(brushId, paintId);
		m_clusters.push_back(
			Cluster(brushId, paintId, potId, cutoff, Cluster::kBrush));
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
	 
		m_clusters.push_back(Cluster(
			back.brushId(),
			back.paintId(),
			back.potId(),
			back.travelCutoff(),
			Cluster::kPaint));

		return m_clusters.back();
	}

	// nothing happened. No reason to add a new cluster.
	// We simply return the last cluster.
	return m_clusters.back();
}
 

void paintingGeom::setApproaches(
	const MFloatVector &approachStart,
	const MFloatVector &approachMid,
	const MFloatVector &approachEnd,
	float ptpThresh)
{

	paintingGeom::cluster_iterator curr_cluster = clusters_begin();
	for (; curr_cluster != clusters_end(); curr_cluster++)
	{
		Cluster::stroke_iterator curr_stroke = curr_cluster->strokes_begin();
		// departure

		for (; curr_stroke != curr_cluster->strokes_end(); curr_stroke++)
		{
			if (std::next(curr_stroke) == curr_cluster->strokes_end())
			{
				curr_stroke->setDeparture(approachEnd);
			}
			else
			{
				curr_stroke->setDeparture(approachMid);
			}
		}
		// arrivals
		curr_stroke = curr_cluster->strokes_begin();
		for (; curr_stroke != curr_cluster->strokes_end(); curr_stroke++)
		{
			if (curr_stroke == curr_cluster->strokes_begin())
			{
				curr_stroke->setArrival(approachStart);
			}
			else
			{
				curr_stroke->setArrival(approachMid, ptpThresh, *(std::prev(curr_stroke)));
			}
		}
	}
}

void paintingGeom::toolCombinations(MIntArray &result) const
{
	std::set<std::tuple<int, int, int> > combos;
	std::vector<Cluster>::const_iterator iter;
	for (iter = m_clusters.begin(); iter != m_clusters.end(); iter++)
	{
		std::tuple<int, int, int> combo(iter->brushId(), iter->paintId(),  iter->potId());
		combos.insert(combo);
	}
	int brushId, paintId, potId;
	for (std::set<std::tuple<int, int, int> >::const_iterator citer = combos.begin();
		 citer != combos.end(); citer++)
	{
		
		std::tie(brushId, paintId, potId) = *citer;
		result.append(brushId);
		result.append(paintId);
		result.append(potId);
		
	}
}
 