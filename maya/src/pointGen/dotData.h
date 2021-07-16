
#ifndef _dotData
#define _dotData


#include <vector>
#include <map>

#include <maya/MFloatVector.h>
#include <maya/MFloatMatrix.h>

// #include "errorMacros.h"
#include "mayaMath.h"
#include "vec2d.h"

typedef mayaMath::axis axis;

class dotData {
public:

	// dotData(const JPoint2D &c, float r, int id);
	// dotData();

	// dotData(const JPoint2D &c, float r, int id);


	dotData(float u, float v, float density,
	        float radius, int id);
	dotData(const dotData &rhs);

	// sets the UVs based on points in 0->1
	void setUV();

	~dotData();


	// void setUV(u, v);
	// void setPosition(const MFloatMatrix &projection);
	// void setRadius(float r);
	// void setDensity(float val);
	// void setAux(float val);


	void setId(int id);
	// bool isInProjection(const MFloatMatrix &projectionInverse) const;

	const JPoint2D &position() const;
	const float &position(axis a) const;
	const float &radius() const;
	const float &min(axis a)  const;
	const float &max(axis a)  const;

	const float &u()  const;
	const float &v()  const;

	MVector asVector() const;

	bool contains(const dotData *other) const;

	void applyForce(const JVector2D &force);


	const int &id() const;


private:
	JPoint2D m_p;
	JPoint2D m_min;
	JPoint2D m_max;
	float m_radius;
	float m_density;
	float m_u;
	float m_v;
	int m_id;


};

typedef std::vector<dotData *> PT_LIST;
// typedef std::map<const dotData *, const dotData *> PT_MAP;

#endif

