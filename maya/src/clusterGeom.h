#ifndef _clusterGeom
#define _clusterGeom


#include <vector>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>
#include "paint.h"
#include "brush.h"

#include "strokeGeom.h"

class clusterGeom
{
public:

	enum Reason {kNone, kTool, kDip, kTcp };

	clusterGeom(
	  short brushId,
	  short paintId,
	  double travelCutoff,
	  clusterGeom::Reason reason);

	~clusterGeom();

	short paintId() const;

	short brushId() const;

	bool ranOutOfPaint() const;


	double travelCutoff() const ;

	clusterGeom::Reason reason() const;


	double travel() const ;

	const std::vector<strokeGeom> &strokes() const ;

	void pushStroke(const strokeGeom &s);


	void setPreStops(double threshold);
	// clusterGeom &operator=( const clusterGeom &other );

	friend ostream &operator<<(ostream &os, const clusterGeom &geom);

private:


	std::vector<strokeGeom> m_strokes;
	Reason m_reason;
	MString m_name;
	short m_paintId;
	short m_brushId;
	double m_travelCutoff;
	double m_travel;

};


#endif