
#ifndef _cImgDilate
#define _cImgDilate

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

class cImgDilate : public cImgProcess
{
public:
	cImgDilate();

	virtual ~cImgDilate();

	virtual bool isAbstractClass() const { return false; }

	static void *creator();

	static MStatus initialize();

	static MTypeId id;

	virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
							CImg<unsigned char> &result);

	static MObject aDilateAmount;
	static MObject aBlur;
	static MObject aThreshold;
	static MObject aMedian;
	static MObject aBitwise;


private:

	enum Bitwise { kNone, kOR, kAND, kXOR};


};

#endif
