#include <map>

#include <maya/MString.h>
#include "brush.h"


Brush::Brush() : m_physicalId(-1),
				 m_customId(-1),
				 m_width(1.0f),
				 m_bristleHeight(0.0f),
				 m_retention(1.0f),
				 m_tip(),
				 m_tcpParam(0.0f),
				 m_shape(Brush::kRound),
				 m_transHeightParam(1.0),
				 m_contactPower(1.0),
				 m_forwardBias0(0.0),
				 m_forwardBias1(0.0),
				 m_gravityBias0(0.0),
				 m_gravityBias1(0.0)
{
}

Brush::Brush(
	int physicalId,
	int customId,
	const MFloatVector &tip,
	float bristleHeight,
	float tcpParam,
	float width,
	Shape shape,
	float retention,
	float transHeightParam,
	float contactPower,
	float forwardBias0,
	float forwardBias1,
	float gravityBias0,
	float gravityBias1)
	: m_physicalId(physicalId),
	  m_customId(customId),
	  m_tip(tip),
	  m_bristleHeight(bristleHeight),
	  m_tcpParam(tcpParam),
	  m_width(width),
	  m_shape(shape),
	  m_retention(retention),
	  m_transHeightParam(transHeightParam),
	  m_contactPower(contactPower),
	  m_forwardBias0(forwardBias0),
	  m_forwardBias1(forwardBias1),
	  m_gravityBias0(gravityBias0),
	  m_gravityBias1(gravityBias1)

{
}

Brush::~Brush() {}

MFloatMatrix Brush::tcp() const
{

	MFloatMatrix result;
	result.setToIdentity();

	// MFloatVector p = m_tip;
	// cerr << "m_tip = " << m_tip << endl;

	MFloatVector p = m_tip - MFloatVector(0.0, 0.0, (m_bristleHeight * m_tcpParam));

	// MFloatVector p(m_tip - MFloatVector( 0.0,  0.0, (m_bristleHeight * m_tcpParam)));
	// cerr << "PPP VECTOR = " << p << endl;
	// cerr << "TIP VECTOR = " << m_tip << endl;
	// cerr << "TCP VECTOR = " << p << endl;

	result[3][0] = p.x;
	result[3][1] = p.y;
	result[3][2] = p.z;
	return result;
}

void appendCube(const MFloatPointArray &verts, MFloatPointArray &triangles)
{
	// base
	triangles.append(verts[0]);
	triangles.append(verts[1]);
	triangles.append(verts[3]);

	triangles.append(verts[3]);
	triangles.append(verts[1]);
	triangles.append(verts[2]);

	// front
	triangles.append(verts[0]);
	triangles.append(verts[4]);
	triangles.append(verts[5]);

	triangles.append(verts[0]);
	triangles.append(verts[5]);
	triangles.append(verts[1]);

	// right
	triangles.append(verts[1]);
	triangles.append(verts[5]);
	triangles.append(verts[6]);

	triangles.append(verts[1]);
	triangles.append(verts[6]);
	triangles.append(verts[2]);

	// back
	triangles.append(verts[2]);
	triangles.append(verts[6]);
	triangles.append(verts[7]);

	triangles.append(verts[2]);
	triangles.append(verts[7]);
	triangles.append(verts[3]);

	// left
	triangles.append(verts[3]);
	triangles.append(verts[7]);
	triangles.append(verts[4]);

	triangles.append(verts[3]);
	triangles.append(verts[4]);
	triangles.append(verts[0]);

	// top
	triangles.append(verts[4]);
	triangles.append(verts[7]);
	triangles.append(verts[6]);

	triangles.append(verts[4]);
	triangles.append(verts[6]);
	triangles.append(verts[5]);
}

void Brush::getTriangles(MFloatPointArray &triangles) const
{
	float majorRadius = m_width * 0.5;
	float minorRadius = majorRadius;
	float ferruleHeight = 1.0f;

	if (m_shape == Brush::kFlat)
	{
		minorRadius = majorRadius * 0.2;
	}

	float handleRadius = 0.5f;
	float handleHeight = m_tip.z - (m_bristleHeight + ferruleHeight);
	MFloatPoint baseCenter(m_tip.x, m_tip.y, 0.0);
	MFloatPoint handleTopCenter(m_tip.x, m_tip.y, handleHeight);
	// HANDLE
	MFloatPointArray verts;
	verts.append(baseCenter + MFloatVector(-handleRadius, handleRadius, 0.0));
	verts.append(baseCenter + MFloatVector(-handleRadius, -handleRadius, 0.0));
	verts.append(baseCenter + MFloatVector(handleRadius, -handleRadius, 0.0));
	verts.append(baseCenter + MFloatVector(handleRadius, handleRadius, 0.0));
	verts.append(handleTopCenter + MFloatVector(-handleRadius, handleRadius, 0.0));
	verts.append(handleTopCenter + MFloatVector(-handleRadius, -handleRadius, 0.0));
	verts.append(handleTopCenter + MFloatVector(handleRadius, -handleRadius, 0.0));
	verts.append(handleTopCenter + MFloatVector(handleRadius, handleRadius, 0.0));
	appendCube(verts, triangles);

	// FERRULE
	verts.clear();
	MFloatPoint ferruleTopCenter(m_tip.x, m_tip.y, handleHeight + ferruleHeight);
	verts.append(handleTopCenter + MFloatVector(-handleRadius, handleRadius, 0.0));
	verts.append(handleTopCenter + MFloatVector(-handleRadius, -handleRadius, 0.0));
	verts.append(handleTopCenter + MFloatVector(handleRadius, -handleRadius, 0.0));
	verts.append(handleTopCenter + MFloatVector(handleRadius, handleRadius, 0.0));

	verts.append(ferruleTopCenter + MFloatVector(-majorRadius, minorRadius, 0.0));
	verts.append(ferruleTopCenter + MFloatVector(-majorRadius, -minorRadius, 0.0));
	verts.append(ferruleTopCenter + MFloatVector(majorRadius, -minorRadius, 0.0));
	verts.append(ferruleTopCenter + MFloatVector(majorRadius, minorRadius, 0.0));
	appendCube(verts, triangles);

	// BRISTLE LOWER
	verts.clear();
	MFloatPoint halfBristleTopCenter(m_tip.x, m_tip.y,
									 handleHeight + ferruleHeight + (m_bristleHeight * 0.5));
	verts.append(ferruleTopCenter + MFloatVector(-majorRadius, minorRadius, 0.0));
	verts.append(ferruleTopCenter + MFloatVector(-majorRadius, -minorRadius, 0.0));
	verts.append(ferruleTopCenter + MFloatVector(majorRadius, -minorRadius, 0.0));
	verts.append(ferruleTopCenter + MFloatVector(majorRadius, minorRadius, 0.0));

	verts.append(halfBristleTopCenter + MFloatVector(-majorRadius, minorRadius, 0.0));
	verts.append(halfBristleTopCenter + MFloatVector(-majorRadius, -minorRadius, 0.0));
	verts.append(halfBristleTopCenter + MFloatVector(majorRadius, -minorRadius, 0.0));
	verts.append(halfBristleTopCenter + MFloatVector(majorRadius, minorRadius, 0.0));
	appendCube(verts, triangles);

	// BRISTLE UPPER
	verts.clear();
	MFloatPoint tipCenter(m_tip.x, m_tip.y, m_tip.z);

	float tipMajorRadius = majorRadius;
	float tipMinorRadius = majorRadius * 0.1;
	if (m_shape == Brush::kRound)
	{
		tipMajorRadius = majorRadius * 0.05;
		tipMinorRadius = tipMajorRadius;
	}

	verts.append(halfBristleTopCenter + MFloatVector(-majorRadius, minorRadius, 0.0));
	verts.append(halfBristleTopCenter + MFloatVector(-majorRadius, -minorRadius, 0.0));
	verts.append(halfBristleTopCenter + MFloatVector(majorRadius, -minorRadius, 0.0));
	verts.append(halfBristleTopCenter + MFloatVector(majorRadius, minorRadius, 0.0));

	verts.append(tipCenter + MFloatVector(-tipMajorRadius, tipMinorRadius, 0.0));
	verts.append(tipCenter + MFloatVector(-tipMajorRadius, -tipMinorRadius, 0.0));
	verts.append(tipCenter + MFloatVector(tipMajorRadius, -tipMinorRadius, 0.0));
	verts.append(tipCenter + MFloatVector(tipMajorRadius, tipMinorRadius, 0.0));
	appendCube(verts, triangles);

	// SLAB AT TCP
	MFloatPoint tcpCenter(m_tip.x, m_tip.y, m_tip.z - (m_bristleHeight * m_tcpParam));
	MFloatPoint tcpBaseCenter(m_tip.x, m_tip.y,
							  m_tip.z - ((m_bristleHeight * m_tcpParam) + 0.1));

	float tcpMajorRadius = majorRadius + 0.1;
	float tcpMinorRadius = minorRadius + 0.1;
	verts.clear();

	verts.append(tcpBaseCenter + MFloatVector(-tipMajorRadius, tipMinorRadius, 0.0));
	verts.append(tcpBaseCenter + MFloatVector(-tipMajorRadius, -tipMinorRadius, 0.0));
	verts.append(tcpBaseCenter + MFloatVector(tipMajorRadius, -tipMinorRadius, 0.0));
	verts.append(tcpBaseCenter + MFloatVector(tipMajorRadius, tipMinorRadius, 0.0));

	verts.append(tcpCenter + MFloatVector(-tcpMajorRadius, tcpMinorRadius, 0.0));
	verts.append(tcpCenter + MFloatVector(-tcpMajorRadius, -tcpMinorRadius, 0.0));
	verts.append(tcpCenter + MFloatVector(tcpMajorRadius, -tcpMinorRadius, 0.0));
	verts.append(tcpCenter + MFloatVector(tcpMajorRadius, tcpMinorRadius, 0.0));
	appendCube(verts, triangles);
}

const float &Brush::retention() const
{
	return m_retention;
}

float Brush::transHeight() const
{
	return m_transHeightParam * m_bristleHeight * m_tcpParam;
}

float Brush::contactPower() const
{
	return m_contactPower;
}

float Brush::forwardBias0() const
{
	return m_forwardBias0;
}

float Brush::forwardBias1() const
{
	return m_forwardBias1;
}

float Brush::gravityBias0() const
{
	return m_gravityBias0;
}

float Brush::gravityBias1() const
{
	return m_gravityBias1;
}

int Brush::physicalId() const
{
	return m_physicalId;
}

int Brush::customId() const
{
	return m_customId;
}

const float &Brush::width() const
{
	return m_width;
}

Brush::Shape Brush::shape() const
{
	return m_shape;
}

bool Brush::isFlat() const
{
	return m_shape == Brush::kFlat;
}

bool Brush::matches(Shape filter) const
{
	return (filter == m_shape) || (filter == Brush::kAll);
}

ostream &operator<<(ostream &os, const Brush &b)
{

	MString shapeStr = "flat";
	if (b.m_shape == Brush::kRound)
	{
		shapeStr = "round";
	}

	os << " width:" << b.m_width;
	os << " retention:" << b.m_retention;
	os << " tip:" << b.m_tip;
	os << " physicalId:" << b.m_physicalId;
	os << " customId:" << b.m_customId;
	os << " transHeightParam:" << b.m_transHeightParam;
	os << " contactPower:" << b.m_contactPower;

	os << " shape:" << shapeStr;
	return os;
}
