
#ifndef _cImgQuantize
#define _cImgQuantize

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

class cImgQuantize : public cImgProcess
{
public:

	cImgQuantize();

	virtual				~cImgQuantize();

	virtual bool    isAbstractClass() const {return false;}

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;

	virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
	                        CImg<unsigned char> &result);


	static MObject aLevels;

	static MObject aKeepRange;



private:

};

#endif
