
#include "strokeGeom.h"
#include "errorMacros.h"



MFloatPoint extractPos(const MMatrix &mat) {
	return MFloatVector(mat[3][0], mat[3][1], mat[3][2]);
}

// strokeGeom::strokeGeom():
// 	m_startApproach(),
// 	m_endApproach(),
// 	m_targets(),
// 	m_tangents(),
// 	m_arcLength(0.0),
// 	m_direction(1),
// 	m_planeNormal()
// {}

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

	m_direction = src.direction();
	m_arcLength = src.arcLength();

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




void strokeGeom::getPoints(MFloatPointArray &result, double stackHeight) const {
	MFloatVector stackOffset = MFloatVector(m_planeNormal * stackHeight);
	unsigned len = m_targets.length();
	result.setLength(len);
	for (int i = 0; i < len; ++i)
	{
		result.set(extractPos(m_targets[i]) + stackOffset , i);
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
                            double brushWidth, bool withLift, double stackHeight) const {
	MFloatVector stackOffset = MFloatVector(m_planeNormal * stackHeight);
	unsigned len = m_targets.length();
	lefts.clear();
	rights.clear();
	double width = brushWidth * 0.5;
	int first = 0;
	int end = len;

	if (!withLift) {
		first = 1;
		end = len - 1;
	}

	for (int i = first; i < end; ++i)
	{
		MFloatVector projectedX = MFloatVector(((MVector::xAxis * m_targets[i]) ^ m_planeNormal)^
		                                       m_planeNormal).normal();
		const MMatrix &m = m_targets[i];
		MFloatPoint c = extractPos(m_targets[i]) + stackOffset;
		projectedX *= width;
		lefts.append( c + projectedX);
		rights.append(c - projectedX);
	}
}


void strokeGeom::getApproaches(MFloatPointArray &startApproachPoints,
                               MFloatPointArray &endApproachPoints, double stackHeight) const {

	MFloatVector stackOffset = MFloatVector(m_planeNormal * stackHeight);

	startApproachPoints.setLength(3);
	endApproachPoints.setLength(3);

	startApproachPoints.set(extractPos(m_targets[1]) + stackOffset, 0);
	startApproachPoints.set(extractPos(m_targets[0]) + stackOffset, 1);
	startApproachPoints.set(extractPos(m_startApproach) + stackOffset, 2);

	unsigned len = m_targets.length();
	endApproachPoints.set(extractPos(m_targets[(len - 2)]) + stackOffset, 0);
	endApproachPoints.set(extractPos(m_targets[(len - 1)]) + stackOffset, 1);
	endApproachPoints.set(extractPos(m_endApproach) + stackOffset, 2);

}

void strokeGeom::getFullPath(MFloatPointArray &points, double stackHeight) const {
	MFloatVector stackOffset = MFloatVector(m_planeNormal * stackHeight);


	unsigned len = m_targets.length();

	points.append(extractPos(m_startApproach) + stackOffset);
	for (int i = 0; i < len; ++i)
	{
		points.append(extractPos(m_targets[i]) + stackOffset);
	}
	points.append(extractPos(m_endApproach) + stackOffset);
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



