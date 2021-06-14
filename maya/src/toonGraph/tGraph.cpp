
#include <maya/MFloatPointArray.h>
#include "tGraph.h"

#include "errorMacros.h"

tGraph::tGraph() : m_nodes()
{
}

tGraph::~tGraph()
{
    for (
        std::map<tcoord, tNode *>::iterator iter = m_nodes.begin(); 
        iter != m_nodes.end(); 
        iter++)
    {
        if (iter->second)
        {
            delete iter->second;
        }
    }
}

void tGraph::addEdge(tcoord ca, tcoord cb, const MFloatPoint &pa, const MFloatPoint &pb)
{
    addNode(ca, pa);
    addNode(cb, pb);
    connect(ca, cb);
}

tNode *tGraph::addNode(tcoord c, const MFloatPoint &p)
{
    tNode *result = 0;
    std::map<tcoord, tNode *>::iterator iter = m_nodes.find(c);
    if (iter == m_nodes.end())
    {
        result = new tNode(c, p);
        m_nodes[c] = result;
    }
    return result;
}

void tGraph::connect(tNode *fromNode, tNode *toNode)
{
    fromNode->neighbors.insert(std::pair<tcoord, tNode *>(toNode->c, toNode));
    toNode->neighbors.insert(std::pair<tcoord, tNode *>(fromNode->c, fromNode));
}

void tGraph::connect(tcoord from, tcoord to)
{
    std::map<tcoord, tNode *>::const_iterator fromiter = m_nodes.find(from);
    std::map<tcoord, tNode *>::const_iterator toiter = m_nodes.find(to);
    fromiter->second->neighbors.insert(*toiter);
    toiter->second->neighbors.insert(*fromiter);
}

void tGraph::disconnect(tNode *a, tNode *b)
{ // assume they are connected
    std::map<tcoord, tNode *>::iterator it;
    it = a->neighbors.find(b->c);
    a->neighbors.erase(it);
    it = b->neighbors.find(a->c);
    b->neighbors.erase(it);
}

void tGraph::deleteNode(tNode *node)
{
    std::map<tcoord, tNode *>::iterator neighborIt, meIt;
    // find each neighbors' reference back to me and and erase if from the map
    for (neighborIt = node->neighbors.begin(); neighborIt != node->neighbors.end();
         neighborIt++)
    {
        // erase ME from their map
        meIt = neighborIt->second->neighbors.find(node->c);
        neighborIt->second->neighbors.erase(meIt);
    }
    // now delete this node
    delete node;
}

int tGraph::resetSeen()
{
    int count = 0;
    for (std::map<tcoord, tNode *>::const_iterator mapiter = m_nodes.begin();
         mapiter != m_nodes.end();
         mapiter++)
    {
        if (mapiter->second->seen)
        {
            mapiter->second->seen = false;
            count++;
        }
    }
    return count;
}


void tGraph::getChains(std::vector<MFloatPointArray> &chains)
{
    // Convert this function to an iterator
    if (! m_nodes.size()) {
        return;
    }

    detachBranches();

    for (std::map<tcoord, tNode *>::const_iterator mapiter = m_nodes.begin();
         mapiter != m_nodes.end();
         mapiter++)
    {
   
        if (!mapiter->second->seen)
        {

            std::deque<tNode *> chain;

            tNode *curr = mapiter->second;
            curr->seen = true;
            chain.push_back(mapiter->second);

            int numNeighbors = curr->unseenNeighborCount();

            if (numNeighbors > 0) // 1 or 2
            {
                propagate(curr, chain, true); // push back
            }

            // go the other way
            numNeighbors = curr->unseenNeighborCount();
            if (numNeighbors > 0)
            {
                propagate(curr, chain, false); // push front
            }

            MFloatPointArray pts;
            for (std::deque<tNode *>::const_iterator iter = chain.begin(); iter != chain.end(); iter++)
            {
                pts.append((*iter)->point);
            }
            chains.push_back(pts);
        }
    }
    resetSeen();
}

void tGraph::propagate(tNode *node, std::deque<tNode *> &chain, bool back)
{

    std::vector<tNode *> neighbors;
    node->getUnseenNeighbors(neighbors);

    tNode *curr = neighbors[0];

    // cerr << "propagate: " << (front ? "front" : "back")  << endl;
    while (true)
    {
        // cerr << (front ? ">" : "<");
        curr->seen = true;
        if (back)
        {
            chain.push_back(curr);
        }
        else
        {
            chain.push_front(curr);
        }

        std::vector<tNode *> neighbors;
        int numNeighbors = curr->getUnseenNeighbors(neighbors);
        if (numNeighbors == 1)
        {
            curr = neighbors[0];
        }
        else
        {
            // we are at the end. Lets see if this was a closed loop and if so, push the seed node.
            if (curr->neighbors.size() == 2)
            {
                if (back)
                {
                    chain.push_back(node);
                }
                else
                {
                    chain.push_front(node);
                }
            }
            break;
        }
    }
}



void tGraph::detachBranches()
{   
    std::map<tcoord, tNode *>::iterator iter = m_nodes.begin();
    for (; iter != m_nodes.end(); iter++)
    {
        tcoord c = iter->first;
        tNode *node = iter->second;
        int layer = 1;
        while (node->neighbors.size() > 2)
        {
            detatchStraightest(node, layer);
            layer++;
        }
    }
    resetSeen();
}

void tGraph::detatchStraightest(tNode *node, int layer)
{
    float straightest = 0;

    int offset = 1;

    tNode *first = 0;
    tNode *second = 0;

    std::map<tcoord, tNode *>::const_iterator iter1 = node->neighbors.begin();
    for (; iter1 != node->neighbors.end(); iter1++)
    {
        std::map<tcoord, tNode *>::const_iterator iter2 = node->neighbors.begin();
        std::advance(iter2, offset);
        for (; iter2 != node->neighbors.end(); iter2++)
        {
            float straightness = node->straightness(iter1->second, iter2->second);
            if (straightness > straightest)
            {
                first = iter1->second;
                second = iter2->second;
                straightest = straightness;
            }
        }
        offset++;
    }
    splitOff(node, first, second, layer);
}


void tGraph::splitOff(tNode *node, tNode *a, tNode *b, int layer)
{
    std::map<tcoord, tNode *>::iterator it;

    /*remove a and b node from the neigbors list of the main node */
    it = node->neighbors.find(a->c);
    node->neighbors.erase(it);
    it = node->neighbors.find(b->c);
    node->neighbors.erase(it);

    /*remove main node from the neigbors lists of the a and b nodes */
    it = a->neighbors.find(node->c);
    a->neighbors.erase(it);
    it = b->neighbors.find(node->c);
    b->neighbors.erase(it);

    /* make a new node at layer */
    tcoord newCoord(node->c.vid, layer);
    tNode *newNode = addNode(newCoord, node->point);

    /* connect up the detached neighbors to the new node */
    connect(newCoord, a->c);
    connect(newCoord, b->c);
}

int tGraph::numNodes() const
{
    return m_nodes.size();
}

ostream &operator<<(ostream &os, const tGraph &g)
{

    for (std::map<tcoord, tNode *>::const_iterator mapiter = g.m_nodes.begin();
         mapiter != g.m_nodes.end();
         mapiter++)
    {
        os << *(mapiter->second) << endl;
    }

    return os;
}