#ifndef _strokeGeom
#define _strokeGeom


#include <vector>
#include <maya/MVectorArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MIntArray.h>
#include <maya/MTransformationMatrix.h>

#include <maya/MFnMesh.h>



#include <maya/MAngle.h>

#include "stroke.h"
#include "brush.h"
#include "paint.h"

class strokeGeom
{
public:


	enum StrokeFilterOperator {
		kGreaterThan,
		kLessThan,
		kEqualTo,
		kNotEqualTo,
		kNoOp
	};

	// strokeGeom();
	strokeGeom(int id, const Stroke &src, short brushId, short paintId, int layerId,
	           bool force);
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

	int  layerId() const;

	bool forceDip() const;

	void setBrushId(short val) ;
	void setPaintId(short val) ;


	bool overlapsPlane(const MMatrix &inversePlaneMatrix) const;



	void setSortColor(const MFloatVector &color);
	void setFilterColor(const MFloatVector &color);

	// void setForceDip(bool value);

	int id() const ;
	int parentId() const;
	int repeatId() const;
	// int globalId() const;

	void setParentId(int parentId) ;
	// void setGlobalId(int globalId) ;

	const MIntArray &sortStack() const;
	void clearSortStack();
	// friend bool operator < (const strokeGeom &lhs, const strokeGeom &rhs);

	/* These functions return data used for drawing. */

	void getHead(MFloatPoint &result, bool withLift,
	             double stackHeight) const;

	void getPoints(MFloatPointArray &result, bool withLift = false,
	               double stackHeight = 0.0) const;
	void getXAxes(MFloatVectorArray &result, bool withLift = false) const;
	void getYAxes(MFloatVectorArray &result, bool withLift = false) const;
	void getZAxes(MFloatVectorArray &result, bool withLift = false) const;

	void getDirectionMatrices(MMatrixArray &result, bool withLift,
	                          double stackHeight) const;

	void getStopPoints(MFloatPointArray &result, double stackHeight = 0.0) const;
	void getStopXAxes(MFloatVectorArray &result) const;
	void getStopYAxes(MFloatVectorArray &result) const;
	void getStopZAxes(MFloatVectorArray &result) const;

	void getBorders(MFloatPointArray &lefts, MFloatPointArray &rights,
	                const Brush &brush, bool withLift = false, double stackHeight = 0.0) const;

	void getApproaches(MFloatPointArray &startApproachPoints,
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

	/*	void getPivotUVs(
		  const MMatrix &inversePlaneMatrix,
		  float &u,
		  float &v) const ;
	*/
	void setCustomSortData(const Brush &brush,  const Paint &paint);

	void setUV( const MMatrix &inversePlaneMatrix);
	void getUV( float &u, float &v);

	void addPreStop(const MMatrix &mat);

	// strokeGeom &operator=( const strokeGeom &other );

	friend ostream &operator<<(ostream &os, const strokeGeom &geom);

	void appendIdToSortStack(bool ascending);
	void appendParentIdToSortStack(bool ascending);
	void appendBrushIdToSortStack(bool ascending);
	void appendPaintIdToSortStack(bool ascending);
	void appendLayerIdToSortStack(bool ascending);
	void appendRepeatIdToSortStack(bool ascending) ;
	void appendCustomBrushIdToSortStack(bool ascending);
	void appendCustomPaintIdToSortStack(bool ascending) ;

	void appendMapRedIdToSortStack(bool ascending) ;
	void appendMapGreenIdToSortStack(bool ascending) ;
	void appendMapBlueIdToSortStack(bool ascending) ;
	void appendValueToSortStack(bool ascending, int value) ;


	bool testAgainstValue(int lhs, StrokeFilterOperator op, int rhs ) const;

	bool testId(StrokeFilterOperator op, int value) const;
	bool testParentId(StrokeFilterOperator op, int value) const;
	bool testBrushId(StrokeFilterOperator op, int value) const;
	bool testPaintId(StrokeFilterOperator op, int value) const;
	bool testLayerId(StrokeFilterOperator op, int value) const;
	bool testRepeatId(StrokeFilterOperator op, int value) const;
	bool testCustomBrushId(StrokeFilterOperator op, int value) const;
	bool testCustomPaintId(StrokeFilterOperator op, int value) const;


	bool testMapRedId(StrokeFilterOperator op, int value) const;
	bool testMapGreenId(StrokeFilterOperator op, int value) const;
	bool testMapBlueId(StrokeFilterOperator op, int value) const;



	void displaceMatrix( MFnMesh &meshFn, MMeshIsectAccelParams &ap, MMatrix &mat);
	void displace( MFnMesh &meshFn, MMeshIsectAccelParams &ap);

private:
	int m_id;
	int m_parentId;
	MMatrix m_startApproach; // These must be displaced
	MMatrix m_endApproach; // These must be displaced
	MMatrixArray m_targets; // These must be displaced
	MVectorArray m_tangents;
	double m_arcLength;
	short m_direction;
	MVector m_planeNormal;
	short m_brushId;
	short m_paintId;
	int m_layerId;
	MPoint m_pivot;
	bool m_forceDip;
	MFloatVector m_sortColor;
	MFloatVector m_filterColor;

	MMatrixArray m_preStops; // These must be displaced
	MIntArray m_sortStack;
	int m_repeatId;
	float m_u;
	float m_v;
	short m_customPaintId;
	short m_customBrushId;

	// int m_globalId;
};



// inline bool operator < (const strokeGeom &lhs , const strokeGeom &rhs) {
// 	const MIntArray &lstack = lhs.sortStack();
// 	const MIntArray &rstack = rhs.sortStack();
// 	int len = lstack.length();
// 	if (len !=  rstack.length()) {
// 		return false;
// 	}
// 	for (int i = 0; i < len; ++i)
// 	{
// 		if (lstack[i] < rstack[i]) {
// 			return true;
// 		}
// 		if (lstack[i] > rstack[i]) {
// 			return false;
// 		}
// 	}
// 	return true;
// }


#endif