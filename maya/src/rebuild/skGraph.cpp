#include <deque>

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



skGraph::skGraph(const CImg<unsigned char>  *pImage):
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

    CImg<bool> image = pImage->get_norm();


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
                    nWhitePixels--; \

                    connect(neighbor, curr);
                }
            }
            q.pop_front();
        }
        start = whitePixel;
    }

    resolveBranches();

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
resolveBranches() is called once the graph has been built.
It finds all junctions, and then removes the straightest 2 branches
from the node, then reconnects them with a new node.

It keeps going until the original junction has less than 3 branches


*/
void skGraph::resolveBranches()
{
    for (std::map<coord, skNode *>::iterator iter = m_nodes.begin();
            iter != m_nodes.end();
            iter++)
    {
        coord c = iter->first;
        cerr << "Coord:(" << c.x << "," << c.y << "," << c.z << ") ";
        skNode *node = iter->second;
        int z = 1;
        while (node->neighbors.size() > 2)
        {
            cerr << z << " ";
            detatchStraightest(node, z);
            z++;
        }
        cerr << endl;;
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
                cerr << "facing: " << facing << endl;
                first = iter1->second;
                second = iter2->second;
                if (facing == -1.0)
                {
                    done = true;
                }

                straightest = facing;
            }
            if (done) { break; }
            // combinations.push_back(std::make_tuple(*iter1, *iter2, dotprod));
        }
        if (done) { break; }
        offset++;
    }

    // if (first && second)
    // {
    splitOff(node, first, second, z);
    cerr << "Did Split" << endl;
    // }

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
