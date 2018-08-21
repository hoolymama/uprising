
#include "strokeGeom.h"
#include "errorMacros.h"

strokeGeom::strokeGeom():
	m_startApproach(),
	m_endApproach(),
	m_targets(),
	m_tangents(),
	m_arcLength(0.0),
	m_direction(1),
	m_planeNormal()
{}

strokeGeom::strokeGeom(const Stroke &src):
	m_startApproach(),
	m_endApproach(),
	m_targets(),
	m_tangents(),
	m_planeNormal(src.planeNormal())
{
	src.appendTargets(m_targets);
	src.appendTangents(m_tangents);
	src.getApproachTargets(m_startApproach, m_endApproach);
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
const MVector &strokeGeom::planeNormal() const {
	return m_planeNormal;
}




void strokeGeom::getPoints(MFloatPointArray &result) const {
	unsigned len = m_targets.length();
	result.setLength(len);
	for (int i = 0; i < len; ++i)
	{
		const MMatrix &m = m_targets[i];
		result.set(MFloatPoint(m[3][0], m[3][1], m[3][2]), i);
	}
}

void strokeGeom::getXAxes(MFloatVectorArray &result) const {
	unsigned len = m_targets.length();
	result.setLength(len);
	for (int i = 0; i < len; ++i)
	{
		result.set( (MFloatVector(MVector::xAxis * m_targets[i])) , i);
	}
}

void strokeGeom::getYAxes(MFloatVectorArray &result) const {
	unsigned len = m_targets.length();
	result.setLength(len);
	for (int i = 0; i < len; ++i)
	{
		result.set( (MFloatVector(MVector::yAxis * m_targets[i])) , i);
	}
}

void strokeGeom::getZAxes(MFloatVectorArray &result) const {
	unsigned len = m_targets.length();
	result.setLength(len);
	for (int i = 0; i < len; ++i)
	{
		result.set( (MFloatVector(MVector::zAxis * m_targets[i])) , i);
	}
}

void strokeGeom::getBorders(MFloatPointArray &lefts, MFloatPointArray &rights,
                            double brushWidth) const {
	unsigned len = m_targets.length();
	lefts.setLength(len);
	rights.setLength(len);
	double width = brushWidth * 0.5;
	for (int i = 0; i < len; ++i)

	{
		MFloatVector projectedX = MFloatVector(((MVector::xAxis * m_targets[i]) ^ m_planeNormal)^
		                                       m_planeNormal).normal();
		const MMatrix &m = m_targets[i];
		MFloatPoint c = MFloatPoint(m[3][0], m[3][1], m[3][2]);
		projectedX *= width;
		lefts.set( c + projectedX, i);
		rights.set(c - projectedX, i);
	}
}



/* Is this assignment the same as default. If so remove it. */
// strokeGeom &strokeGeom::operator=( const strokeGeom &other )
// {
// 	if ( &other != this ) {
// 		m_startApproach = other.startApproach() ;
// 		m_endApproach = other.endApproach();
// 		m_targets = other.targets();
// 		m_tangents = other.tangents();
// 		m_arcLength = other.arcLength();
// 		m_direction = other.direction();
// m_planeNormal = other.planeNormal();
// 	}
// 	return *this;
// }



ostream &operator<<(ostream &os, const strokeGeom &g)
{
	MString direction = (g.m_direction == 1) ? "forward" : "back"  ;

	os << " Direction:" <<  direction << ",";
	os << " Targets:" << g.m_targets.length() << ",";
	os << " Arc length:" <<  g.m_arcLength;

	return os;
}



