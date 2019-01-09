#include <deque>

#include <maya/MFloatArray.h>
#include "skGraph.h"



coord findWhitePixel(const coord &start,  const CImg<bool> &image )
{
    int w = image.width();
    int h = image.height();

    for ( int y =  start.y ; y < h; ++y)
    {
        int  x = (y ==  start.y) ?  start.x : 0;
        for (; x < w; ++x)
        {
            if (image(x, y)) {
                return coord(x, y);
            }
        }
    }
    return  coord(-1, -1);
}





skGraph::skGraph():
    m_nodes(),
    m_width(),
    m_height()
{}



skGraph::skGraph(const CImg<float>  *pImage):
    m_nodes(),
    m_width(),
    m_height()
{
    m_width = pImage->width();
    m_height = pImage->height();

    std::deque<coord> q;
    std::vector< coord >    kernel;
    kernel.push_back(coord(0, 1));
    kernel.push_back(coord(-1, 1));
    kernel.push_back(coord(1, 1));
    kernel.push_back(coord(-1, 0));
    kernel.push_back(coord(1, 0));
    kernel.push_back(coord(-1, -1));
    kernel.push_back(coord(1, -1));
    kernel.push_back(coord(0, -1));

    CImg<bool> image = pImage->get_threshold(0.5);

    int nWhitePixels = image.sum();
    coord start(0, 0, 0);

    while (nWhitePixels > 0)
    {
        coord whitePixel = findWhitePixel(start,  image );
        if (!whitePixel.valid(m_width, m_height)) {
            break;
        }
        int x =  whitePixel.x;
        int y =  whitePixel.y;


        addNode(x, y);
        q.push_back(whitePixel);
        image(x, y) = false;
        nWhitePixels--;


        while (! q.empty())
        {
            coord curr = q.front();

            std::vector< coord >::const_iterator iter;
            for (iter = kernel.begin(); iter != kernel.end(); iter++ )
            {
                coord neighbor = curr.offset(*iter);
                if (! neighbor.valid(m_width, m_height))
                {
                    continue;
                }
                if (image(neighbor.x, neighbor.y))
                {

                    addNode(neighbor.x, neighbor.y);
                    q.push_back(neighbor);
                    image(neighbor.x, neighbor.y) = false;
                    nWhitePixels--;

                    connect(neighbor, curr);
                }
            }
            q.pop_front();
        }
        start = whitePixel;
    }
    detachBranches();
    setRadius(pImage);
}



skGraph::~skGraph()
{
    for (std::map<coord, skNode *>::iterator iter = m_nodes.begin(); iter != m_nodes.end();
            iter++)
    {
        if (iter->second)
        {
            delete iter->second;
        }
    }
}




void skGraph::setRadius(const CImg<float>  *pImage)
{
    for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
            iter != m_nodes.end();
            iter++)
    {
        coord c = iter->first;
        float rad = (*pImage)(c.x, c.y);
        // cerr << ", " << rad;
        iter->second->radius = (*pImage)(c.x, c.y);
    }
    // cerr << endl;
}


skNode *skGraph::addNode(int x, int y, int z)
{
    return addNode(coord(x, y, z));
}

skNode *skGraph::addNode(const coord &c)
{
    skNode *result = 0;
    std::map<coord, skNode *>::iterator iter = m_nodes.find(c);
    if (iter == m_nodes.end())
    {
        result = new skNode(c);
        m_nodes[c] = result;
    }
    return result;
}


void skGraph::connect(coord from, coord to)
{
    std::map<coord, skNode *>::const_iterator fromiter = m_nodes.find(from);
    std::map<coord, skNode *>::const_iterator toiter = m_nodes.find(to);

    fromiter->second->neighbors.insert(*toiter);
    toiter->second->neighbors.insert(*fromiter);
}



void skGraph::getPoints(MVectorArray &result) const
{
    for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
            iter != m_nodes.end();
            iter++)
    {
        const skNode *node = iter->second;
        result.append(node->c.toUV(m_width, m_height));
    }
}

void skGraph::getRadius(MDoubleArray &result) const
{
    for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
            iter != m_nodes.end();
            iter++)
    {
        result.append(iter->second->radius);
    }
}


void skGraph::getEdges(MVectorArray &result) const
{
    for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
            iter != m_nodes.end();
            iter++)
    {
        const skNode *node = iter->second;
        std::map<coord, skNode *>::const_iterator niter;
        for (niter = node->neighbors.begin(); niter != node->neighbors.end(); niter++)
        {
            /*
            cerr << "Node:(" << node->c.x << "," << node->c.y << ") Neighbor:(" <<    (*niter)->c.x << "," << (*niter)->c.y << ")" << endl;
            */
            result.append(node->c.toUV(m_width, m_height));
            result.append(niter->second->c.toUV(m_width, m_height));
        }
    }
}





/*
detachBranches() is called once the graph has been built.
It finds all junctions, and then removes the straightest 2 branches
from the node, then reconnects them with a new node.

It keeps going until the original junction has less than 3 branches

*/
void skGraph::detachBranches()
{
    for (std::map<coord, skNode *>::iterator iter = m_nodes.begin();
            iter != m_nodes.end();
            iter++)
    {
        coord c = iter->first;
        skNode *node = iter->second;
        int z = 1;
        while (node->neighbors.size() > 2)
        {
            detatchStraightest(node, z);
            z++;
        }
    }
}




void skGraph::detatchStraightest( skNode *node, int z)
{

    float straightest  = 2.0; // not straight at all

    int offset = 1;
    std::map< coord, skNode * >::const_iterator iter1 = node->neighbors.begin();

    skNode *first = 0;
    skNode *second = 0;
    bool done = false;
    for (; iter1 != node->neighbors.end(); iter1++)
    {
        std::map< coord, skNode * >::const_iterator iter2 = node->neighbors.begin() ;
        std::advance(iter2, offset);
        for (; iter2 != node->neighbors.end(); iter2++)
        {
            float facing = node->facing(iter1->second, iter2->second);
            if (facing < straightest) {
                // cerr << "facing: " << facing << endl;
                first = iter1->second;
                second = iter2->second;
                if (facing == -1.0)
                {
                    done = true;
                }

                straightest = facing;
            }
            if (done) { break; }
        }
        if (done) { break; }
        offset++;
    }

    splitOff(node, first, second, z);

}

void skGraph::splitOff( skNode *node,  skNode *first,  skNode *second, int z)
{
    std::map<coord, skNode *>::iterator it;


    /*remove first and second node from the neigbors list of the main node */
    it = node->neighbors.find(first->c);
    node->neighbors.erase(it);
    it = node->neighbors.find(second->c);
    node->neighbors.erase(it);
    /*remove main node from the neigbors lists of the first and second nodes */
    it = first->neighbors.find(node->c);
    first->neighbors.erase(it);
    it = second->neighbors.find(node->c);
    second->neighbors.erase(it);

    /* make a new node at Z level */
    coord newCoord(node->c.x, node->c.y, z);
    skNode *newNode = addNode(newCoord);

    connect(newCoord, first->c);
    connect(newCoord, second->c);
}




bool skGraph::hasJunctions() const
{
    for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
            iter != m_nodes.end();
            iter++)
    {
        const skNode *node = iter->second;
        if (node->neighbors.size() > 2) {
            return true;
        }
    }
    return false;
}


void skGraph::getChains(const  MFloatMatrix &projection,  std::vector< skChain > &chains ,
                        int step, int minPixels)
const
{

    float pixelWidth = projection[0][0] * 2.0 / float(m_width);


    MFloatVector half(m_width * 0.5, m_height * 0.5);
    MFloatMatrix norm;
    norm[0][0] = half.x;
    norm[1][1] = -half.y;
    norm[3][0] = half.x;
    norm[3][1] = half.y;

    MFloatMatrix transformation = norm.inverse()  *  projection;
    /*
    Loop through nodes, looking for endpoints that we havent seen yet,
    so we may make .
    */
    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
            mapiter != m_nodes.end();
            mapiter++)
    {
        // identify the start of a chain with at lest 2 nodes that has not been seen
        if (mapiter->second->isEnd() && (! mapiter->second->seen )
                && ( mapiter->second->neighbors.size() > 0)  ) {

            // make a chain and set both ends to seen
            // MFloatArray distances;
            skChain chain;

            skNode *curr = mapiter->second;
            skNode *last = mapiter->second;
            // float accum = 0;
            int count = 0;
            for (;; count++)
            {
                auto neighbor_iter =  std::find_if(
                                          curr->neighbors.begin(),
                                          curr->neighbors.end(),
                                          [](const std::pair<coord, skNode *> &p) -> bool { return p.second->seen == false; }
                                      );
                MFloatVector xy =  curr->c * transformation;
                // cerr << "curr->radius * pixelWidth: " << curr->radius << " * " << pixelWidth << endl;
                skPoint pt(xy.x, xy.y, (curr->radius * pixelWidth));

                chain.add(pt);

                curr->seen = true;

                if (neighbor_iter == curr->neighbors.end())
                {
                    break;
                }
                last = curr;
                curr = neighbor_iter->second;
            }

            // chain is made
            if (count < minPixels) {
                continue;
            }

            if (step > 1) {
                skChain interpolated;
                chain.interpolate( step, interpolated);
                chains.push_back(interpolated);
            }
            else {
                chains.push_back(chain);
            }
        }
    }
}


