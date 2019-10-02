
#ifndef _brush_
#define _brush_

#include <map>
#include <maya/MDataBlock.h>
#include <maya/MVector.h>
#include "mayaMath.h"

class Brush
{
public:
	enum Shape
	{
		kFlat,
		kRound,
		kAll
	};

	Brush();

	Brush(
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
		float gravityBias1);

	// Brush &operator=(const Brush &rhs);
	// Brush(const Brush &rhs);

	~Brush();

	MFloatMatrix tcp() const;

	void getTriangles(MFloatPointArray &triangles) const;

	const float &retention() const;
	float transHeight() const;
	float contactPower() const;

	float forwardBias0() const;
	float forwardBias1() const;
	float gravityBias0() const;
	float gravityBias1() const;

	int physicalId() const;

	int customId() const;

	bool matches(Shape filter) const;

	Brush::Shape shape() const;

	const float &width() const;

	bool isFlat() const;

	friend ostream &operator<<(ostream &os, const Brush &b);

private:
	int m_physicalId;
	int m_customId;
	MFloatVector m_tip;
	float m_width;
	float m_bristleHeight;
	float m_retention;
	float m_tcpParam;
	Shape m_shape;
	float m_transHeightParam;
	float m_contactPower;
	float m_forwardBias0;
	float m_forwardBias1;
	float m_gravityBias0;
	float m_gravityBias1;
};
#endif
