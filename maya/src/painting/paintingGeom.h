#ifndef _paintingGeom
#define _paintingGeom

#include <vector>
#include <map>

#include "stroke.h"

#include "cluster.h"

class paintingGeom
{
public:

	using cluster_iterator = typename std::vector<Cluster>::iterator;
	using const_cluster_iterator = typename std::vector<Cluster>::const_iterator;

	cluster_iterator clusters_begin() { return m_clusters.begin(); }
	cluster_iterator clusters_end() { return m_clusters.end(); }
	const_cluster_iterator clusters_begin() const { return m_clusters.begin(); }
	const_cluster_iterator clusters_end() const { return m_clusters.end(); }


	paintingGeom();

	~paintingGeom();

	const std::vector<Cluster> &clusters() const;
	const std::map<int, Paint> &paints() const;
	const std::map<int, Brush> &brushes() const;

	void setPaints(const std::map<int, Paint> &paints);

	void setBrushes(const std::map<int, Brush> &brushes);

	const Brush &brushFromId(int id) const;
	const Paint &paintFromId(int id) const;

	float travelCutoff(int brushId, int paintId) const;

	void addStroke(const Stroke &stroke, int parentIndex);

	void setApproaches(float approachStart, float approachMid,
					   float approachEnd, float ptpThresh);

	void dipCombinations(MIntArray &result) const;

	// void displace(MFnMesh &meshFn, MMeshIsectAccelParams &ap);

	// void offsetBrushContact();
 
	// friend ostream &operator<<(ostream &os, const paintingGeom &geom);

private:
	Cluster &prepCluster(
		int brushId,
		int phisicalId,
		int paintId);

	std::vector<Cluster> m_clusters;
	std::map<int, Paint> m_paints;
	std::map<int, Brush> m_brushes;
};

#endif