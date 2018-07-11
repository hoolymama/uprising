
#ifndef _stroke_
#define _stroke_

#include <vector>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>

#include <maya/MObject.h>

#include <mayaMath.h>

#include "brush.h"
#include "paint.h"


class Stroke {
public:

	Stroke(
	  unsigned curveId,
	  double startDist,
	  double endDist,
	  double density,
	  const MVector &planeNormal,
	  double rotation,
	  double translation,
	  double pivotFraction,
	  const MVector &brushRotate,
	  bool follow,
	  bool forceDip,
	  const Brush &brush,
	  const Paint &paint,
	  const MObject &curveObject,
	  bool backstroke = false
	);

	Stroke(
	  const Stroke &mother,
	  double offset,
	  double advance,
	  bool reverse,
	  const MVector &planeNormal
	);

	~Stroke();

	const MMatrixArray &targets() const;

	// flat normalized tangents.
	// flat means, projected onto the construction plane.
	const MVectorArray &tangents() const;

	const Brush &brush() const;
	const Paint &paint() const;

	double arcLength() const;

	bool  follow() const;
	bool  forceDip() const;

	unsigned curveId() const;

	double rotation() const;
	double translation() const;

	const MPoint &pivot() const;
	const MVector &brushRotate() const;
	bool overlapsPlane(const MMatrix &inversePlaneMatrix) const;


	void getPivotUVs(const MMatrix &inversePlaneMatrix, float &u, float &v) const ;

	void rotate(float rotation, const MVector &axis);

	void translate(const MFloatVector &translation, const MVector &planeNormal);

	void setApproach(double start, double end)  ;

	double approachStart() const;

	double approachEnd() const ;

	bool isBackstroke() const ;

private:
	unsigned m_curveId;
	MMatrixArray m_targets;
	MVectorArray m_tangents;
	// MVectorArray m_normals;
	Brush m_brush;
	Paint m_paint;
	MPoint m_pivot;
	MVector m_brushRotate;
	double m_bank;
	double m_twist;
	double m_rotation;
	double m_translation;
	double m_arcLength;
	bool m_isBackstroke;
	bool m_follow;
	bool m_forceDip;
	double m_approachDistStart;
	double m_approachDistEnd;

};

// typedef std::vector<stroke> STROKE_VECTOR;

#endif

