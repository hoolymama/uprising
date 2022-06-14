
#ifndef paletteOrder_H
#define paletteOrder_H

#include <math.h> 
#include <maya/MPxNode.h>
#include <maya/MColor.h>

class paletteOrder: public MPxNode {

public:

	enum Method
	{
		kNone,
		kHSPAscending,
		kHSPDescending
	};

	virtual	void		postConstructor();
	paletteOrder ();
	virtual ~paletteOrder ();

	virtual MStatus compute (const MPlug &plug, MDataBlock &data);
	static void *creator ();
	static MStatus initialize ();
	static MTypeId id;


    static float calculateHSP(const MColor &color);

private:


	static MObject aInput;
	static MObject aMethod;
	static MObject aOutput;

};


inline float paletteOrder::calculateHSP(const MColor &color)
{
	return  sqrt( 0.299*color.r*color.r + .587*color.g*color.g + .114*color.b*color.b );
}


#endif