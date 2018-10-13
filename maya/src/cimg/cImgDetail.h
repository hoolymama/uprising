
#ifndef _cImgDetail
#define _cImgDetail

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"
// #include "cImgData.h"

class cImgDetail : public cImgProcess
{
public:

	cImgDetail();

	virtual				~cImgDetail();

	// virtual MStatus		compute( const MPlug &plug, MDataBlock &data );
	virtual bool    isAbstractClass() const {return false;}

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;

	virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
	                        CImg<unsigned char> &result);

	static MObject aBlur;

private:

};

#endif
