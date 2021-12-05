
#ifndef _cImgReverse
#define _cImgReverse

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

class cImgReverse : public cImgProcess
{
public:
	cImgReverse();

	virtual ~cImgReverse();

	virtual bool isAbstractClass() const { return false; }

	static void *creator();

	static MStatus initialize();

	static MTypeId id;

	virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
							CImg<unsigned char> &result);

private:
};

#endif
