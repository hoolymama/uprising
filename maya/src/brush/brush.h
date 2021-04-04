
#ifndef _brush_
#define _brush_

#include <maya/MFloatMatrix.h>
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
		const MFloatMatrix &matrix,
		int physicalId,
	
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

	~Brush();

	MFloatMatrix tcp() const;

	void getTriangles(MFloatPointArray &triangles) const;

	const float &retention() const;
	const MFloatMatrix &matrix() const;

	/**
	 * @brief Return a value representing how far away the brush should start from the ground.

	 It is the product of the bristle height, the TCP param, and a multiplier (transHeightParam)
	 * 
	 * @return float 
	 */
	float transitionHeight() const;
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
	MFloatMatrix m_matrix;
	int m_physicalId;
	MFloatVector m_tip;
	float m_width;
	float m_bristleHeight;
	float m_retention;
	float m_tcpParam;
	Shape m_shape;
	float m_transHeightParam;
	float m_contactPower;
	float m_forwardBias0; //> The distance to move along the stroke to compensate for brush bend at weight=0
	float m_forwardBias1; //> The distance to move along the stroke to compensate for brush bend at weight=1
	float m_gravityBias0;
	float m_gravityBias1;
};



#endif
