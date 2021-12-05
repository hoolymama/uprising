
#ifndef _cImgGate
#define _cImgGate

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

class cImgGate : public cImgProcess
{
public:
	cImgGate();

	virtual ~cImgGate();

	virtual bool isAbstractClass() const { return false; }

	static void *creator();

	static MStatus initialize();

	static MTypeId id;

	virtual MStatus process(
		MDataBlock &data, 
		const CImg<unsigned char> &image,
		CImg<unsigned char> &result);

	static MObject aDilate;
	static MObject aBlur;
	static MObject aMinGate;
	static MObject aMaxGate;

private:
};

#endif
