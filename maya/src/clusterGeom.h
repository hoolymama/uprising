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

	enum Reason {kNone, kTool, kDip };

	// clusterGeom();

	// clusterGeom(const Paint *p, const Brush *b);
	clusterGeom(
	  short brushId,
	  short paintId,
	  double travelCutoff,
	  clusterGeom::Reason reason);

	// clusterGeom(const clusterGeom &other);




	~clusterGeom();

	// const MMatrix &startApproach() const ;
	// const MMatrix &endApproach() const ;
	// const MMatrixArray &targets() const ;
	// const MVectorArray &tangents() const ;
	// short direction() const ;  // 1 or -1

	short paintId() const;

	short brushId() const;

	bool ranOutOfPaint() const;


	double travelCutoff() const ;

	clusterGeom::Reason reason() const;


	double travel() const ;

	MString name() const ;


	const std::vector<strokeGeom> &strokes() const ;
	// double maxTravel() const ;

	// void setPaint(Paint *p) ;

	// void setBrush(Brush *p) ;

	void pushStroke(const strokeGeom &s);

	clusterGeom &operator=( const clusterGeom &other );

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