
#ifndef _cImgMult
#define _cImgMult

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgMerge.h"

class cImgMult : public cImgMerge
{
public:

	cImgMult();

	virtual				~cImgMult();

	virtual bool    isAbstractClass() const {return false;}

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;

	virtual MStatus process(MDataBlock &data, const CImgList<unsigned char> &images,
	                        CImg<unsigned char> &result);


private:

};

#endif
