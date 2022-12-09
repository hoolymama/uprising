
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

class dotData
{
public:

	dotData(float u, float v, float density,
			float radius, int id);
	dotData(const dotData &rhs);

	// sets the UVs based on points in 0->1
	void setUV();
	void setGradient(float x, float y);

	~dotData();


	void setId(int id);

	const JPoint2D &position() const;
	const float &position(axis a) const;
	const float &radius() const;
	const float &min(axis a) const;
	const float &max(axis a) const;

	const float &u() const;
	const float &v() const;

	MVector asVector() const;
	MVector transformed(const MMatrix &mat) const;
	MVector gradient() const;
	MVector gradientTransformed(const MMatrix &mat) const;

	bool contains(const dotData *other) const;

	void applyForce(const JVector2D &force);
	
	void setSortParam(const MMatrix &worldMatrix, const MVector&direction);
	const float & sortParam() const; 
	const int &id() const;
	friend bool operator<(const dotData &a, const dotData &b);

private:
	JPoint2D m_p;
	JVector2D m_gradient;
	JPoint2D m_min;
	JPoint2D m_max;
	float m_radius;
	float m_density;
	float m_u;
	float m_v;
	int m_id;
	float m_sortParam;

};

typedef std::vector<dotData *> PT_LIST;

#endif
