#ifndef _strokeCurveGeom
#define _strokeCurveGeom


#include <vector>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>
#include "stroke.h"
#include "strokeGeom.h"


// struct outStroke  {
// 	MMatrix startApproach;
// 	MMatrix endApproach;
// 	MMatrixArray targets;
// 	MVectorArray tangents;
// 	double arcLength;
// 	short direction; // 1 or -1
// };

class strokeCurveGeom
{
public:
	strokeCurveGeom();
	~strokeCurveGeom();

	void create(
	  const std::vector<std::unique_ptr<Stroke> > &strokes,
	  bool forceDip,
	  short brushId,
	  short paintId
	);

	// strokeCurveGeom &operator=( const strokeCurveGeom &other );


	const std::vector<strokeGeom> &strokes() const ;

	short brushId() const;
	short paintId() const;
	bool forceDip() const;

	friend ostream &operator<<(ostream &os, const strokeCurveGeom &g);


private:

	std::vector<strokeGeom> m_strokes;
	bool m_forceDip;
	short m_brushId;
	short m_paintId;

};


#endif