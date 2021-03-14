#ifndef _tGraph_H
#define _tGraph_H

#include <vector>
#include <map>
#include <deque>
#include <maya/MFloatPoint.h>
#include "stroke.h"

#include "tNode.h"

class tGraph
{
public:
    tGraph();
    ~tGraph();

    void addEdge(tcoord ca, tcoord cb, const MFloatPoint &pa, const MFloatPoint &pb);

    void getChains(std::vector<MFloatPointArray> &chains);
 
    int numNodes() const;
    friend ostream &operator<<(ostream &os, const tGraph &g );

private:
    tNode * addNode(tcoord c, const MFloatPoint&p);
    void connect(tcoord from, tcoord to);
    void connect(tNode * fromNode, tNode * toNode);


    void disconnect( tNode *a, tNode *b);

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
    void detatchStraightest( tNode *node, int layer);
    
    /**
     * @brief Remove two neighbors from a node, and join them back up with a new
     * node.
     *
     * There can be a junction with more than 2 neighbors. While in the process of
     * breaking the graph up into chains, we visit any such junction and lift 2 of
     * the neighbors away, and join them with a new node. The new node needs a
     * unique layer number because the key to a node is an object with its XYZ
     * coordinates and a layer. Since the new node is a copy, its XYZ coors are the
     * same, so the layer must change. 
     *
     * @param node The node to detatch from the two neighbors.
     * @param a Neighbor A
     * @param b Neighbor B
     * @param layer A layer to assign to the new node that will join A and B. 
     */
    void splitOff( tNode *node,  tNode *a,  tNode *b, int layer);
    void deleteNode(tNode *node); 
    int  resetSeen();

    void propagate(tNode * node, std::deque<tNode *> &chain, bool front);

    std::map<tcoord, tNode * > m_nodes;


};

#endif