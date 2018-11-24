
#ifndef _cImgNoise
#define _cImgNoise

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

class cImgNoise : public cImgProcess
{
public:

	cImgNoise();

	virtual				~cImgNoise();

	virtual bool    isAbstractClass() const {return false;}

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;

	virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
	                        CImg<unsigned char> &result);


	static MObject aScale;
	static MObject aInterpolation;

	static MObject aAmplitude;
	static MObject aType;



private:

};

#endif
