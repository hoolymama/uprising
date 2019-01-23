#ifndef _paintingGeom
#define _paintingGeom

#include <vector>
#include <map>

#include "stroke.h"

#include "clusterGeom.h"

class paintingGeom
{
public:


	paintingGeom();

	~paintingGeom();


	const std::vector<clusterGeom> &clusters() const;
	const std::map<int, Paint>    &paints() const;
	const std::map<int, Brush>    &brushes() const;

	void setPaints(const std::map<int, Paint> &paints);

	void setBrushes(const std::map<int, Brush> &brushes);

	const Brush &brushFromId(int id) const ;
	const Paint &paintFromId(int id) const ;

	double travelCutoff(int brushId, int paintId) const;


	void addStroke(const Stroke &stroke, int parentIndex);

	void  setApproaches(double approachStart, double approachMid,
	                    double approachEnd, double ptpThresh);

	void dipCombinations(MIntArray &result) const ;

	void 	displace( MFnMesh &meshFn,  MMeshIsectAccelParams &ap );

	void offsetBrushContact();
	// friend ostream &operator<<(ostream &os, const paintingGeom &geom);

private:

	clusterGeom &prepCluster(
	  int brushId,
	  int phisicalId,
	  int paintId);

	std::vector<clusterGeom> m_clusters;
	std::map<int, Paint> m_paints;
	std::map<int, Brush> m_brushes;

};


#endif