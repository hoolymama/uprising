
#ifndef _cImgBlur
#define _cImgBlur

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

class cImgBlur : public cImgProcess
{
public:

	cImgBlur();

	virtual				~cImgBlur();

	virtual bool    isAbstractClass() const {return false;}

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;

	virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
	                        CImg<unsigned char> &result);


	static MObject aAmount;
	static MObject aBoundary;
	static MObject aGaussian;



private:


	enum Boundary { kBoundaryDirichlet, kBoundaryNeumann};

};

#endif
