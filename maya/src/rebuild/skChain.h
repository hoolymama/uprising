

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

	void appendPoints(MPointArray &positions) const;

	void appendPoints(MVectorArray &positions) const;

	// int getContacts( MDoubleArray &contacts, double maxWidth) const;


	// int getContacts(
	//   MDoubleArray &contacts,
	//   const std::vector< std::pair<int, float> > &widths) const;



	void appendRadii( MFloatArray &radii) const;
	void appendRadii( MDoubleArray &radii) const;

	void appendParams( MDoubleArray &params) const;

	void interpolate(int step, skChain &result) const;


	const float &maxRadius() const;
	const float &minRadius() const;


private:
	std::vector< skPoint > points;

	float m_maxRadius;
	float m_minRadius;

};
#endif
