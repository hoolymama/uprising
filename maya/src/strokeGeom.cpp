
#include "strokeGeom.h"
#include "errorMacros.h"

strokeGeom::strokeGeom():
	m_startApproach(),
	m_endApproach(),
	m_targets(),
	m_tangents(),
	m_arcLength(0.0),
	m_direction(1)
{}

strokeGeom::strokeGeom(const Stroke &src, const MVector &planeNormal):
	m_startApproach(),
	m_endApproach(),
	m_targets(),
	m_tangents()
{
	src.appendTargets(planeNormal, m_targets);
	src.appendTangents(m_tangents);
	src.getApproachTargets(planeNormal, m_startApproach, m_endApproach);
	m_direction =  src.direction();
	m_arcLength =  src.arcLength();

}

strokeGeom::~strokeGeom() {}


const MMatrix &strokeGeom::startApproach() const {
	return m_startApproach;
}
const MMatrix &strokeGeom::endApproach() const {
	return m_endApproach;
}
const MMatrixArray &strokeGeom::targets() const {
	return m_targets;
}
const MVectorArray &strokeGeom::tangents() const {
	return m_tangents;
}
double strokeGeom::arcLength() const {
	return m_arcLength;
}
short strokeGeom::direction() const {
	return m_direction;
}


/* Is this assignment the same as default. If so remove it. */
strokeGeom &strokeGeom::operator=( const strokeGeom &other )
{
	if ( &other != this ) {
		m_startApproach = other.startApproach() ;
		m_endApproach = other.endApproach();
		m_targets = other.targets();
		m_tangents = other.tangents();
		m_arcLength = other.arcLength();
		m_direction = other.direction();
	}
	return *this;
}



ostream &operator<<(ostream &os, const strokeGeom &g)
{
	MString direction = (g.m_direction == 1) ? "forward" : "back"  ;

	os << " Direction:" <<  direction << ",";
	os << " Targets:" << g.m_targets.length() << ",";
	os << " Arc length:" <<  g.m_arcLength;

	return os;
}



