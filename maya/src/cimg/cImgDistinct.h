
#ifndef _cImgDistinct
#define _cImgDistinct

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

class cImgDistinct : public MPxNode
{
public:

	cImgDistinct();

	virtual				~cImgDistinct();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );


	MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
	                CImg<unsigned char> &result);

	static  void		*creator();

	static MStatus initialize();


	static MTypeId	id;

	static MObject aInput;
	static MObject aOutput;

	static MObject aPaletteSize;
	static MObject aPalette;


	// static MObject aPaletteR;
	// static MObject aPaletteG;
	// static MObject aPaletteB;




private:

};

#endif
