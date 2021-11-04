

#ifndef _particleTrail_H
#define _particleTrail_H

#include <maya/MFloatPointArray.h>
#include <maya/MColorArray.h>

class particleTrail {

public:
	particleTrail()
		: m_positions(), 
        m_colors() 
    {}

    ~particleTrail(){};

	void append(const MFloatPoint &p, const MColor &c)
	{
		m_positions.append(p);
		m_colors.append(c);
	}

	const MFloatPointArray & positions() const
	{
		return m_positions;
	}

	const MColorArray & colors() const 
	{
		return m_colors;
	}
	

private:
	MFloatPointArray m_positions;
	MColorArray m_colors;
};
#endif
