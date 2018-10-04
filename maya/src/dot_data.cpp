/***************************************************************************

***************************************************************************/

#include <dot_data.h>

// dotData::dotData()
// 	: m_p(),
// 	  m_radius(0.0),
// 	  m_id(),
// 	  m_u(),
// 	  m_v(),
// 	  m_min(),
// 	  m_max()
// {}


dotData::dotData(const MFloatMatrix &projection, float u, float v, float radius,
                 const  MFloatVector &densityVals, int id)
{
	m_u = u;
	m_v = v;

	MFloatPoint p = MFloatPoint(
	                  (m_u * 2.0f) - 1.0f,
	                  (m_v * 2.0f) - 1.0f,
	                  0.0
	                ) * projection;

	m_p = JPoint2D(p.x, p.y);
	m_radius = radius * densityVals.y;
	m_density = densityVals.x;
	m_aux =  densityVals.z;
	m_id = id;
	JPoint2D rad2d(m_radius, m_radius);
	m_min = m_p - rad2d;
	m_max = m_p + rad2d;
}

// dotData::dotData(const JPoint2D &c, float r, int id)
// 	: m_p(c.x, c.y),
// 	  m_radius(r),
// 	  m_id(id),
// 	  m_u(0.0f),
// 	  m_v(0.0f),
// 	  m_min(c.x - r, c.y - r),
// 	  m_max(c.x + r , c.y + r)
// {}

// copy
dotData::dotData(const dotData &rhs)
{
	m_p = rhs.m_p;
	m_radius = rhs.m_radius;
	m_density = rhs.m_density;
	m_aux = rhs.m_aux;

	m_id = rhs.m_id;
	m_u =  rhs.m_u;
	m_v =  rhs.m_v;
	m_min = rhs.m_min;
	m_max = rhs.m_max;
}

dotData::~dotData() {}

// void dotData::setId(int id)
// {
// 	m_id = id;
// }

// void dotData::setUV(float u, float v)
// {
// 	m_u = u;
// 	m_v = v;
// }

// void dotData::setPosition(const MFloatMatrix &projection)
// {
// 	MFloatPoint p = MPoint(m_u, m_v, 0.0) * projection;
// 	m_p = JPoint2D(p.x, p.y);
// 	m_min = JPoint2D(m_p.x - r, m_p.y - r),
// 	m_max = JPoint2D(m_p.x + r, m_p.y + r),
// }

// void dotData::setRadius(float r)
// {
// 	m_radius = r;
// 	m_min = JPoint2D(m_p.x - r, m_p.y - r);
// 	m_max = JPoint2D(m_p.x + r, m_p.y + r);
// }

// void dotData::setDensity(float val)
// {
// 	m_density= val;
// }
// void dotData::setAux(float val)
// {
// 	m_aux= val;
// }


void dotData::push(const JVector2D &force)
{
	m_p += force;
	JVector2D rad2d(m_radius, m_radius);
	m_min = m_p - rad2d;
	m_max = m_p + rad2d;
}

MVector dotData::asVector() const
{
	return MVector(m_p.x, m_p.y, 0.0);
}

const JPoint2D &dotData::position() const {
	return m_p;
}

const float &dotData::position(axis a) const {
	return (a == mayaMath::xAxis) ? m_p.x : m_p.y;
}

const float &dotData::radius()  const {return m_radius; }

const float &dotData::min(axis a)  const {
	return m_min[a];
}

const float &dotData::max(axis a)  const {
	return m_max[a] ;
}

const int &dotData::id() const {return m_id;}

bool dotData::contains(const dotData *other) const {
	float sqdist = (other->position() - m_p).sqlength();
	return (sqdist < (m_radius * m_radius));
}
