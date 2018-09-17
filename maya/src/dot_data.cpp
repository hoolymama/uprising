/***************************************************************************

***************************************************************************/

#include <dot_data.h>

dotData::dotData(const JPoint2D &c, float r, int id)
	: m_p(c.x, c.y),
	  // m_depth(p.z),
	  m_radius(r),
	  m_id(id),
	  m_min(c.x - r, c.y - r),
	  m_max(c.x + r , c.y + r)
	  // ,
	  // m_pLastVisitBy(0),
	  // m_nLastVisitBy(-1)
{}

dotData::~dotData() {}

const JPoint2D &dotData::position() const {
	return m_p;
}

const float &dotData::position(axis a) const {
	return (a == mayaMath::xAxis) ? m_p.x : m_p.y;
}

const float &dotData::radius()  const {return m_radius; }

// const float &dotData::depth()  const {return m_depth; }

const float &dotData::min(axis a)  const {
	return m_min[a];
}

const float &dotData::max(axis a)  const {
	return m_max[a] ;
}

const int &dotData::id() const {return m_id;}

// bool dotData::wasVisitedBy(const dotData* ptr)  const {
// 	return (m_pLastVisitBy == ptr);
// }
// void dotData::setLastVisitBy(const dotData* other) {
// 	m_pLastVisitBy = other;
// }

// bool dotData::wasVisitedBy(int i) const {
// 	return (m_nLastVisitBy == i);
// }

// void dotData::setLastVisitBy(int i) {
// 	m_nLastVisitBy = i;
// }

bool dotData::contains(const dotData *other) const {
	float sqdist = (other->position() - m_p).sqlength();
	return (sqdist < (m_radius * m_radius));
}

// float dotData::emptinessAtPoint(const dotData* other) const {
// 	float sqdist = (other->position() - m_p).sqlength();
// 	if (sqdist > (m_radius * m_radius)) return 1.0f;
// 	return sqrt(sqdist) / m_radius;
// }
// float dotData::overlappingEmptiness(const dotData* other) const {
// 	float sqdist = (other->position() - m_p).sqlength();
// 	float combinedRadii = m_radius + other->radius();
// 	// cerr << "dist over comb rad" <<  sqrt(sqdist) << " / " << combinedRadii << " = " << sqrt(sqdist) / combinedRadii << endl;
// 	if (sqdist > (combinedRadii * combinedRadii)) {
// 		return 1.0f;
// 	}

// return sqrt(sqdist) / combinedRadii;
// }