
#ifndef _cImgGradient
#define _cImgGradient

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>


#include "cImgData.h"

class cImgGradient : public MPxNode
{
public:

	cImgGradient();

	virtual				~cImgGradient();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	virtual MStatus process(
		MDataBlock &data, 
		const CImg<unsigned char> &image,
		CImg<float> &result);

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;


protected:


	static MObject aInput;
	static MObject aBlur;
	static MObject aOutput;

};

#endif
