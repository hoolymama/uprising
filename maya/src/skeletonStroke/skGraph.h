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

class skGraph
{
public:
    skGraph();
    // skGraph(const CImg<float>   &inImage, int x, int y, int resx , int res);
    skGraph(const CImg<float> &inImage);

    ~skGraph();

    void getPoints(MVectorArray &result) const;
    void getEdges(MVectorArray &result) const;
    void getRadius(MDoubleArray &result) const;
    bool hasJunctions() const;

    int getChains(const MFloatMatrix &projection,
                   std::vector<skChain> &chains,
                   int step);

    // void  draw(
    //     CImg<unsigned char>  &image,
    //     float mult = 1.0,
    //     float offset = 0.0,
    //     bool adjustStampAtMaxOnly = true) const;

    void draw(
        CImg<unsigned char> &image,
        float maxStampRadiusPixels) const;

    void  draw(
        CImg<unsigned char> &image, 
        float maxStampRadiusPixels, 
        int spanPixels) const;

    void clampRadius(float maxRadius);

    void adjustRadius(float mult, float offset);

    void prune(int minBranchLength);
    void detachBranches();

    void verify() const;
    void betterPrune(int minBranchLength);
    void removeLooseTwigs(int minTwigLength);

    int numNodes() const;

private:
    void _pruneTwig(TWIG &twig, skNode *junction = 0);

    void _getTwigClusters(int maxNodes, CLUSTERS &result);

    void _verifyDegrees() const;
    void _verifyNeighborsExist() const;
    // void _setRadius(const CImg<float>  *pImage);
    skNode *_addNode(int x, int y, int z, float radius);
    skNode *_addNode(const coord &c, float radius);

    void _deleteNode(skNode *node);

    void _connect(coord from, coord to);

    void _detatchStraightest(skNode *node, int z);
    void _splitOff(skNode *node, skNode *first, skNode *second, int z);
    int _resetSeen();
    void _disconnect(skNode *a, skNode *b);
    std::map<coord, skNode *> m_nodes;

    int m_width;
    int m_height;
};

#endif