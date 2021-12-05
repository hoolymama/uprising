
#ifndef _cImgMinMax
#define _cImgMinMax

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgMerge.h"

class cImgMinMax : public cImgMerge
{
public:

	cImgMinMax();

	virtual				~cImgMinMax();

	virtual bool    isAbstractClass() const {return false;}

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;

	virtual MStatus process(MDataBlock &data, const CImgList<unsigned char> &images,
	                        CImg<unsigned char> &result);


private:


	enum Operation { kMinimum, kMaximum};


	static MObject aOperation;


};

#endif
