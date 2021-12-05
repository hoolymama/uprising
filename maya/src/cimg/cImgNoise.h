
#ifndef _cImgNoise
#define _cImgNoise

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgData.h"

class cImgNoise : public MPxNode
{
public:
	cImgNoise();

	virtual ~cImgNoise();

	virtual MStatus compute(const MPlug &plug, MDataBlock &data);

	static void *creator();

	static MStatus initialize();

	static MTypeId id;

private:
	static MObject aResolution;

	static MObject aFrequency;
	static MObject aAmplitude;
	static MObject aLacunarity;
	static MObject aPersistence;
	static MObject aOctaves;

	static MObject aBlur;
	static MObject aOutput;
};

#endif
