#ifndef _clusterGeom
#define _clusterGeom

#include <vector>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>
#include "paint.h"
#include "brush.h"

#include "stroke.h"

class clusterGeom
{
public:
	enum Reason
	{
		kNone,
		kTool,
		kDip,
		kTcp
	};

	clusterGeom(
		short brushId,
		short paintId,
		double travelCutoff,
		clusterGeom::Reason reason);

	~clusterGeom();

	short paintId() const;

	short brushId() const;

	bool ranOutOfPaint() const;

	double travelCutoff() const;

	clusterGeom::Reason reason() const;

	double travel() const;

	const std::vector<Stroke> &strokes() const;

	void pushStroke(const Stroke &s, int parentIndex);

	void setApproaches(double approachStart, double approachMid,
					   double approachEnd, double ptpThresh);

	// void setPreStops(double threshold);
	// clusterGeom &operator=( const clusterGeom &other );

	void displace(MFnMesh &meshFn, MMeshIsectAccelParams &ap);

	void offsetBrushContact(const Brush &brush);
	void applyBiases(const Brush &brush, float mult);

	// friend ostream &operator<<(ostream &os, const clusterGeom &geom);

private:
	void setDeparture(double approachMid, double approachEnd);
	void setArrival(double approachStart, double approachMid, double ptpThresh);

	std::vector<Stroke> m_strokes;
	Reason m_reason;
	MString m_name;
	short m_paintId;
	short m_brushId;
	double m_travelCutoff;
	double m_travel;
};

#endif