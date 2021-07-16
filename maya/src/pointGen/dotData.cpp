/***************************************************************************

***************************************************************************/

#include <dotData.h>

 

dotData::dotData(float u, float v, float density,
                 float radius, int id):
	m_u(u),
	m_v(v),
	m_density(density),
	m_radius(radius),
	m_id(id)

{

	MFloatPoint p = MFloatPoint(
	                  (m_u * 2.0f) - 1.0f,
	                  (m_v * 2.0f) - 1.0f,
	                  0.0
	                ) ;

	m_p = JPoint2D(p.x, p.y);

	JPoint2D rad2d(m_radius, m_radius);
	m_min = m_p - rad2d;
	m_max = m_p + rad2d;
}


void dotData::setUV()
{
	m_u = (m_p.x * 0.5);
	m_v = (m_p.y * 0.5);
}

 

// copy
dotData::dotData(const dotData &rhs)
{
	m_p = rhs.m_p;
	m_radius = rhs.m_radius;
	m_density = rhs.m_density;
	m_id = rhs.m_id;
	m_u =  rhs.m_u;
	m_v =  rhs.m_v;
	m_min = rhs.m_min;
	m_max = rhs.m_max;
}

dotData::~dotData() {}

 

void dotData::applyForce(const JVector2D &force)
{

	// m_p += force;
	JPoint2D p = m_p + force;
	MFloatPoint pnorm = MFloatPoint(p.x, p.y, 0.0f);
	bool apply = true;
	if (pnorm.x < -1.0) {return;}
	if (pnorm.y < -1.0) {return;}
	if (pnorm.x >  1.0) {return;}
	if (pnorm.y >  1.0) {return;}
 
	m_p = p;
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

const float &dotData::u()  const {
	return m_u;
}
const float &dotData::v()  const {
	return m_v;
}

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
