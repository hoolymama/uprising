#include <deque>
#include <cmath>
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

    // Black and white image
    CImg<bool> image = inImage.get_threshold(0.5);

    int nWhitePixels = image.sum();
    coord start(0, 0, 0);
    float radius;
    while (nWhitePixels > 0)
    {
        // march through the image looking for white
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

void skGraph::draw(CImg<unsigned char> &image, float maxStampRadiusPixels, int spanPixels) const
{
    if (!(
            image.width() == m_width &&
            image.height() == m_height &&
            image.spectrum() == 1))
    {
        return;
    }

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

void skGraph::adjustRadius(float offset, float maxRadius)
{
    for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
         iter != m_nodes.end();
         iter++)
    {
        skNode *node = iter->second;
        node->radius = std::max(0.0f, std::min(node->radius + offset, maxRadius));
    }
}

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

// void skGraph::_connect(skNode * from, skNode * to)
// {
//     from->neighbors.insert( std::make_pair(to->c, to) );
//     to->neighbors.insert(std::make_pair(from->c, from));
// }


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

int skGraph::getChains(
    const MFloatMatrix &projection,
    std::vector<skChain> &chains,
    int span)

{
    if (span < 1)
    {
        span = 1;
    }

    float pixelWidth = projection[0][0] * 2.0 / float(m_width);

    MFloatVector half(m_width * 0.5, m_height * 0.5);

    MFloatMatrix norm;
    norm.setToIdentity();
    norm[0][0] = half.x;
    norm[1][1] = -half.y;
    norm[3][0] = half.x;
    norm[3][1] = half.y;

    int added = 0;

    MFloatMatrix transformation = norm.inverse() * projection;
    /*
    Loop through nodes, looking for endpoints that we havent seen yet.
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

                MFloatVector xy = curr->c * transformation;
                skPoint pt(xy.x, xy.y, (curr->radius * pixelWidth));
                chain.add(pt);
                curr->seen = true;

                auto neighbor_iter = std::find_if(
                    curr->neighbors.begin(),
                    curr->neighbors.end(),
                    [](const std::pair<coord, skNode *> &p) -> bool
                    {
                        return p.second->seen == false;
                    });

                if (neighbor_iter == curr->neighbors.end())
                {
                    break;
                }
                curr = neighbor_iter->second;
            }
            if (span > 1)
            {
                skChain interpolated;
                chain.interpolate(span, interpolated);
                chains.push_back(interpolated);
            }
            else
            {
                chains.push_back(chain);
            }
            added++;
        }
    }
    // reset the seen flag
    _resetSeen();
    return added;
}

int skGraph::getChains(
    std::vector<pearlChain> &chains,
    int span)

{
    if (span < 1)
    {
        span = 1;
    }

    float pixelWidth = 2.0 / float(m_width);

    MFloatVector half(m_width * 0.5, m_height * 0.5);

    MFloatMatrix norm;
    norm.setToIdentity();
    norm[0][0] = half.x;
    norm[1][1] = -half.y;
    norm[3][0] = half.x;
    norm[3][1] = half.y;

    int added = 0;

    MFloatMatrix transformation = norm.inverse();
    /*
    Loop through nodes, looking for endpoints that we havent seen yet.
    */

    for (std::map<coord, skNode *>::const_iterator mapiter = m_nodes.begin();
         mapiter != m_nodes.end();
         mapiter++)
    {
        // identify the start of a chain with at least 2 nodes that has not been seen
        if (mapiter->second->isEnd() && (!mapiter->second->seen))
        {

            // make a chain and set both ends to seen
            pearlChain chain;
            skNode *curr = mapiter->second;
            int count = 0;
            for (;; count++)
            {

                MFloatVector xy = curr->c * transformation;
                pearl pt(xy.x, xy.y, (curr->radius * pixelWidth));
                chain.add(pt);
                curr->seen = true;

                auto neighbor_iter = std::find_if(
                    curr->neighbors.begin(),
                    curr->neighbors.end(),
                    [](const std::pair<coord, skNode *> &p) -> bool
                    {
                        return p.second->seen == false;
                    });

                if (neighbor_iter == curr->neighbors.end())
                {
                    break;
                }
                curr = neighbor_iter->second;
            }
            if (span > 1)
            {
                pearlChain interpolated;
                chain.interpolate(span, interpolated);
                chains.push_back(interpolated);
            }
            else
            {
                chains.push_back(chain);
            }
            added++;
        }
    }
    // reset the seen flag
    _resetSeen();
    return added;
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
                    [](const std::pair<coord, skNode *> &p) -> bool
                    { return p.second->seen == false; });
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

void skGraph::trimToLongestChain()
{

    TWIG_CLUSTER walks;

    // Initialize the TWIGS with all the ends nodes.
    for (std::map<coord, skNode *>::const_iterator iter = m_nodes.begin();
         iter != m_nodes.end();
         iter++)
    {
        if (iter->second->isEnd())
        {
            TWIG walk;
            iter->second->seen = true;
            walk.push_back(iter->second); // The end node pointer.
            walks.push_back(walk);
        }
    }

    // Now itrate all twigs from the ends in parallel.
    while (true)
    {

        bool finished = true;
        // Walk all twigs
        for (TWIG_CLUSTER::iterator pTwig = walks.begin(); pTwig != walks.end(); pTwig++)
        {
            if (pTwig->size() < 1)
            {
                // It's been emptied - ignore;
                continue;
            }
            finished = false;

            skNode *pCurr = pTwig->back();
            pCurr->seen = true;

            std::vector<skNode *> currNeighbors;
            pCurr->getUnseenNeighbors(currNeighbors);
            // Should be 1 or 0 unseen neighbors.
            int numNeighbors = currNeighbors.size();
            if (numNeighbors == 0)
            {
                // We hit the end, which is to say, we hit another twig coming the other way.
                // Clear this twig but don't prune from the map.
                pTwig->clear();
            }
            else if (numNeighbors == 1)
            {
                // There is a neighbor to look at
                // If it is a junction, then we are not one of the last two to arrive here, since all
                // previous arrivals would have deleted themselves.
                skNode *pNext = currNeighbors[0];
                int degree = pNext->neighbors.size();
                if (degree > 2)
                {
                    // Its a junction, so prune and clear
                    _pruneTwig(*pTwig, pNext);
                    pTwig->clear();
                }
                else
                {
                    // move one step along
                    pNext->seen = true;
                    pTwig->push_back(pNext);
                }
            }
            else
            {
                cerr << "WE SHOULD NOT BE HERE" << endl;
                finished = true;
            }
        }

        if (finished)
        {
            break;
        }
    }
    _resetSeen();
}

void skGraph::extendLeaves(float amount, int accuracy)
{

    TWIG_MAP twigMap;

    for (std::map<coord, skNode *>::iterator iter = m_nodes.begin();
         iter != m_nodes.end();
         iter++)
    {
        if (iter->second->isEnd())
        {
            skNode *node = iter->second;
            const coord &startCoord = iter->first;
            MFloatVector extension = _getEndDirection(node, accuracy);
            if (extension.isEquivalent(MVector::zero))
            {
                continue;
            }
            extension *= (amount * node->radius);
            coord endCoord = startCoord + coord(round(extension.x), round(extension.y), 0);

            TWIG twig;
            // generateTwig(node, endCoord, twig);
            bresenham(node, endCoord, twig);
            twigMap[node] = twig;
            // add nodes to the twig until we are beyond the length defined by node radius*amount.

            // // MFloatVector direction = iter->second->getEndDirection();
            // for (int i = 0; i < accuracy; i++)
            // {
            //     // iter->second->addEndPoint(direction * amount);
            //     iter->second->addEndPoint(amount);
            // }

            // iter->second->extendLeaf(amˆount,accuracy);
        }
    }

    for (TWIG_MAP::iterator miter = twigMap.begin(); miter != twigMap.end(); miter++)
    {
        skNode *lastNode = miter->first;

        TWIG twig = miter->second;
        cerr <<  "----------------------";
        cerr <<  twig.size() << " twig connections for " << miter->first->c << endl;
        for (TWIG::iterator viter = twig.begin(); viter != twig.end(); viter++)
        {
            cerr <<  (*viter)->c << endl;
            _connect((*viter)->c, lastNode->c);
            lastNode = *viter;
        }
    }
    _resetSeen();
}


void skGraph::bresenham(const skNode *node, const coord &endCoord, TWIG &twig)
{
    const coord &startCoord = node->c;
    int x1 = startCoord.x;
    int y1 = startCoord.y;
    int x2 = endCoord.x;
    int y2 = endCoord.y;
    float radius = node->radius;

   int m_new = 2 * (y2 - y1);
   int slope_error_new = m_new - (x2 - x1);
   for (int x = x1, y = y1; x <= x2; x++)
   {
        if (!((x == x1) && (y == y1)))
        {
            skNode *newNode = _addNode(x, y, 9, radius);
            twig.push_back(newNode);
        }

    //   cout << "(" << x << "," << y << ")\n";
   
      // Add slope to increment angle formed
      slope_error_new += m_new;
   
      // Slope error reached limit, time to
      // increment y and update slope error.
      if (slope_error_new >= 0)
      {
         y++;
         slope_error_new  -= 2 * (x2 - x1);
      }
   }
}
   


// Bressenhams line algorithm
void skGraph::generateTwig(const skNode *node, const coord &endCoord, TWIG &twig)
{
    const coord &startCoord = node->c;
    int x0 = startCoord.x;
    int y0 = startCoord.y;
    int x1 = endCoord.x;
    int y1 = endCoord.y;
    float radius = node->radius;

    int dx, dy, p, x, y;
    dx = x1 - x0;
    dy = y1 - y0;
    x = x0;
    y = y0;
    p = 2 * dy - dx;
    // bool started = false;

    while (x <= x1)
    {
        // if (x > m_width || y > m_height || x < 0 || y < 0)
        // {
        //     break;
        // }

        // We dont want the x0 and y0 node to be added to the twig because it is the original end node.
        if (!((x == x0) && (y == y0)))
        {
            skNode *newNode = _addNode(x, y, 9, radius);
            twig.push_back(newNode);
        }
        if (p >= 0)
        {
            // putpixel(x, y, 7);
            y = y + 1;
            p = p + 2 * dy - 2 * dx;
        }
        else
        {
            // putpixel(x, y, 7);
            p = p + 2 * dy;
        }
        x = x + 1;
    }
}


MFloatVector skGraph::_getEndDirection( skNode *node, int steps) const
{
    // Walk the given number of steps from the end node, then calculate the difference.
    // This is the direction of the end of the twig.
    // Return the normalized direction.
    MFloatVector result = MFloatVector::zero;
    const skNode *lastNode = 0;
    coord currCoord = node->c;
    skNode *currNode = node;

    cerr << "Getting end direction for node " << node->c << endl;
    for (int i = 0; i < steps; i++)
    {
        currNode->seen = true;
        std::vector<skNode *> neighbors;
        int count = currNode->getUnseenNeighbors(neighbors);
        // int count = currNode->getNeighborsExcluding(lastNode, neighbors);
        // lastNode = currNode;
        if (count == 1)
        {
            currNode = neighbors[0];
            currCoord = currNode->c;
        }
        else
        {
            cerr << "_getEndDirection: break on step" << i << endl;
            break;
        }
    }
    if (currNode != node)
    {
        coord diff = node->c - currNode->c;
        result = MFloatVector(diff.x, diff.y, 0).normal();
        cerr << "Getting end direction for node " << node->c << " -- Direction:" << result << endl;
    } else {
        cerr << "Didn't traverse" << endl;
    }
    return result;
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
                    [](const std::pair<coord, skNode *> &p) -> bool
                    { return p.second->seen == false; });

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
