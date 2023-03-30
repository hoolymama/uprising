
#ifndef _cImgQuantize
#define _cImgQuantize

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>
#include <maya/MColor.h>
#include <math.h>

#include "cImgData.h"

class cImgQuantize : public MPxNode
{
public:
	cImgQuantize();

	virtual ~cImgQuantize();

	virtual MStatus compute(const MPlug &plug, MDataBlock &data);

	static void *creator();

	static MStatus initialize();

	static MObject aInput;
	static MObject aOutput;
	static MObject aLevels; 	





	static MObject aGreyscale;

	static MObject aQuantize;
	static MObject aDither;

	static MTypeId id;

	MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
					CImg<unsigned char> &result);

private:
	MStatus quantizeUniform(MDataBlock &data, const CImg<unsigned char> &image,
							CImg<unsigned char> &result);

	MStatus quantizeKMeans(MDataBlock &data, const CImg<unsigned char> &image,
						   CImg<unsigned char> &result);

	enum Dither
	{
		kNone,
		kFloydSteinberg
	};
	enum Quantize
	{
		kUniform,
		kMedianCut,
		kKMeans
	};
};

#endif