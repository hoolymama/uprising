
#ifndef _stroke
#define _stroke

#include <vector>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <maya/MObject.h>

#include <mayaMath.h>

// enum axis {xAxis, yAxis ,zAxis };
// typedef mayaMath::axis axis;
// class stroke ;

// typedef std::vector<stroke*> STROKE_VECTOR;





class stroke {
public:

	///
	stroke(
	  double startDist,
	  double endDist,
	  double density,
	  const MVector &normal,
	  const double3 &attack,
	  const double3 &lift,
	  double elevation,
	  double tipDist,
	  short brushId,
	  short paintId,
	  const MObject &curveObject
	);

	~stroke();


	short brushId() const;

	short paintId() const;

	MVectorArray points;
	MVectorArray normals;
	short m_brushId;
	short m_paintId;


};


// typedef std::vector<stroke> STROKE_VECTOR;

#endif

