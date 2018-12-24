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
	const std::map<short, Paint>    &paints() const;
	const std::map<short, Brush>    &brushes() const;

	void setPaints(const std::map<short, Paint> &paints);

	void setBrushes(const std::map<short, Brush> &brushes);

	const Brush &brushFromId(short id) const ;
	const Paint &paintFromId(short id) const ;

	double travelCutoff(short brushId, short paintId) const;


	void addStroke(const Stroke &stroke, int parentIndex);

	void  setApproaches(double approachStart, double approachMid,
	                    double approachEnd, double ptpThresh);

	void dipCombinations(MIntArray &result) const ;

	void 	displace( MFnMesh &meshFn,  MMeshIsectAccelParams &ap );

	void setBrushTransitions();
	// friend ostream &operator<<(ostream &os, const paintingGeom &geom);

private:

	clusterGeom &prepCluster(
	  short brushId,
	  short phisicalId,
	  short paintId);

	std::vector<clusterGeom> m_clusters;
	std::map<short, Paint> m_paints;
	std::map<short, Brush> m_brushes;

};


#endif