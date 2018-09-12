#ifndef _strokeGeom
#define _strokeGeom


#include <vector>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>

#include <maya/MTransformationMatrix.h>
#include <maya/MAngle.h>


#include "stroke.h"
#include "brush.h"

class strokeGeom
{
public:

	strokeGeom(const Stroke &src, short brushId, short paintId, bool force);
	~strokeGeom();

	const MMatrix &startApproach() const ;
	const MMatrix &endApproach() const ;
	const MMatrixArray &targets() const ;
	const MVectorArray &tangents() const ;
	const MVector &planeNormal() const;

	const MPoint &pivot() const;



	double arcLength() const ;
	short direction() const ;  // 1 or -1

	short brushId() const;
	short paintId() const;
	bool forceDip() const;

	void setBrushId(short val) ;
	void setPaintId(short val) ;

	// void setForceDip(bool value);


	int id() const ;
	int parentId() const;

	void setIds(int parentId, int uid) ;

	/* These functions return data used for drawing. */
	void getPoints(MFloatPointArray &result, double stackHeight = 0.0) const;
	void getXAxes(MFloatVectorArray &result) const;
	void getYAxes(MFloatVectorArray &result) const;
	void getZAxes(MFloatVectorArray &result) const;

	void getStopPoints(MFloatPointArray &result, double stackHeight = 0.0) const;
	void getStopXAxes(MFloatVectorArray &result) const;
	void getStopYAxes(MFloatVectorArray &result) const;
	void getStopZAxes(MFloatVectorArray &result) const;

	void getBorders(MFloatPointArray &lefts, MFloatPointArray &rights,
	                const Brush &brush, bool withLift = false, double stackHeight = 0.0) const;

	void  getApproaches(MFloatPointArray &startApproachPoints,
	                    MFloatPointArray &endApproachPoints, double stackHeight = 0.0) const;

	void getFullPath(MFloatPointArray &points, double stackHeight = 0.0) const;

	/* These functions return data for painting query command */
	void getAllPositions(const MMatrix &worldMatrix, MPointArray &result) const;

	void getAllRotations(
	  const MMatrix &worldMatrix,
	  MTransformationMatrix::RotationOrder order,
	  MAngle::Unit unit,
	  MVectorArray &result ) const;



	void getStopPositions(const MMatrix &worldMatrix, MPointArray &result) const;

	void getStopRotations(
	  const MMatrix &worldMatrix,
	  MTransformationMatrix::RotationOrder order,
	  MAngle::Unit unit,
	  MVectorArray &result ) const;



	void getAllTangents(const MMatrix &worldMatrix, MVectorArray &result) const;

	void getPivotUVs(
	  const MMatrix &inversePlaneMatrix,
	  float &u,
	  float &v) const ;

	friend ostream &operator<<(ostream &os, const strokeGeom &geom);

	void addPreStop(const MMatrix &mat);


private:
	unsigned m_id;
	unsigned m_parentId;
	MMatrix m_startApproach;
	MMatrix m_endApproach;
	MMatrixArray m_targets;
	MVectorArray m_tangents;
	double m_arcLength;
	short m_direction; // 1 or -1
	MVector m_planeNormal;
	short m_brushId;
	short m_paintId;
	MPoint m_pivot;
	bool m_forceDip;

	MMatrixArray m_preStops;

};


#endif