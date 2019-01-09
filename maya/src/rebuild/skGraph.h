#ifndef _skGraph_H
#define _skGraph_H

#include <vector>
#include <map>
#include <string>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>


#define cimg_display 0
#include "CImg.h"

#include "skPoint.h"
#include "skNode.h"
#include "skChain.h"

using namespace cimg_library;



// typedef std::tuple <int, int, int> coord;

class skGraph
{
public:
    skGraph();

    skGraph(const CImg<float>  *pImage);

    ~skGraph();



    void getPoints(MVectorArray &result) const;
    void getEdges(MVectorArray &result) const;
    void getRadius(MDoubleArray &result) const;
    bool hasJunctions() const;
    // void getChains(std::vector< skChain > &chains, int step = 1, int minPixels = 2 ) const;
    void getChains(const  MFloatMatrix &projection,  std::vector< skChain > &chains ,
                   int step = 1, int minPixels = 2) const;
private:

    void setRadius(const CImg<float>  *pImage);
    skNode *addNode(int x, int y, int z = 0);
    skNode *addNode(const coord &c);

    void connect(coord from, coord to);
    void detachBranches();
    void  detatchStraightest( skNode *node, int z);
    // void build(const CImg<unsigned char> *const inImage);
    void  splitOff( skNode *node,  skNode *first,  skNode *second, int z);
    std::map<coord, skNode * > m_nodes;
    int m_width;
    int m_height;

};


#endif