/**
 * @file tGraph.h
 * @brief Defines the tGraph class for managing toon outline graphs.
 * 
 * This file contains the tGraph class which provides functionality for creating,
 * manipulating, and traversing graphs used in toon outline rendering. It manages
 * collections of tNode objects and their connections, and provides methods for
 * extracting chains of connected nodes for stroke generation.
 */

#ifndef _tGraph_H
#define _tGraph_H

#include <vector>
#include <map>
#include <deque>
#include <maya/MFloatPoint.h>
#include "stroke.h"

#include "tNode.h"

/**
 * @class tGraph
 * @brief A class for managing graphs of toon outline nodes.
 * 
 * The tGraph class manages a collection of tNode objects and their connections,
 * providing functionality for creating, manipulating, and traversing graphs used
 * in toon outline rendering. It includes methods for adding edges, extracting chains
 * of connected nodes, and handling complex junctions in the graph.
 */
class tGraph
{
public:
    /**
     * @brief Default constructor.
     */
    tGraph();
    
    /**
     * @brief Destructor.
     */
    ~tGraph();

    /**
     * @brief Adds an edge between two coordinates in the graph.
     * @param ca The first coordinate.
     * @param cb The second coordinate.
     * @param pa The 3D position of the first coordinate.
     * @param pb The 3D position of the second coordinate.
     */
    void addEdge(tcoord ca, tcoord cb, const MFloatPoint &pa, const MFloatPoint &pb);

    /**
     * @brief Extracts chains of connected nodes from the graph.
     * @param chains Output vector to store the extracted chains as arrays of points.
     */
    void getChains(std::vector<MFloatPointArray> &chains);
 
    /**
     * @brief Gets the number of nodes in the graph.
     * @return The number of nodes.
     */
    int numNodes() const;
    
    /**
     * @brief Output stream operator for tGraph.
     * @param os The output stream.
     * @param g The tGraph to output.
     * @return The output stream.
     */
    friend ostream &operator<<(ostream &os, const tGraph &g );

private:
    /**
     * @brief Adds a node to the graph.
     * @param c The coordinate of the node.
     * @param p The 3D position of the node.
     * @return Pointer to the newly added or existing node.
     */
    tNode * addNode(tcoord c, const MFloatPoint&p);
    
    /**
     * @brief Connects two nodes by their coordinates.
     * @param from The coordinate of the source node.
     * @param to The coordinate of the target node.
     */
    void connect(tcoord from, tcoord to);
    
    /**
     * @brief Connects two nodes directly.
     * @param fromNode Pointer to the source node.
     * @param toNode Pointer to the target node.
     */
    void connect(tNode * fromNode, tNode * toNode);

    /**
     * @brief Disconnects two nodes.
     * @param a Pointer to the first node.
     * @param b Pointer to the second node.
     */
    void disconnect(tNode *a, tNode *b);

    /**
     * @brief Removes the straightest 2 branches from each junction then
     * reconnects them with a new node. It keeps on doing this until the node
     * has no more than 2 neighbors.
     */
    void detachBranches();

    /**
     * @brief Determine the straightest 2 neighbors, detach them and join them back
     * together with a new node.
     *
     * @param node The node to examine.
     * @param layer The layer that must be assigned to the newly created node.
     */
    void detatchStraightest(tNode *node, int layer);
    
    /**
     * @brief Remove two neighbors from a node, and join them back up with a new
     * node.
     *
     * There can be a junction with more than 2 neighbors. While in the process of
     * breaking the graph up into chains, we visit any such junction and lift 2 of
     * the neighbors away, and join them with a new node. The new node needs a
     * unique layer number because the key to a node is an object with its XYZ
     * coordinates and a layer. Since the new node is a copy, its XYZ coords are the
     * same, so the layer must change. 
     *
     * @param node The node to detatch from the two neighbors.
     * @param a Neighbor A
     * @param b Neighbor B
     * @param layer A layer to assign to the new node that will join A and B. 
     */
    void splitOff(tNode *node, tNode *a, tNode *b, int layer);
    
    /**
     * @brief Deletes a node from the graph.
     * @param node Pointer to the node to delete.
     */
    void deleteNode(tNode *node); 
    
    /**
     * @brief Resets the seen flag on all nodes.
     * @return The number of nodes whose seen flag was reset.
     */
    int resetSeen();

    /**
     * @brief Propagates through the graph to build a chain of nodes.
     * @param node The starting node.
     * @param chain The deque to store the chain of nodes.
     * @param front Whether to add nodes to the front of the chain.
     */
    void propagate(tNode * node, std::deque<tNode *> &chain, bool front);

    /** @brief Map of all nodes in the graph, keyed by their coordinates. */
    std::map<tcoord, tNode * > m_nodes;
};

#endif