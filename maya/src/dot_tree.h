
#ifndef _dotTree
#define _dotTree

//
// Copyright (C) 2018 hoolyMama
//
// File: dotTree.h
//
// Author: Julian Mann

const float epsilon = 0.00000001;

#include <maya/MFloatVectorArray.h>
#include <vector>
#include <queue>
// #include <algorithm>
#include <map>

#include <dot_data.h>
#include "errorMacros.h"
#include "mayaMath.h"
#include "vec2d.h"





typedef mayaMath::axis axis;

class dotData;

struct dotTreeNode  {
	bool bucket;					// is this node a bucket
	axis cutAxis;					// if not, this axis will divide it
	float cutVal;					// at this value
	dotTreeNode *loChild, *hiChild;		// and these pointers point to the children
	unsigned int loPoint, hiPoint;	// Indices into the permutations array
	PT_LIST *overlapList;					/// list of dotData whose radii intersect this bucket
};


// struct knnSearchData;

struct knnSearchPdData;

// typedef std::vector<dotData*> PT_LIST;

//typedef std::priority_queue<knnSearchData> KNN_QUEUE;

typedef std::priority_queue<knnSearchPdData> KNN_PD_QUEUE;

// typedef std::vector<PT_LIST *> PD_CLUSTERS;

// this struct is used as an element in a priority queue
// struct knnSearchData {
// 	float dist;
// 	MFloatVector point;
// 	bool operator< (const knnSearchData &other) const {return dist < other.dist;}
// };

struct knnSearchPdData {
	float weakness;
	float length;
	JVector2D diff;
	dotData *dot;

	bool operator< (const knnSearchPdData &other) const
	{
		return weakness < other.weakness;
	}

	bool hasDifference() const {
		return (length > epsilon);
	}

	JVector2D force() const {
		if (length < epsilon)
		{
			return JVector2D(0.0, 0.0);
		}
		return diff * ((1.0f - weakness) / length);
	}

};




class dotTree
{

public:

	dotTree();

	~dotTree();

	const int size() const;

	void  setMaxPointsPerBucket( int b);

	// void populate(const MFloatVectorArray &p);

	MStatus populate( const std::vector<dotData> &dots);
	// MStatus populate(const MFloatPointArray &p);
	// MStatus populate(const MFloatPointArray &p, const MIntArray &indices);
	//MStatus populate(const MFloatVectorArray & p, const MDoubleArray & r);

	// MStatus build(bool doOverlapList = true);

	MStatus build();

	const dotTreeNode *root() const;

	PT_LIST *perm();
	PT_LIST *dots();

	// float calculateOffsetAndReturnRadius(
	// 	const dotTreeNode * p,
	// 	const MFloatVector & searchPoint,
	// 	unsigned searchPointId,
	// 	float thresh,
	// 	int neighbors,
	// 	const MFloatVectorArray * normals,
	// 	MFloatVector & result ) const;

	// void  closestNPts(
	// 	const dotTreeNode * p,
	// 	const MFloatVector & searchPoint,
	// 	unsigned searchPointId,
	// 	const MFloatVectorArray * normals,
	// 	const float &thresh,
	// 	KNN_PD_QUEUE *q
	// 	)  const;

	// do search only
	// void  fixedRadiusSearch(
	// 	const dotTreeNode * p,
	// 	const MFloatVector & searchPoint,
	// 	const float & radius,
	// 	PT_LIST * result
	// 	)  const ;

	void fixedRadiusSearch(
	  const dotTreeNode *p,
	  const dotData *searchDot,
	  KNN_PD_QUEUE *q
	)  const  ;

	void  strongestNPoints(
	  const dotTreeNode *p,
	  const dotData *searchDot,
	  KNN_PD_QUEUE *q
	)  const ;

	// bool  pointIsOccupied(
	//   const dotTreeNode *p,
	//   const dotData *searchDot
	// )  const ;

	// void emptinessAtPoint(
	//   const dotTreeNode *p,
	//   const dotData *searchDot,
	//   float &emptiness
	// )  const;

	// void  overlappingEmptiness(
	//   const dotTreeNode *p,
	//   const dotData *searchDot,
	//   float &emptiness
	// )  const ;

private:

	void wirthSelect(  int left,  int right,  int k, axis cutAxis )  ;

	const axis findMaxAxis(int low, int high) const;

	void makeEmpty(dotTreeNode *p) ;

	void  setOverlapList(dotTreeNode *p,  dotData *dot  ) ;

	dotTreeNode *build( int low, int high );

	PT_LIST *m_pPerm;						// all points
	PT_LIST *m_pClean;						// all points in original order

	dotTreeNode *m_pRoot;					// pointer to root of tree
	unsigned int
	m_maxPointsPerBucket;	// at build time keep recursing until no bucket holds more than this



};

#endif

