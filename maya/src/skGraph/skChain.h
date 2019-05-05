#ifndef _skChain_H
#define _skChain_H
#include <vector>

#include <maya/MVectorArray.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatArray.h>

#include "skPoint.h"

class skChain
{
public:

	skChain();

	~skChain();

	int size() const;

	void add(const skPoint &pt);

	void appendPointsTo(MPointArray &positions) const;

	void appendPointsTo(MVectorArray &positions) const;

	// int getContacts( MDoubleArray &contacts, double maxWidth) const;


	// int getContacts(
	//   MDoubleArray &contacts,
	//   const std::vector< std::pair<int, float> > &widths) const;


	const skPoint &operator[] (const int index) const ;




	void appendRadiiTo( MFloatArray &radii) const;
	void appendRadiiTo( MDoubleArray &radii) const;

	void appendParamsTo( MDoubleArray &params) const;

	void interpolate(int step, skChain &result) const;


	const float &maxRadius() const;
	const float &minRadius() const;

	const std::vector< skPoint > &points() const;

private:
	std::vector< skPoint > m_points;

	float m_maxRadius;
	float m_minRadius;

};
#endif
