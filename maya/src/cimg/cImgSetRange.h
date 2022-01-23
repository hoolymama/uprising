
#ifndef _cImgSetRange
#define _cImgSetRange

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

class cImgSetRange : public cImgProcess
{
public:
	cImgSetRange();

	virtual ~cImgSetRange();

	virtual bool isAbstractClass() const { return false; }

	static void *creator();

	static MStatus initialize();

	static MTypeId id;

	virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
							CImg<unsigned char> &result);

private:

    static MObject aMinColor;
    static MObject aMaxColor;
};

#endif
