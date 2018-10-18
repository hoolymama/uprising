#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgQuantize.h"
#include "jMayaIds.h"

MTypeId     cImgQuantize::id( k_cImgQuantize );


MObject 	cImgQuantize::aLevels;

MObject 	cImgQuantize::aKeepRange;


cImgQuantize::cImgQuantize() {}

cImgQuantize::~cImgQuantize() {}

void *cImgQuantize::creator()
{
	return new cImgQuantize();
}

MStatus cImgQuantize::initialize()
{
	MStatus st;



	inheritAttributesFrom("cImgProcess");

	MFnNumericAttribute nAttr;


	aLevels = nAttr.create( "levels", "lv", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(8);
	st = addAttribute(aLevels);


	aKeepRange = nAttr.create( "keepRange", "kpr", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aKeepRange);


	st = attributeAffects(aLevels, aOutput );
	st = attributeAffects(aKeepRange, aOutput );

	return MS::kSuccess;
}


MStatus cImgQuantize::process(MDataBlock &data, const CImg<unsigned char> &image,
                              CImg<unsigned char> &result)

{
	int levels = data.inputValue(aLevels).asInt();
	bool keepRange = data.inputValue(aKeepRange).asBool();

	result = image.get_quantize(levels, keepRange);

	return MS::kSuccess;

}
