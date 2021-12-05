
#ifndef _cImgProcess
#define _cImgProcess

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>


#include "cImgData.h"

class cImgProcess : public MPxNode
{
public:

	cImgProcess();

	virtual				~cImgProcess();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );
	virtual bool    isAbstractClass() const {return true;}


	virtual MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
	                        CImg<unsigned char> &result);

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;


protected:

	static MObject aInvert;
	static MObject aInput;
	static MObject aOutput;

};

#endif
