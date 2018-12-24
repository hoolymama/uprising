#ifndef _skGraph_H
#define _skGraph_H

#include <vector>
#include <map>
#include <string>
#include "skNode.h"
#include <maya/MVectorArray.h>

#define cimg_display 0
#include "CImg.h"
using namespace cimg_library;


// typedef std::tuple <int, int, int> coord;

class skGraph
{
public:
    skGraph();
    skGraph(const CImg<unsigned char>  *pImage );
    ~skGraph();



    void getPoints(MVectorArray &result) const;
    void getEdges(MVectorArray &result) const;

private:

    skNode *addNode(int x, int y, int z = 0);
    skNode *addNode(const coord &c);

    void connect(coord from, coord to);
    void resolveBranches();
    void  detatchStraightest( skNode *node, int z);
    // void build(const CImg<unsigned char> *const inImage);
    void  splitOff( skNode *node,  skNode *first,  skNode *second, int z);
    std::map<coord, skNode * > m_nodes;
    int m_width;
    int m_height;

};


#endif