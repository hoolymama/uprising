#ifndef _paintingGeom
#define _paintingGeom

#include <vector>
#include <map>

#include "stroke.h"

#include "cluster.h"

class paintingGeom
{
public:
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

	void offsetBrushContact();
	// void applyBiases(float mult);

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