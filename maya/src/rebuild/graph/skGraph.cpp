#include <deque>

#include <maya/MFloatArray.h>
#include "skGraph.h"

#include "errorMacros.h"


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
    std::vector< coord > kernel;
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
    float radius ;
    while (nWhitePixels > 0)
    {
        coord whitePixel = findWhitePixel(start,  image );
        if (!whitePixel.valid(m_width, m_height)) {
            break;
        }
        int x =  whitePixel.x;
        int y =  whitePixel.y;

        radius = (*pImage)(x, y);
        _addNode(x, y, 0, radius);
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
                    radius = (*pImage)(neighbor.x, neighbor.y);
                    _addNode(neighbor.x, neighbor.y);
                    q.push_back(neighbor);
                    image(neighbor.x, neighbor.y) = false;
                    nWhitePixels--;


                    _connect(neighbor, curr);
                }
            }
            q.pop_front();
        }
        start = whitePixel;
    }
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

void skGraph::draw(CImg<unsigned char>  &result) const
{

    result.assign(m_width, m_height, 1, 1, 0);
    unsigned char color[] = { 255 };


    for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
            iter != m_nodes.end();
            iter++)
    {
        coord c = iter->first;
        result.draw_circle(c.x, c.y, iter->second->radius, color);
    }
}

// void skGraph::_setRadius(const CImg<float>  *pImage)
// {
//     for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
//             iter != m_nodes.end();
//             iter++)
//     {
//         coord c = iter->first;
//         // float rad = (*pImage)(c.x, c.y);
//         // cerr << ", " << rad;
//         iter->second->radius = (*pImage)(c.x, c.y);
//     }
//     // cerr << endl;
// }


skNode *skGraph::_addNode(int x, int y, int z, float radius)
{
    return _addNode(coord(x, y, z), radius);
}

skNode *skGraph::_addNode(const coord &c, float radius)
{
    skNode *result = 0;
    std::map<coord, skNode *>::iterator iter = m_nodes.find(c);
    if (iter == m_nodes.end())
    {
        result = new skNode(c, radius);
        m_nodes[c] = result;
    }
    return result;
}




void skGraph::_connect(coord from, coord to)
{
    std::map<coord, skNode *>::const_iterator fromiter = m_nodes.find(from);
    if (fromiter == m_nodes.end())
    {
        cerr << "fromiter == m_nodes.end()" << endl;
    }
    std::map<coord, skNode *>::const_iterator toiter = m_nodes.find(to);
    if (toiter == m_nodes.end())
    {
        cerr << "toiter == m_nodes.end()" << endl;
    }

    fromiter->second->neighbors.insert(*toiter);
    toiter->second->neighbors.insert(*fromiter);
}

void skGraph::_disconnect( skNode *a, skNode *b)
{   // assume they are connected
    std::map<coord, skNode *>::iterator it;
    it = a->neighbors.find(b->c);
    a->neighbors.erase(it);
    it = b->neighbors.find(a->c);
    b->neighbors.erase(it);
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
detachBranches() should be called once the graph has been built.
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
            _detatchStraightest(node, z);
            z++;
        }
    }
}




void skGraph::_detatchStraightest( skNode *node, int z)
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

    _splitOff(node, first, second, z);
}

void skGraph::_splitOff( skNode *node,  skNode *a,  skNode *b, int z)
{
    std::map<coord, skNode *>::iterator it;


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

    /* make a new node at Z level - copy over the radius */
    coord newCoord(node->c.x, node->c.y, z);
    skNode *newNode = _addNode(newCoord, node->radius);

    /* connect up the detached neighbors to the new node */
    _connect(newCoord, a->c);
    _connect(newCoord, b->c);
}


void skGraph::_deleteNode(skNode *node)
{
    std::map<coord, skNode * >::iterator neighborIt , meIt;
    // find each neighbors' reference back to me and and erase if from the map
    for ( neighborIt =  node->neighbors.begin(); neighborIt != node->neighbors.end();
            neighborIt++)
    {
        // erase ME from their map
        meIt = neighborIt->second->neighbors.find(node->c);
        neighborIt->second->neighbors.erase(meIt);
    }
    // now delete this node
    delete node;
}



void skGraph::_resetSeen()
{
    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
            mapiter != m_nodes.end();
            mapiter++)
    {
        mapiter->second->seen = false;
    }
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


void skGraph::getChains(
    const  MFloatMatrix &projection ,
    std::vector< skChain > &chains,
    int step,
    int minPixels)

{
    if (step < 1)
    {
        step = 1;
    }
    if (minPixels < 1)
    {
        minPixels = 1;
    }

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
        // identify the start of a chain with at least 2 nodes that has not been seen
        if (mapiter->second->isEnd()
                && (! mapiter->second->seen )   /*  && ( mapiter->second->neighbors.size() > 0)  */  ) {

            // make a chain and set both ends to seen
            // MFloatArray distances;
            skChain chain;

            skNode *curr = mapiter->second;
            // skNode *last = mapiter->second;
            // float accum = 0;
            int count = 0;
            for (;; count++)
            {

                // find unseen neighbor
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
                // last = curr;
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
    // reset the seen flag
    _resetSeen();
}

void skGraph::prune(int minBranchLength)
{
    /*
    iterate over deg_1 nodes. Walk until find deg3 node, or exceed minBranchLength.
    If deg3 found, delete nodes leading up to it.

    */
    if (minBranchLength < 2) {
        return;
    }


    // JPMDBG;
    // cerr << "Pruning..............................................." << endl;
    std::vector<skNode *> toDelete;
    // JPMDBG;

    int disconnectCount = 0;
    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
            mapiter != m_nodes.end();
            mapiter++)
    {
        // JPMDBG;

        if (mapiter->second->isEnd()  && (! mapiter->second->seen ) )
        {
            // cerr << "Found unseen end node, starting walk...." << endl;
            // start walking
            std::vector<skNode *> deletionCandidates;
            int count = 0;
            // JPMDBG;

            skNode *curr = mapiter->second;

            bool doPrune = false;
            for (;;  count++)
            {
                // JPMDBG;

                // count++;

                // cerr << "count: " << count << endl;

                if ( count >= minBranchLength)
                {
                    // cerr << "count:" << count << " >= min:" << minBranchLength << " so break!" << count << endl;
                    // note, when count is equal to minLen - we bail out and do nothing,
                    // because this branch is long enough to stay
                    break;
                }

                // JPMDBG;

                // if curr has more than 1 unseen neighbor, then it is a junction
                // cerr << "curr->unseenNeighborCount():" << curr->unseenNeighborCount() << endl;
                if (curr->unseenNeighborCount() > 1)
                {   // detach from last
                    // cerr << "current skNode is junction so DISCONNECT and break!" << endl;
                    disconnectCount++;
                    _disconnect(curr, deletionCandidates.back());

                    doPrune = true;
                    break;
                }
                // JPMDBG;

                // cerr << "get iter to next node" << endl;
                auto nextNodeIt =  std::find_if(
                                       curr->neighbors.begin(),
                                       curr->neighbors.end(),
                                       [](const std::pair<coord, skNode *> &p) -> bool { return p.second->seen == false; }
                                   );

                // JPMDBG;

                // curr is either the end, or another non-junction chain node
                curr->seen = true;

                // JPMDBG;


                if (nextNodeIt == curr->neighbors.end())
                {   // curr is at the other end, so leave it (for now)
                    // NOTE: Another option would be to delete the chain
                    // as its quite small -
                    // To do that, we would
                    //
                    // cerr << "There is not next node - so break" << endl;
                    break;
                }
                else
                {
                    // cerr << "There IS a next node - so put the current node on the deletion queue" << endl;
                    deletionCandidates.push_back(curr);
                }
                // JPMDBG;
                // cerr << "Assign next to curr" << endl;

                curr = nextNodeIt->second;
                // JPMDBG;

            }
            // we broke out of the loop -
            // NOTE: Only if the last node in the  deletionCandidates is an end (meaning it has been detached)
            //  do we actually delete the chain. If its not detached
            // JPMDBG;
            if (doPrune && deletionCandidates.size()) {
                // if (deletionCandidates.back()->isEnd())
                // {
                // cerr << "There are deletion candidates and they are orphaned" << endl;
                toDelete.insert( toDelete.end(), deletionCandidates.begin(), deletionCandidates.end() );
                // }
            }

            // JPMDBG;

        }
    }
    //
    // cerr << "DONE LOOPING: disconnectCount is: " << disconnectCount << endl;
    std::vector<skNode *>::iterator deliter ;
    std::map<coord, skNode *>::const_iterator it;
    // JPMDBG;


    // dont delete -
    for (deliter = toDelete.begin(); deliter != toDelete.end(); deliter++)
    {
        it = m_nodes.find((*deliter)->c);
        delete it->second;
        m_nodes.erase(it);
    }
    // JPMDBG;
    _resetSeen();
    // JPMDBG;

}
