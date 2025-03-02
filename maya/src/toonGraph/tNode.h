/**
 * @file tNode.h
 * @brief Defines the tNode class and tcoord struct for graph representation in toon rendering.
 * 
 * This file contains the tNode class and tcoord struct which provide the basic building
 * blocks for graph-based representation of toon outlines. The tcoord struct represents
 * a coordinate in the graph with vertex ID and layer information, while the tNode class
 * represents a node in the graph with position and connectivity information.
 */

#ifndef _tNode_H
#define _tNode_H

#include <map>
#include <vector>

#include <maya/MPoint.h>

/**
 * @struct tcoord
 * @brief A structure representing a coordinate in the toon graph.
 * 
 * The tcoord struct encapsulates a vertex ID and a layer number, which together
 * uniquely identify a position in the toon graph. It provides comparison operators
 * for sorting and equality testing, as well as an offset method for coordinate
 * manipulation.
 */
struct tcoord {
	/** @brief Vertex ID. */
	int vid;
	
	/** @brief Layer number. */
	int layer;

	/**
	 * @brief Constructor with vertex ID and optional layer.
	 * @param v The vertex ID.
	 * @param l The layer number (default: 0).
	 */
	tcoord(int v, int l = 0)
		: vid(v), layer(l) {	}

	/**
	 * @brief Equality operator.
	 * @param other The tcoord to compare with.
	 * @return True if both vertex ID and layer are equal.
	 */
	bool operator==(const tcoord &other) const
	{
		return (vid == other.vid) && (layer == other.layer);
	}

	/**
	 * @brief Less-than operator for sorting.
	 * @param other The tcoord to compare with.
	 * @return True if this tcoord should be ordered before the other.
	 */
	bool operator<(const tcoord &other) const
	{
		if (layer < other.layer) { return true; }
		if (layer > other.layer) { return false; }
		if (vid < other.vid) { return true; }
		return false;
	}
 
	/**
	 * @brief Creates a new tcoord offset by another tcoord.
	 * @param other The tcoord to offset by.
	 * @return A new tcoord with the offset applied.
	 */
	tcoord offset(const tcoord &other) const
	{
		return tcoord( vid + other.vid);
	}
 
	/**
	 * @brief Output stream operator for tcoord.
	 * @param os The output stream.
	 * @param c The tcoord to output.
	 * @return The output stream.
	 */
	friend ostream &operator<<(ostream &os, const tcoord &c)
	{
		os << "[" << c.vid << ", " << c.layer <<"]" ;
		return os;
	}
};

/**
 * @class tNode
 * @brief A class representing a node in the toon graph.
 * 
 * The tNode class represents a node in the toon graph, with a position in 3D space
 * and connections to neighboring nodes. It provides methods for querying the node's
 * properties, such as whether it's an end node, how many neighbors it has, and the
 * straightness of the path through the node.
 */
class tNode
{
public:
	/** @brief The coordinate of this node in the graph. */
	tcoord c;
	
	/** @brief Map of neighboring nodes, keyed by their coordinates. */
	std::map<tcoord, tNode * > neighbors;
	
	/**
	 * @brief Constructor with coordinate and position.
	 * @param rhs The coordinate of the node.
	 * @param p The 3D position of the node.
	 */
	tNode(const tcoord &rhs, const MFloatPoint &p);
	
	/**
	 * @brief Destructor.
	 */
	~tNode();

	/** @brief The 3D position of the node. */
	MFloatPoint point;
	
	/** @brief Flag indicating whether this node has been visited during traversal. */
	bool seen;
	
	/**
	 * @brief Checks if this node is an end node (has 0 or 1 neighbors).
	 * @return True if this is an end node, false otherwise.
	 */
	bool isEnd() const;
	
	/**
	 * @brief Gets the number of neighbors this node has.
	 * @return The number of neighbors.
	 */
	int neighborCount() const;
	
	/**
	 * @brief Calculates the straightness of the path through this node.
	 * @param a The first neighboring node.
	 * @param b The second neighboring node.
	 * @return A value indicating the straightness of the path (higher is straighter).
	 */
	float straightness(const tNode *a, const tNode *b) const;
 
	/**
	 * @brief Gets the number of unseen neighbors.
	 * @return The number of neighbors that have not been visited.
	 */
	int unseenNeighborCount() const;

	/**
	 * @brief Gets the unseen neighbors of this node.
	 * @param result Vector to store the unseen neighbors.
	 * @return The number of unseen neighbors found.
	 */
	int getUnseenNeighbors(std::vector<tNode *> &result) const;
	
	/**
	 * @brief Output stream operator for tNode.
	 * @param os The output stream.
	 * @param t The tNode to output.
	 * @return The output stream.
	 */
	friend ostream &operator<<(ostream &os, const tNode &t);
};

#endif