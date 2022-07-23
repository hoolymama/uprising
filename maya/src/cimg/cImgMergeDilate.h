
#ifndef _cImgMergeDilate
#define _cImgMergeDilate

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgMerge.h"


class cImgMergeDilate : public cImgMerge
{
public:
	cImgMergeDilate();

	virtual ~cImgMergeDilate();

	virtual bool isAbstractClass() const { return false; }

	static void *creator();

	static MStatus initialize();

	static MTypeId id;

	virtual MStatus process(MDataBlock &data, const CImgList<unsigned char> &images,
	                        CImg<unsigned char> &result);

private:

	static MObject aDilateAmount;
	static MObject aBlur;
	static MObject aThreshold;
	static MObject aMedian;

	static MObject aOperation;

	enum Operation { kMinimum, kMaximum};


};

#endif
