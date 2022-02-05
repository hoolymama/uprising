#ifndef _pearlChain_H
#define _pearlChain_H
#include <vector>

#include <maya/MVectorArray.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatArray.h>

#include "pearl.h"

class pearlChain
{
public:
	pearlChain();

	~pearlChain();

	int size() const;

	void add(const pearl &pt);

	void appendPointsTo(MPointArray &positions) const;

	void appendPointsTo(MVectorArray &positions) const;

	const pearl &operator[](const int index) const;

	void appendRadiiTo(MFloatArray &radii) const;
	void appendRadiiTo(MDoubleArray &radii) const;

	void appendParamsTo(MDoubleArray &params) const;

	void interpolate(int step, pearlChain &result) const;

	const float &maxRadius() const;
	const float &minRadius() const;

	const std::vector<pearl> &points() const;

private:
	std::vector<pearl> m_points;

	float m_maxRadius;
	float m_minRadius;
};
#endif
