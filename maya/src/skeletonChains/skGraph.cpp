#include <deque>

#include <maya/MFloatArray.h>
#include "skGraph.h"

#include "errorMacros.h"

coord findWhitePixel(const coord &start, const CImg<bool> &image)
{
    int w = image.width();
    int h = image.height();

    for (int y = start.y; y < h; ++y)
    {
        int x = (y == start.y) ? start.x : 0;
        for (; x < w; ++x)
        {
            if (image(x, y))
            {
                return coord(x, y);
            }
        }
    }
    return coord(-1, -1);
}

skGraph::skGraph() : m_nodes(),
                     m_width(),
                     m_height()
{
}

skGraph::skGraph(const CImg<float> &inImage) : m_nodes(),
                                               m_width(),
                                               m_height()
{
    m_width = inImage.width();
    m_height = inImage.height();

    std::deque<coord> q;
    std::vector<coord> kernel;
    kernel.push_back(coord(0, 1));
    kernel.push_back(coord(-1, 1));
    kernel.push_back(coord(1, 1));
    kernel.push_back(coord(-1, 0));
    kernel.push_back(coord(1, 0));
    kernel.push_back(coord(-1, -1));
    kernel.push_back(coord(1, -1));
    kernel.push_back(coord(0, -1));

    CImg<bool> image = inImage.get_threshold(0.5);

    int nWhitePixels = image.sum();
    coord start(0, 0, 0);
    float radius;
    while (nWhitePixels > 0)
    {
        coord whitePixel = findWhitePixel(start, image);
        if (!whitePixel.valid(m_width, m_height))
        {
            break;
        }
        int x = whitePixel.x;
        int y = whitePixel.y;

        radius = inImage(x, y);
        _addNode(x, y, 0, radius);
        q.push_back(whitePixel);
        image(x, y) = false;
        nWhitePixels--;

        while (!q.empty())
        {
            coord curr = q.front();

            std::vector<coord>::const_iterator iter;
            for (iter = kernel.begin(); iter != kernel.end(); iter++)
            {
                coord neighbor = curr.offset(*iter);
                if (!neighbor.valid(m_width, m_height))
                {
                    continue;
                }
                if (image(neighbor.x, neighbor.y))
                {
                    radius = inImage(neighbor.x, neighbor.y);
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

// skGraph::skGraph(const CImg<float>   &inImage, int x, int y, int resx , int resy):
//     m_nodes(),
//     m_width(),
//     m_height()
// {

//     CImg<float> croppedImage =  inImage.get_crop(x, y ,0,0,x+resx, y+resy,1,1);
//     CImg<bool> image = croppedImage.get_threshold(0.5);

//     m_width = image.width();
//     m_height = image.height();

//     std::deque<coord> q;
//     std::vector< coord > kernel;
//     kernel.push_back(coord(0, 1));
//     kernel.push_back(coord(-1, 1));
//     kernel.push_back(coord(1, 1));
//     kernel.push_back(coord(-1, 0));
//     kernel.push_back(coord(1, 0));
//     kernel.push_back(coord(-1, -1));
//     kernel.push_back(coord(1, -1));
//     kernel.push_back(coord(0, -1));

//     int nWhitePixels = image.sum();
//     coord start(0, 0, 0);
//     float radius ;
//     while (nWhitePixels > 0)
//     {
//         coord whitePixel = findWhitePixel(start,  image );
//         if (!whitePixel.valid(m_width, m_height)) {
//             break;
//         }

//         int x =  whitePixel.x;
//         int y =  whitePixel.y;

//         radius = croppedImage(x, y);
//         _addNode(x, y, 0, radius);
//         q.push_back(whitePixel);
//         image(x, y) = false;
//         nWhitePixels--;

//         while (! q.empty())
//         {
//             coord curr = q.front();

//             std::vector< coord >::const_iterator iter;
//             for (iter = kernel.begin(); iter != kernel.end(); iter++ )
//             {
//                 coord neighbor = curr.offset(*iter);
//                 if (! neighbor.valid(m_width, m_height))
//                 {
//                     continue;
//                 }
//                 if (image(neighbor.x, neighbor.y))
//                 {
//                     radius = croppedImage(neighbor.x, neighbor.y);
//                     _addNode(neighbor.x, neighbor.y, 0, radius);
//                     q.push_back(neighbor);
//                     image(neighbor.x, neighbor.y) = false;
//                     nWhitePixels--;
//                     _connect(neighbor, curr);
//                 }
//             }
//             q.pop_front();
//         }
//         start = whitePixel;
//     }

// }

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

void skGraph::draw(CImg<unsigned char> &image, float maxStampRadiusPixels) const
{
    if (!(
            image.width() == m_width &&
            image.height() == m_height &&
            image.spectrum() == 1))
    {
        return;
    }

    // result.assign(m_width, m_height, 1, 1, 0);
    unsigned char color[] = {0};
    for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
         iter != m_nodes.end();
         iter++)
    {
        coord c = iter->first;
        int circleRadius = int(fmin(maxStampRadiusPixels, iter->second->radius));
        image.draw_circle(c.x, c.y, circleRadius, color);
    }
}

void skGraph::clampRadius(float maxRadius)
{

    for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
         iter != m_nodes.end();
         iter++)
    {
        skNode *node = iter->second;
        if (node->radius > maxRadius)
        {
            node->radius = maxRadius;
        }
    }
}

void skGraph::adjustRadius(float mult, float offset)
{
    for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
         iter != m_nodes.end();
         iter++)
    {
        skNode *node = iter->second;
        node->radius = std::max(0.0f, ((node->radius * mult) + offset));
    }
}

// void skGraph::offets(const coord &offsetCoord)
// {
//     for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
//             iter != m_nodes.end();
//             iter++)
//     {
//         skNode *node = iter->second;
//         node->radius = std::max(0.0f, ((node->radius * mult) + offset));
//     }
// }

// coord offset(const coord &other) const
// 	{
// 		return coord( x + other.x,  y + other.y);
// 	}

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
    // if (fromiter == m_nodes.end())
    // {
    //     cerr << "fromiter == m_nodes.end()" << endl;
    // }
    std::map<coord, skNode *>::const_iterator toiter = m_nodes.find(to);
    // if (toiter == m_nodes.end())
    // {
    //     cerr << "toiter == m_nodes.end()" << endl;
    // }

    fromiter->second->neighbors.insert(*toiter);
    toiter->second->neighbors.insert(*fromiter);
}

void skGraph::_disconnect(skNode *a, skNode *b)
{ // assume they are connected
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

void skGraph::_detatchStraightest(skNode *node, int z)
{

    float straightest = 2.0; // not straight at all

    int offset = 1;
    std::map<coord, skNode *>::const_iterator iter1 = node->neighbors.begin();

    skNode *first = 0;
    skNode *second = 0;
    bool done = false;
    for (; iter1 != node->neighbors.end(); iter1++)
    {
        std::map<coord, skNode *>::const_iterator iter2 = node->neighbors.begin();
        std::advance(iter2, offset);
        for (; iter2 != node->neighbors.end(); iter2++)
        {
            float facing = node->facing(iter1->second, iter2->second);
            if (facing < straightest)
            {
                // cerr << "facing: " << facing << endl;
                first = iter1->second;
                second = iter2->second;
                if (facing == -1.0)
                {
                    done = true;
                }

                straightest = facing;
            }
            if (done)
            {
                break;
            }
        }
        if (done)
        {
            break;
        }
        offset++;
    }

    _splitOff(node, first, second, z);
}

void skGraph::_splitOff(skNode *node, skNode *a, skNode *b, int z)
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
    std::map<coord, skNode *>::iterator neighborIt, meIt;
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

int skGraph::_resetSeen()
{
    int count = 0;
    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
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

bool skGraph::hasJunctions() const
{
    for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
         iter != m_nodes.end();
         iter++)
    {
        const skNode *node = iter->second;
        if (node->neighbors.size() > 2)
        {
            return true;
        }
    }
    return false;
}

void skGraph::getChains(
    const MFloatMatrix &projection,
    std::vector<skChain> &chains,
    int step
    )

{
    if (step < 1)
    {
        step = 1;
    }
    // cerr << "origResX:"<< origResX<< " origResY:"<< origResY<< endl;
    // coord cropOffset(offsetX, offsetY);
    // cerr << "cropOffset" << offsetX << " -- " << offsetY << endl;

    float pixelWidth = projection[0][0] * 2.0 / float(m_width);

    MFloatVector half(m_width * 0.5, m_height * 0.5);
    // MFloatVector half(origResX * 0.5, origResY * 0.5);
    
    MFloatMatrix norm;
    norm.setToIdentity();
    norm[0][0] = half.x;
    norm[1][1] = -half.y;
    norm[3][0] = half.x;
    norm[3][1] = half.y;

    // MFloatMatrix cropOffset;
    // cropOffset.setToIdentity();

    // cropOffset[3][0] = offsetX;
    // cropOffset[3][1] = offsetY;

    // MFloatMatrix transformation = norm.inverse()  * crop.inverse() *  projection;
    MFloatMatrix transformation = norm.inverse() * projection;
    /*
    Loop through nodes, looking for endpoints that we havent seen yet,
    so we may make .
    */

    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
         mapiter != m_nodes.end();
         mapiter++)
    {
        // identify the start of a chain with at least 2 nodes that has not been seen
        if (mapiter->second->isEnd() && (!mapiter->second->seen))
        {

            // make a chain and set both ends to seen
            skChain chain;
            skNode *curr = mapiter->second;
            int count = 0;
            for (;; count++)
            {
                // coord offsetCoord = curr->c.offset(cropOffset);
                // coord offsetCoord = curr->c;
                // offsetCoord.x =
                // coord offsetCoord  = curr->c;
                // if (count == 0) {
                //     cerr << "offsetCoord" << offsetCoord << endl;
                // }
                MFloatVector xy = curr->c * transformation;
                skPoint pt(xy.x, xy.y, (curr->radius * pixelWidth));
                chain.add(pt);
                curr->seen = true;

                auto neighbor_iter = std::find_if(
                    curr->neighbors.begin(),
                    curr->neighbors.end(),
                    [](const std::pair<coord, skNode *> &p) -> bool {
                        return p.second->seen == false;
                    });

                if (neighbor_iter == curr->neighbors.end())
                {
                    break;
                }
                curr = neighbor_iter->second;
            }
            if (step > 1)
            {
                skChain interpolated;
                chain.interpolate(step, interpolated);
                chains.push_back(interpolated);
            }
            else
            {
                chains.push_back(chain);
            }
        }
    }
    // reset the seen flag
    _resetSeen();
}

bool _compareTwigLength(const TWIG &a, const TWIG &b)
{
    return (a.size() < b.size());
}

void skGraph::prune(int minBranchLength)
{
    CLUSTERS clusters;

    _getTwigClusters(minBranchLength, clusters);

    CLUSTERS::const_iterator mapiter;
    for (mapiter = clusters.begin(); mapiter != clusters.end(); mapiter++)
    {
        skNode *junctionNode = mapiter->first;
        TWIG_CLUSTER twigCluster = mapiter->second;

        int degree = junctionNode->neighbors.size();
        int numCandidates = twigCluster.size();

        int numToDelete = std::min((degree - 2), numCandidates);
        TWIG_CLUSTER::iterator tciter;

        if (numToDelete < numCandidates)
        {
            // if not deleting all twigs, then sort them on length so we
            // can just delete the shortest.
            std::sort(twigCluster.begin(), twigCluster.end(), _compareTwigLength);
        }

        for (int i = 0; i < numToDelete; i++)
        {
            _pruneTwig(twigCluster[i], junctionNode);
        }
    }
}

void skGraph::_pruneTwig(TWIG &twig, skNode *junction)
{
    /*
        The twig we want to be deleted maybe connected to a junction
    */
    if (junction)
    {
        _disconnect(junction, twig.back());
    }
    std::map<coord, skNode *>::iterator it;
    for (TWIG::iterator twigiter = twig.begin(); twigiter != twig.end(); twigiter++)
    {
        it = m_nodes.find((*twigiter)->c);
        delete it->second;
        m_nodes.erase(it);
    }
}

void skGraph::_getTwigClusters(int minBranchLength, CLUSTERS &twigClusters)
{
    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
         mapiter != m_nodes.end();
         mapiter++)
    {
        if (mapiter->second->isEnd())
        {
            skNode *curr = mapiter->second;
            TWIG twig /*= std::vector<skNode *>()*/;
            // walk until we find a junction, or exceed the max
            for (int count = 0; count < minBranchLength; count++)
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
                auto nextNodeIt = std::find_if(
                    curr->neighbors.begin(),
                    curr->neighbors.end(),
                    [](const std::pair<coord, skNode *> &p) -> bool { return p.second->seen == false; });
                //////////////////////////////////////////

                //////////////// OTHER END /////////////////////
                if (nextNodeIt == curr->neighbors.end())
                { // curr is at the other end,
                    break;
                }
                //////////////////////////////////////////

                curr->seen = true;
                twig.push_back(curr);
                curr = nextNodeIt->second;
            }
        }
    }
    _resetSeen();
}

void skGraph::removeLooseTwigs(int minTwigLength)
{

    // delete isolated linear sections of the graph less than minTwigLength
    TWIG_CLUSTER looseTwigs;
    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
         mapiter != m_nodes.end();
         mapiter++)
    {
        if (mapiter->second->isEnd() && (!mapiter->second->seen))
        {
            TWIG twig;

            bool doDelete = false;
            skNode *curr = mapiter->second;
            for (int count = 0; count < minTwigLength; count++)
            {
                if (curr->neighbors.size() > 2)
                {
                    doDelete = false;
                    // junction, so we are done and we dont delete anything
                    break;
                }

                curr->seen = true;
                twig.push_back(curr);

                // look for unseen neighbor
                auto nextNodeIt = std::find_if(
                    curr->neighbors.begin(),
                    curr->neighbors.end(),
                    [](const std::pair<coord, skNode *> &p) -> bool { return p.second->seen == false; });

                if (nextNodeIt == curr->neighbors.end())
                {
                    // curr is the end - flag the end as seen so we dont try from the other direction

                    doDelete = true;
                    break;
                }

                // curr is not the end, so keep going
                curr = nextNodeIt->second;
            }

            if (doDelete)
            {
                looseTwigs.push_back(twig);
            }
            else
            {
                // we will not delete this twig, so walk back and set seen to false
                for (TWIG::iterator twigiter = twig.begin(); twigiter != twig.end(); twigiter++)
                {
                    (*twigiter)->seen = false;
                }
            }
        }
    }
    TWIG_CLUSTER::iterator tciter;
    for (tciter = looseTwigs.begin(); tciter != looseTwigs.end(); tciter++)
    {
        _pruneTwig(*tciter);
    }
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
            std::map<coord, skNode *>::const_iterator fiter = m_nodes.find(ncoord);
            if (fiter == m_nodes.end())
            {
                cerr << "Node: " << mapiter->first << " points to neighbor at " << ncoord << " but no node exists there!";
                errors++;
            }
            // find the node the neighbor points to
        }
    }
    if (!errors)
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

int skGraph::numNodes() const
{
    return m_nodes.size();
}
