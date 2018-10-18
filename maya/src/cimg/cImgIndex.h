
#ifndef _cImgIndex
#define _cImgIndex

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

class cImgIndex : public cImgProcess
{
public:

	cImgIndex();

	virtual				~cImgIndex();

	virtual bool    isAbstractClass() const {return false;}

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;

	virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
	                        CImg<unsigned char> &result);

	static MObject aDithering;

	static MObject aMapIndices;



private:

};

#endif
