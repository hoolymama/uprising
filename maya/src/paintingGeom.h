#ifndef _paintingGeom
#define _paintingGeom

#include <vector>
#include <map>

#include "clusterGeom.h"

#include "strokeCurveGeom.h"
class paintingGeom
{
public:


	paintingGeom();

	~paintingGeom();

	// paintingGeom &operator=( const paintingGeom &other );

	const std::vector<clusterGeom> &clusters() const;
	const std::map<short, Paint>    &paints() const;
	const std::map<short, Brush>    &brushes() const;

	void setPaints(const std::map<short, Paint> &paints);

	void setBrushes(const std::map<short, Brush> &brushes);

	void  addStrokeCurve(const strokeCurveGeom &strokeCurve) ;

	const Brush &brushFromId(short id) const ;
	const Paint &paintFromId(short id) const ;

	double travelCutoff(short brushId, short paintId) const;

	friend ostream &operator<<(ostream &os, const paintingGeom &geom);

private:

	void prepClusterForStroke(
	  bool force,
	  short brushId,
	  short paintId
	);

	std::vector<clusterGeom> m_clusters;
	std::map<short, Paint> m_paints;
	std::map<short, Brush> m_brushes;

};


#endif