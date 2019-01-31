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
                    _addNode(neighbor.x, neighbor.y, 0, radius);
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


    cerr << "Get Chains form graph W: " << m_width << " -- H: " << m_height << endl;
    if (step < 1)
    {
        step = 1;
    }
    if (minPixels < 1)
    {
        minPixels = 1;
    }

    float pixelWidth = projection[0][0] * 2.0 / float(m_width);


    // JPMDBG;

    MFloatVector half(m_width * 0.5, m_height * 0.5);
    MFloatMatrix norm;
    norm[0][0] = half.x;
    norm[1][1] = -half.y;
    norm[3][0] = half.x;
    norm[3][1] = half.y;

    // JPMDBG;
    MFloatMatrix transformation = norm.inverse()  *  projection;
    /*
    Loop through nodes, looking for endpoints that we havent seen yet,
    so we may make .
    */


    // JPMDBG;
    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
            mapiter != m_nodes.end();
            mapiter++)
    {
        // identify the start of a chain with at least 2 nodes that has not been seen
        if (mapiter->second->isEnd()
                && (! mapiter->second->seen )   /*  && ( mapiter->second->neighbors.size() > 0)  */  ) {

            // JPMDBG;
            // make a chain and set both ends to seen
            // MFloatArray distances;
            skChain chain;
            // JPMDBG;
            skNode *curr = mapiter->second;
            // skNode *last = mapiter->second;
            // float accum = 0;
            int count = 0;
            for (;; count++)
            {

                MFloatVector xy =  curr->c * transformation;
                // cerr << "XY: " << xy << " -- curr->radius * pixelWidth: " << curr->radius << "*" <<   pixelWidth << endl;
                // cerr << "curr->radius * pixelWidth: " << curr->radius << " * " << pixelWidth << endl;
                // JPMDBG;
                skPoint pt(xy.x, xy.y, (curr->radius * pixelWidth));
                // JPMDBG;
                chain.add(pt);
                // JPMDBG;
                curr->seen = true;
                // JPMDBG;


                // find unseen neighbor

                // cerr << "curr: " << curr << endl;
                // cerr << "Num unseen neighbors: " << curr->unseenNeighborCount() << endl;


                // auto neighbor_iter =  std::find_if(
                //                           curr->neighbors.begin(),
                //                           curr->neighbors.end(),
                // [](const std::pair<coord, skNode *> &p) -> bool {
                //     // cerr << "p.second : " << p.second << endl;
                //     return p.second->seen == false;
                // });
                std::vector<skNode *> neighbors;
                int numUnseen = curr->getUnseenNeighbors(neighbors) ;
                // JPMDBG;
                // if (neighbor_iter == curr->neighbors.end())
                if (numUnseen == 0)
                {
                    break;
                }
                // JPMDBG;
                // last = curr;
                // cerr << "neighbors[0]: " <<   neighbors[0] << endl;
                skNode *pn = neighbors[0];
                // cerr << "pn.c: " <<  pn->c.x << "," <<  pn->c.y << "," <<  pn->c.z << endl;
                // JPMDBG;

                curr = neighbors[0];
                // JPMDBG;
                // JPMDBG;
            }
            // JPMDBG;
            // chain is made
            if (count < minPixels) {
                continue;
            }
            // JPMDBG;
            if (step > 1) {
                skChain interpolated;
                chain.interpolate( step, interpolated);
                chains.push_back(interpolated);
            }
            else {
                chains.push_back(chain);
            }
            // JPMDBG;
        }
    }
    // JPMDBG
    // reset the seen flag
    _resetSeen();
}









void skGraph::prune(int minBranchLength)
{
    /*
    iterate over deg_1 nodes. Walk until find deg>2 node, or exceed minBranchLength.
    If deg3 found, delete nodes leading up to it.

    */
    if (minBranchLength < 2) {
        return;
    }

    std::vector<skNode *> toDelete;

    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
            mapiter != m_nodes.end();
            mapiter++)
    {
        if (mapiter->second->isEnd() /* && (! mapiter->second->seen ) */)
        {
            // start walking
            std::vector<skNode *> deletionCandidates;
            // int count = 0;

            skNode *curr = mapiter->second;

            bool doPrune = false;
            for (int count = 0; count < minBranchLength;  count++)
            {

                // if curr has more than 2 neighbor, then it is a junction
                // This cannot happen the first iteration because the first iter is an end.
                // We must have stared the walk. Therefore, sfe to assume candidates contains
                // the previous node
                if (curr->neighbors.size() > 2)
                {
                    _disconnect(curr, deletionCandidates.back());
                    doPrune = true;
                    break;
                }

                auto nextNodeIt =  std::find_if(
                                       curr->neighbors.begin(),
                                       curr->neighbors.end(),
                                       [](const std::pair<coord, skNode *> &p) -> bool { return p.second->seen == false; }
                                   );
                // curr is either the end, or a non-junction chain node

                if (nextNodeIt == curr->neighbors.end())
                {
                    // curr is at the other end, so leave it (for now)
                    // NOTE: Another option would be to delete the chain
                    // as its quite small -
                    doPrune = false;
                    break;
                }
                // There IS a next node - so flag this as seen, put the current node on the deletion queue
                curr->seen = true;
                deletionCandidates.push_back(curr);

                curr = nextNodeIt->second;
            }
            // we broke out of the loop -
            // NOTE: Only if the last node in the  deletionCandidates is an end (meaning it has been detached)
            // do we actually delete the chain. If its not detached
            if (doPrune && deletionCandidates.size()) {
                toDelete.insert( toDelete.end(), deletionCandidates.begin(), deletionCandidates.end() );
            }
        }
    }
    std::vector<skNode *>::iterator deliter ;
    std::map<coord, skNode *>::const_iterator it;
    for (deliter = toDelete.begin(); deliter != toDelete.end(); deliter++)
    {
        it = m_nodes.find((*deliter)->c);
        delete it->second;
        m_nodes.erase(it);
    }
    _resetSeen();
}





// typedef std::pair < skNode *, int > TWIG;
// typedef std::vector < TWIG  > TWIG_CLUSTER;
// typedef std::map <   skNode *, TWIG_CLUSTER > CLUSTERS;



void skGraph::betterPrune(int minBranchLength)
{
    cerr << "betterPrune" << endl;;
    CLUSTERS clusters;

    _getTwigs( minBranchLength, clusters);


    /*
    Now lets prune all the branches in the cluster UNLESS
    that would be all branches


    */



    // for (mapiter = clusters.begin(); mapiter != clusters.end(); mapiter++)
    // {
    //     skNode *junctionNode = mapiter->first;
    //     TWIG_CLUSTER twigCluster = mapiter->second;

    //     // cerr << "CLUSTER JUNCTION NODE AT: " << junctionNode->c << "has " << twigCluster.size() <<
    //     //      "twigs ----------" << endl;

    //     int degree = junctionNode->neighbors.size();
    //     int candidates = twigCluster.size();

    //     numToDelete = std::min( (degree - 2), candidates);
    //     if (numToDelete == candidates)
    //     {
    //         // no need to sort, just delete them all
    //     }
    //     else
    //     {
    //         // delete the shortest
    //     }


    //     TWIG_CLUSTER::iterator tciter;
    //     for (tciter = twigCluster.begin(); tciter != twigCluster.end(); tciter++)
    //     {
    //         cerr << "Twig at end " << tciter->first->c << " has " << tciter->second <<
    //              " nodes before the junction" << endl;
    //     }
    // }




    // cerr << "clusters.size()" << clusters.size() << endl;;
    // CLUSTERS::const_iterator mapiter;
    // for (mapiter = clusters.begin(); mapiter != clusters.end(); mapiter++)
    // {

    //     skNode *junctionNode = mapiter->first;
    //     TWIG_CLUSTER twigCluster = mapiter->second;


    //     cerr << "CLUSTER JUNCTION NODE AT: " << junctionNode->c << "has " << twigCluster.size() <<
    //          "twigs ----------" << endl;
    //     TWIG_CLUSTER::iterator tciter;
    //     for (tciter = twigCluster.begin(); tciter != twigCluster.end(); tciter++)
    //     {
    //         cerr << "Twig at end " << tciter->first->c << " has " << tciter->second <<
    //              " nodes before the junction" << endl;
    //     }
    // }
}

// void pruneTwigs()


void skGraph::_getTwigs(int minBranchLength, CLUSTERS &twigClusters)
{

    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
            mapiter != m_nodes.end();
            mapiter++)
    {
        if (mapiter->second->isEnd() )
        {
            // cerr << "found end "  << endl;
            skNode *curr = mapiter->second;
            TWIG twig = std::make_pair(curr, 0);
            // walk until we find a junction, or exceed the max
            for (int count = 0; count < minBranchLength;  count++)
            {

                //////////////// JUNCTION /////////////////////
                if (curr->neighbors.size() > 2)
                {
                    // its a junction, so stop
                    twigClusters[curr].push_back(twig);
                    break;
                }
                //////////////////////////////////////////



                //////////////// NEXT NODE /////////////////////
                auto nextNodeIt =  std::find_if(
                                       curr->neighbors.begin(),
                                       curr->neighbors.end(),
                                       [](const std::pair<coord, skNode *> &p) -> bool { return p.second->seen == false; }
                                   );
                //////////////////////////////////////////


                //////////////// OTHER END /////////////////////
                if (nextNodeIt == curr->neighbors.end())
                {   // curr is at the other end,
                    break;
                }
                //////////////////////////////////////////


                curr->seen = true;
                twig.second++;
                curr = nextNodeIt->second;
            }
        }
    }
    _resetSeen();
}


void skGraph::verify() const
{
    cerr << "m_nodes.size(): " << m_nodes.size() << endl;
    cerr << "m_width: " << m_width << "  m_height: " << m_height << endl;


    _verifyDegrees();
    _verifyNeighborsExist();

}

void skGraph::_verifyNeighborsExist() const
{

    int errors = 0;
    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
            mapiter != m_nodes.end();
            mapiter++)
    {

        int deg = mapiter->second->neighbors.size();
        std::map<coord, skNode *> neighbors = mapiter->second->neighbors;

        std::map<coord, skNode *>::const_iterator niter;
        for (niter = neighbors.begin();
                niter != neighbors.end();
                niter++)
        {
            coord ncoord = niter->first;
            std::map<coord, skNode *>::const_iterator fiter =  m_nodes.find(ncoord);
            if (fiter == m_nodes.end())
            {
                cerr << "Node: " << mapiter->first << " points to neighbor at " << ncoord <<
                     " but no node exists there!";
                errors++;
            }
            // find the node the neighbor points to
        }
    }
    if (! errors)
    {
        cerr << "All neighbors accounted for!" << endl;
    }
}

void skGraph::_verifyDegrees() const
{
    std::vector<int> counts(9, 0);
    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
            mapiter != m_nodes.end();
            mapiter++)
    {
        int deg = mapiter->second->neighbors.size();
        if (deg < 9)
        {
            counts[deg]++;
        }
    }
    for (int i = 0; i < 9; ++i)
    {
        if (counts[i] > 0)
        {
            cerr << "Num degree " << i << "nodes: " << counts[i] << endl;
        }
    }

}
