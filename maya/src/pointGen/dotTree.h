/**
 * @file dotTree.h
 * @brief Defines the dotTree class for spatial organization of 2D points.
 * 
 * This file contains the dotTree class and related structures which implement
 * a k-d tree for efficient spatial organization and querying of 2D points.
 * It provides functionality for nearest neighbor searches, fixed radius searches,
 * and other spatial queries useful for point-based algorithms.
 * 
 * Copyright (C) 2018 hoolyMama
 * Author: Julian Mann
 */

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

#include <dotData.h>
// #include "errorMacros.h"
#include "mayaMath.h"
#include "vec2d.h"





typedef mayaMath::axis axis;

class dotData;

/**
 * @struct dotTreeNode
 * @brief Node structure for the k-d tree.
 * 
 * Represents a node in the k-d tree, which can be either a bucket node
 * containing points or an internal node that divides the space.
 */
struct dotTreeNode  {
	bool bucket;					/**< Flag indicating if this node is a bucket. */
	axis cutAxis;					/**< For internal nodes, the axis used to divide the space. */
	float cutVal;					/**< For internal nodes, the value along the cut axis. */
	dotTreeNode *loChild, *hiChild;	/**< Pointers to child nodes. */
	unsigned int loPoint, hiPoint;	/**< Indices into the permutations array. */
	PT_LIST *overlapList;			/**< List of dotData whose radii intersect this bucket. */
};


// struct knnSearchData;

/**
 * @struct knnSearchPdData
 * @brief Structure for k-nearest neighbor search results.
 * 
 * Used in priority queues for k-nearest neighbor searches, storing
 * information about the relationship between points.
 */
struct knnSearchPdData {
	float weakness;    /**< Weakness value for prioritization. */
	float length;      /**< Length of the difference vector. */
	JVector2D diff;    /**< Difference vector between points. */
	dotData *dot;      /**< Pointer to the dotData object. */

    /**
     * @brief Less-than operator for priority queue ordering.
     * @param other The other knnSearchPdData to compare with.
     * @return True if this object has lower weakness than the other.
     */
	bool operator< (const knnSearchPdData &other) const
	{
		return weakness < other.weakness;
	}

    /**
     * @brief Checks if there is a significant difference.
     * @return True if the length is greater than epsilon.
     */
	bool hasDifference() const {
		return (length > epsilon);
	}

    /**
     * @brief Calculates the force vector based on weakness and length.
     * @return Force vector as a JVector2D.
     */
	JVector2D force() const {
		if (length < epsilon)
		{
			return JVector2D(0.0, 0.0);
		}
		return diff * ((1.0f - weakness) / length);
	}

};

/** @brief Type definition for a priority queue of knnSearchPdData. */
typedef std::priority_queue<knnSearchPdData> KNN_PD_QUEUE;


/**
 * @class dotTree
 * @brief A k-d tree implementation for efficient spatial organization of 2D points.
 * 
 * The dotTree class implements a k-d tree data structure for efficient spatial
 * organization and querying of 2D points. It provides methods for building the tree,
 * performing nearest neighbor searches, fixed radius searches, and other spatial
 * queries useful for point-based algorithms.
 */
class dotTree
{

public:
    /**
     * @brief Default constructor.
     */
	dotTree();

    /**
     * @brief Destructor.
     */
	~dotTree();

    /**
     * @brief Gets the number of points in the tree.
     * @return The number of points.
     */
	const int size() const;

    /**
     * @brief Sets the maximum number of points per bucket.
     * @param b The maximum number of points.
     */
	void  setMaxPointsPerBucket( int b);

    /**
     * @brief Populates the tree with points from a vector of dotData.
     * @param dots Vector of dotData objects to populate the tree with.
     * @return MStatus indicating success or failure.
     */
	MStatus populate( const std::vector<dotData> &dots);

    /**
     * @brief Builds the k-d tree structure.
     * @return MStatus indicating success or failure.
     */
	MStatus build();

    /**
     * @brief Gets the root node of the tree.
     * @return Pointer to the root node.
     */
	const dotTreeNode *root() const;

    /**
     * @brief Gets the permutation list of points.
     * @return Pointer to the permutation list.
     */
	PT_LIST *perm();
    
    /**
     * @brief Gets the original list of points.
     * @return Pointer to the original list.
     */
	PT_LIST *dots();

    /**
     * @brief Performs a fixed radius search around a point.
     * @param p The node to start the search from.
     * @param searchDot The point to search around.
     * @param q Queue to store the search results.
     */
	void fixedRadiusSearch(
	  const dotTreeNode *p,
	  const dotData *searchDot,
	  KNN_PD_QUEUE *q
	)  const;

    /**
     * @brief Finds the N strongest points relative to a search point.
     * @param p The node to start the search from.
     * @param searchDot The point to search relative to.
     * @param q Queue to store the search results.
     */
	void  strongestNPoints(
	  const dotTreeNode *p,
	  const dotData *searchDot,
	  KNN_PD_QUEUE *q
	)  const;

private:
    /**
     * @brief Selects the kth element using Wirth's algorithm.
     * @param left Left index of the range.
     * @param right Right index of the range.
     * @param k The position to select.
     * @param cutAxis The axis to use for comparison.
     */
	void wirthSelect(int left, int right, int k, axis cutAxis);

    /**
     * @brief Finds the axis with maximum spread.
     * @param low Low index of the range.
     * @param high High index of the range.
     * @return The axis with maximum spread.
     */
	const axis findMaxAxis(int low, int high) const;

    /**
     * @brief Recursively deletes a node and its children.
     * @param p The node to delete.
     */
	void makeEmpty(dotTreeNode *p);

    /**
     * @brief Sets the overlap list for a node.
     * @param p The node to set the overlap list for.
     * @param dot The dot to check for overlap.
     */
	void setOverlapList(dotTreeNode *p, dotData *dot);

    /**
     * @brief Recursively builds the tree.
     * @param low Low index of the range.
     * @param high High index of the range.
     * @return Pointer to the root of the built subtree.
     */
	dotTreeNode *build(int low, int high);

	PT_LIST *m_pPerm;						/**< Permutation of all points. */
	PT_LIST *m_pClean;						/**< All points in original order. */

	dotTreeNode *m_pRoot;					/**< Pointer to root of tree. */
	unsigned int m_maxPointsPerBucket;	    /**< Maximum points per bucket. */
};

#endif

