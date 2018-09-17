
#ifndef _dotData
#define _dotData


#include <vector>
#include <map>

#include <maya/MFloatVector.h>
#include "errorMacros.h"
#include "mayaMath.h"
#include "vec2d.h"

typedef mayaMath::axis axis;

class dotData {
public:

	dotData(const JPoint2D &c, float r, int id);

	~dotData();

	const JPoint2D &position() const;
	const float &position(axis a) const;
	const float &radius() const;
	const float &depth() const;
	const float &min(axis a)  const;
	const float &max(axis a)  const;

	// bool wasVisitedBy(const dotData *ptr) const;

	// void setLastVisitBy(const dotData *ptr);

	// bool wasVisitedBy(int) const;

	// void setLastVisitBy(int);

	bool contains(const dotData *other) const;

	// float emptinessAtPoint(const dotData *other) const;

	// float overlappingEmptiness(const dotData *other) const ;

	const int &id() const;


private:
	const JPoint2D m_p;
	const JPoint2D m_min;
	const JPoint2D m_max;
	const float m_radius;
	// const float m_depth;
	const int m_id;
	// const dotData* m_pLastVisitBy;
	// int m_nLastVisitBy;

};

typedef std::vector<dotData *> PT_LIST;
typedef std::map<const dotData *, const dotData *> PT_MAP;

#endif

