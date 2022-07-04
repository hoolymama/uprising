
#ifndef _paint_
#define _paint_

#include <maya/MFloatVector.h>
#include <maya/MColor.h>
#include <maya/MString.h>


class Paint
{
public:


	Paint();
 
	Paint( const MColor &color,int pot, float travel, const MString &name);
	Paint( const MFloatVector &color, float opacity,int pot, float travel, const MString &name );

	~Paint();

	const MColor& color() const;
	int pot() const;
	float travel() const;
	const MString&  name() const;

	friend ostream &operator<<(ostream &os, const Paint &b);

private:
	MColor m_color;
	float m_travel;
	MString m_name;
	int m_pot;



	// compat
};

#endif
