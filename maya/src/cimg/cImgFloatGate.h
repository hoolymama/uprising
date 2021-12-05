
#ifndef _cImgGateFloat
#define _cImgGateFloat

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

// #include "cImgProcess.h"

class cImgFloatGate : public MPxNode
{
public:
	cImgFloatGate();

	virtual ~cImgFloatGate();
 
	static void *creator();

	static MStatus initialize();

	static MTypeId id;

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	virtual MStatus process(
		MDataBlock &data, 
		const CImg<float> &image,
		CImg<unsigned char> &result);
 
	static MObject aMinGate;
	static MObject aMaxGate;
	static MObject aInput;
	static MObject aOutput;



private:
};

#endif
