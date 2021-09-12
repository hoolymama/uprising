
#ifndef _paint_
#define _paint_

#include <maya/MFloatVector.h>
#include <maya/MColor.h>

class Paint
{
public:


	Paint();

	Paint( const MColor &color, float travel);
	Paint( const MFloatVector &color, float opacity, float travel );

	~Paint();

	const MColor& color() const;
	int pot() const;
	float travel() const;

	friend ostream &operator<<(ostream &os, const Paint &b);

private:
	MColor m_color;
	float m_travel;


	// compat
};

#endif
