
#ifndef _cImgMerge
#define _cImgMerge

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>


#include "cImgData.h"

class cImgMerge : public MPxNode
{
public:

	cImgMerge();

	virtual				~cImgMerge();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );
	virtual bool    isAbstractClass() const {return true;}


	virtual MStatus process(MDataBlock &data, const CImgList<unsigned char> &images,
	                        CImg<unsigned char> &result);

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;


protected:


	static MObject aInput;
	static MObject aInvert;
	static MObject aOutput;

};

#endif
