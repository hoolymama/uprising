
#ifndef _stroke_
#define _stroke_

#include <vector>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <maya/MObject.h>

#include <mayaMath.h>

#include "brush.h"
#include "paint.h"


class stroke {
public:

	///
	stroke(
	  double startDist,
	  double endDist,
	  double density,
	  const MVector &planeNormal,
	  const double3 &attack,
	  const double3 &lift,
	  double elevation,
	  double rotation,
	  double translation,
	  double pivotFraction,
	  const Brush &brush,
	  const Paint &paint,
	  const MObject &curveObject
	);

	~stroke();

	const MVectorArray &points() const;
	const MVectorArray &normals() const;
	const Brush &brush() const;
	const Paint &paint() const;

	double arcLength() const;

	MPoint pivot() const;

	bool overlapsPlane(const MMatrix &inversePlaneMatrix) const;


	void getPivotUVs(const MMatrix &inversePlaneMatrix, float &u, float &v) const ;

	void rotate(float rotation, const MVector &axis);

	void translate(const MFloatVector &translation, const MVector &planeNormal);


private:
	MVectorArray m_points;
	MVectorArray m_normals;
	Brush m_brush;
	Paint m_paint;
	MPoint m_pivot;
	double m_rotation;
	double m_translation;
	double m_arcLength;
};

// typedef std::vector<stroke> STROKE_VECTOR;

#endif

