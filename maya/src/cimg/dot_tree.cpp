
#define P_TOLERANCE 0.00000001

#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFloatVector.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MIntArray.h>
#include <dot_tree.h>
#include "vec2d.h"


const float bigNum = 99999999999.0  ;

dotTree::dotTree()
	: m_pPerm(0),
	  m_pRoot(0),
	  m_maxPointsPerBucket(4)
{
	m_pPerm = new PT_LIST;
	m_pClean = new PT_LIST;
	m_pRoot = 0;
};


dotTree::~dotTree()
{
	// delete the elements in the permutations array
	if (m_pPerm) {
		PT_LIST::iterator p = m_pPerm->begin();
		while (p != m_pPerm->end()) {
			delete *p;
			*p = 0;
			p++;
		}
		// then delete the array itself
		delete m_pPerm;
		m_pPerm = 0;
	}
	if (m_pClean) {
		delete m_pClean;
		m_pClean = 0;
	}

	makeEmpty(m_pRoot);  // recursively delete tree
};

void dotTree::makeEmpty(dotTreeNode *p) {

	if (p) {
		if (!(p->bucket)) { // if its not a bucket node
			makeEmpty(p->loChild);	// delete child nodes recursively
			makeEmpty(p->hiChild);
		}
		delete p;            // then delete this node
		p = 0;
	}
}


MStatus dotTree::populate( const std::vector<dotData> &dots)
{
	// unsigned int pl = p.length();

	std::vector<dotData>::const_iterator citer ;
	for (citer = dots.begin() ; citer != dots.end(); citer++)
	{
		dotData *pp = new dotData(*citer);
		m_pPerm->push_back(pp);
	}
	*m_pClean = *m_pPerm;
	return MS::kSuccess;
}

// MStatus dotTree::populate( const MFloatPointArray &p)
// {
// 	unsigned int pl = p.length();
// 	for (unsigned int i = 0; i < pl; i++) {
// 		JPoint2D p2d(p[i].x, p[i].y)
// 		dotData *pp = new dotData(p2d, , i);
// 		m_pPerm->push_back(pp);
// 	}
// 	*m_pClean = *m_pPerm;
// 	return MS::kSuccess;
// }

// MStatus dotTree::populate(const MFloatPointArray &p, const MIntArray &indices)
// {
// 	unsigned int pl = p.length();
// 	for (unsigned int i = 0; i < pl; i++) {
// 		dotData *pp = new dotData(p[i], indices[i]);
// 		m_pPerm->push_back(pp);
// 	}
// 	*m_pClean = *m_pPerm;
// 	return MS::kSuccess;
// }

const int  dotTree::size() const {
	return m_pPerm->size();
}

MStatus dotTree::build() {
	m_pRoot = build( 0, (m_pPerm->size() - 1) );

	if (!m_pRoot) {
		return MS::kUnknownParameter;
	}

	PT_LIST::iterator p = m_pPerm->begin();
	while (p != m_pPerm->end()) {
		setOverlapList(m_pRoot , *p);
		p++;
	}

	return MS::kSuccess;
}

// MStatus dotTree::build(bool doOverlapList) {
// 	m_pRoot = build( 0, (m_pPerm->size() - 1) );

// 	if (!m_pRoot) {
// 		return MS::kUnknownParameter;
// 	}
// 	if (doOverlapList) {
// 		PT_LIST::iterator p = m_pPerm->begin();
// 		while (p != m_pPerm->end()) {
// 			setOverlapList(m_pRoot , *p);
// 			p++;
// 		}
// 	}
// 	return MS::kSuccess;
// }

dotTreeNode   *dotTree::build( int low,  int high	) {

	dotTreeNode *p = new dotTreeNode;

	if ( unsigned((high - low) + 1) <= m_maxPointsPerBucket) {   // stop recursing

		p->bucket = true;
		p->loPoint = low;
		p->hiPoint = high;
		p->loChild = 0;
		p->hiChild = 0;
		p->overlapList = new PT_LIST;
	}
	else {
		p->bucket = false;
		p->cutAxis = findMaxAxis(low, high);
		int mid = ((low + high) / 2);
		wirthSelect(low, high, mid, p->cutAxis);
		p->cutVal = float(((*m_pPerm)[mid])->position(p->cutAxis));
		p->loChild = build(low, mid);
		p->hiChild = build(mid + 1, high);
	}
	return p;
}

void  dotTree::setOverlapList(dotTreeNode *p,  dotData *dot  )  {
	// recursive setOverlapList method
	// put dot in every bucket it overlaps
	if (p->bucket) {
		p->overlapList->push_back(dot);
	}
	else {
		if (dot->min(p->cutAxis) < p->cutVal) {
			setOverlapList(p->loChild, dot);
		}
		if (dot->max(p->cutAxis) > p->cutVal) {
			setOverlapList(p->hiChild, dot);
		}
	}
}



const dotTreeNode *dotTree::root() const {
	return m_pRoot;
};


PT_LIST *dotTree::perm() {
	return m_pPerm;
};

PT_LIST *dotTree::dots() {
	return m_pClean;
};


// a selection algorithm.
void dotTree::wirthSelect(  int left,  int right,  int k, axis cutAxis )
{
	int n = (right - left) + 1;
	if (n <= 1) { return; }
	int i, j, l, m;
	dotData *x;
	dotData *tmp;

	l = left;
	m = right;
	while (l < m) {
		x = (*m_pPerm)[k];
		i = l;
		j = m;
		do {
			while (  ((*m_pPerm)[i])->position(cutAxis) <  x->position(cutAxis)  ) { i++; }
			while (  ((*m_pPerm)[j])->position(cutAxis) >  x->position(cutAxis)  ) { j--; }

			if (i <= j) {
				tmp = (*m_pPerm)[i];
				(*m_pPerm)[i] = (*m_pPerm)[j] ;
				(*m_pPerm)[j] = tmp ;
				i++; j--;
			}
		}
		while (i <= j);
		if (j < k) { l = i; }
		if (k < i) { m = j; }
	}
}



// find the axis containing the longest
// side of the bounding rectangle of the points
// estimate using a subset of the points

const axis dotTree::findMaxAxis(  int low,   int high) const {

	int  num = (high - low ) + 1;
	int interval = int(sqrt(float(num)));
	int i;
	JVector2D p = ((*m_pPerm)[low])->position();

	float minx , miny,  maxx , maxy , tmpVal;
	float sx, sy;

	minx = p.x;
	maxx = minx;
	miny = p.y;
	maxy = miny;

	for (i = (low + interval); i <= high; i += interval ) {
		p = ((*m_pPerm)[i])->position();
		tmpVal = p.x;
		if (tmpVal < minx) {
			minx = tmpVal;
		}
		else {
			if (tmpVal > maxx) {
				maxx = tmpVal;
			}
		}
		tmpVal = p.y;
		if (tmpVal < miny) {
			miny = tmpVal;
		}
		else {
			if (tmpVal > maxy) {
				maxy = tmpVal;
			}
		}
	}

	sx = maxx - minx;
	sy = maxy - miny;

	if (sx > sy) {
		return mayaMath::xAxis;
	}
	else {
		return mayaMath::yAxis;
	}
}


void  dotTree::strongestNPoints(
  const dotTreeNode *p,
  const dotData *searchDot,
  KNN_PD_QUEUE *q
)  const  {
	// this method finds n strongest N points within a given radius -
	// in other words, for each potential close neighbor, we look at
	// the ratio of their interaction (pos1 - pos2).length() / (rad1 + rad2)
	if (!p) { return; }
	if (p->bucket) {

		const PT_LIST *overlapList = p->overlapList;
		PT_LIST::const_iterator curr;
		curr = overlapList->begin();
		while (curr != overlapList->end()) {
			/// TODO - Test timings for checking overlap list before checking separation
			//if (! ((*curr)->wasVisitedBy(searchDot) || (*curr == searchDot))) {
			// (*curr)->setLastVisitBy(searchDot) ;
			JVector2D diff = searchDot->position() - (*curr)->position();
			float combinedRadii = ((*curr)->radius() + searchDot->radius());
			float sqlen = diff.sqlength();
			if (sqlen < (combinedRadii * combinedRadii)) {
				float length = sqrt(sqlen);
				float weakness = (length / combinedRadii);
				if (weakness < q->top().weakness) {
					//if (visited->find(*curr) == visited->end()) { // not been visited already
					q->pop();
					knnSearchPdData ksd;
					ksd.weakness = weakness;
					ksd.length = length;
					ksd.diff = diff;
					ksd.dot =  *curr;
					q->push(ksd)  ;
					//(*visited)[*curr] = *curr;
				}
				//}
			}
			curr++;
		}

	}
	else {
		float diff = searchDot->position(p->cutAxis) -
		             p->cutVal; // distance to the cut wall for this bucket
		if (diff < 0) { // we are in the lo child so search it
			strongestNPoints(p->loChild, searchDot, q);
			if (searchDot->radius() >=
			    -diff) { // if radius overlaps the hi child then search that too
				strongestNPoints(p->hiChild, searchDot, q);
			}
		}
		else {   // we are in the hi child so search it
			strongestNPoints(p->hiChild, searchDot, q);
			if (searchDot->radius() >= diff) { // if radius overlaps the lo child then search that too
				strongestNPoints(p->loChild, searchDot, q);
			}
		}
	}
}


// do a fixed radius search and return the result points in a KNN_PD_QUEUE
void  dotTree::fixedRadiusSearch(
  const dotTreeNode *p,
  const dotData *searchDot,
  KNN_PD_QUEUE *q
)  const  {
	if (!p) { return; }
	if (p->bucket) {
		// searchList( p->overlapList, searchDot, maxNeighbors, result);

		for (unsigned int i = p->loPoint; i <= p->hiPoint; i++ ) {
			dotData *targetDot = (*m_pPerm)[i];
			if (searchDot != targetDot) { // make sure its not me

				JVector2D diff = searchDot->position() - targetDot->position();
				float sqlen = diff.sqlength();
				const float &rad = searchDot->radius();
				if (sqlen < (rad * rad)) {
					float length = sqrt(sqlen);
					float weakness = (length / rad);
					if (weakness < q->top().weakness) {
						q->pop();
						knnSearchPdData ksd;
						ksd.weakness = weakness;
						ksd.length = length;
						ksd.diff = diff;
						ksd.dot =  targetDot;
						q->push(ksd);
					}
				}
			}
		}
	}
	else {
		float diff = searchDot->position(p->cutAxis) -
		             p->cutVal; // distance to the cut wall for this bucket
		if (diff < 0) { // we are in the lo child so search it
			fixedRadiusSearch(p->loChild, searchDot, q);
			if (searchDot->radius() >=
			    -diff) { // if radius overlaps the hi child then search that too
				fixedRadiusSearch(p->hiChild, searchDot, q);
			}
		}
		else {   // we are in the hi child so search it
			fixedRadiusSearch(p->hiChild, searchDot, q);
			if (searchDot->radius() >= diff) { // if radius overlaps the lo child then search that too
				fixedRadiusSearch(p->loChild, searchDot, q);
			}
		}
	}
}

// do a point search and return true if the
// point is within the radius of any particle in the tree
// bool  dotTree::pointIsOccupied(
//   const dotTreeNode *p,
//   const dotData *searchDot
// )  const  {
// 	if (!p) { return false; }
// 	if (p->bucket) {

// 		const PT_LIST *overlapList = p->overlapList;
// 		PT_LIST::const_iterator curr;
// 		curr = overlapList->begin();
// 		while (curr != overlapList->end()) {
// 			if ((*curr)->contains(searchDot)) {
// 				return true;
// 			}
// 			curr++;
// 		}
// 	}
// 	else {
// 		float diff = searchDot->position(p->cutAxis) -
// 		             p->cutVal; // distance to the cut wall for this bucket
// 		if (diff < 0) { // we are in the lo child so search it
// 			return pointIsOccupied(p->loChild, searchDot);
// 		}
// 		else {   // we are in the hi child so search it
// 			return pointIsOccupied(p->hiChild, searchDot);
// 		}
// 	}
// 	return false;
// }

// void dotTree::emptinessAtPoint(
//   const dotTreeNode *p,
//   const dotData *searchDot,
//   float &emptiness
// )  const  {
// 	// this method traverses the ovelerlap list for the bucket it is in,
// 	// and emptinesss the weakness of containing particles.
// 	// The result is 0 for a heavily populated area, and 1.0 for an empty area
// 	if (!p) { return; }
// 	if (p->bucket) {

// 		const PT_LIST *overlapList = p->overlapList;
// 		PT_LIST::const_iterator curr;
// 		curr = overlapList->begin();
// 		while (curr != overlapList->end()) {
// 			emptiness *= (*curr)->emptinessAtPoint(searchDot);
// 			curr++;
// 		}

// 	}
// 	else {
// 		float diff = searchDot->position(p->cutAxis) -
// 		             p->cutVal; // distance to the cut wall for this bucket
// 		if (diff < 0) { // we are in the lo child so search it
// 			emptinessAtPoint(p->loChild, searchDot, emptiness);
// 		}
// 		else {   // we are in the hi child so search it
// 			emptinessAtPoint(p->hiChild, searchDot, emptiness);
// 		}
// 	}
// }


// void  dotTree::overlappingEmptiness(
//   const dotTreeNode *p,
//   const dotData *searchDot,
//   float &emptiness
// )  const  {
// 	if (!p) { return; }
// 	if (p->bucket) {

// 		const PT_LIST *overlapList = p->overlapList;
// 		PT_LIST::const_iterator curr;
// 		curr = overlapList->begin();
// 		while (curr != overlapList->end()) {
// 			if (! (*curr)->wasVisitedBy(searchDot->id()) ) {
// 				(*curr)->setLastVisitBy(searchDot->id()) ;
// 				emptiness *= (*curr)->overlappingEmptiness(searchDot);
// 			}
// 			curr++;
// 		}
// 	}
// 	else {
// 		float diff = searchDot->position(p->cutAxis) -
// 		             p->cutVal; // distance to the cut wall for this bucket
// 		if (diff < 0) { // we are in the lo child so search it
// 			overlappingEmptiness(p->loChild, searchDot, emptiness);
// 			if (searchDot->radius() >=
// 			    -diff) { // if radius overlaps the hi child then search that too
// 				overlappingEmptiness(p->hiChild, searchDot, emptiness);
// 			}
// 		}
// 		else {   // we are in the hi child so search it
// 			overlappingEmptiness(p->hiChild, searchDot, emptiness);
// 			if (searchDot->radius() >= diff) { // if radius overlaps the lo child then search that too
// 				overlappingEmptiness(p->loChild, searchDot, emptiness);
// 			}
// 		}
// 	}
// }

