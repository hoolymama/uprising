#ifndef _tGraph_H
#define _tGraph_H

#include <vector>
#include <map>
#include <deque>
#include <maya/MFloatPoint.h>
// #include <maya/MDoubleArray.h>
#include "stroke.h"

#include "tNode.h"


 
class tGraph
{
public:
    tGraph();
    ~tGraph();


    void addEdge(tcoord ca, tcoord cb, const MFloatPoint &pa, const MFloatPoint &pb);


    int  addStrokes(std::vector< Stroke > *strokes, int strokeIndex);
    // void getStrokes( std::vector< Stroke > *strokes )  ;


 
    int numNodes() const;
    friend ostream &operator<<(ostream &os, const tGraph &g );

private:
    tNode * addNode(tcoord c, const MFloatPoint&p);
    void connect(tcoord from, tcoord to);
    void connect(tNode * fromNode, tNode * toNode);


    void disconnect( tNode *a, tNode *b);
    void detachBranches();

    void detatchStraightest( tNode *node, int layer);
    void splitOff( tNode *node,  tNode *a,  tNode *b, int layer);
    void deleteNode(tNode *node); 
    int  resetSeen();

    void propagate(tNode * node, std::deque<tNode *> &chain, bool front);

    std::map<tcoord, tNode * > m_nodes;


};

#endif