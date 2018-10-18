#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgIndex.h"
#include "jMayaIds.h"

MTypeId     cImgIndex::id( k_cImgIndex );


MObject 	cImgIndex::aDithering;

MObject 	cImgIndex::aMapIndices;


cImgIndex::cImgIndex() {}

cImgIndex::~cImgIndex() {}

void *cImgIndex::creator()
{
	return new cImgIndex();
}

MStatus cImgIndex::initialize()
{
	MStatus st;



	inheritAttributesFrom("cImgProcess");

	MFnNumericAttribute nAttr;

	aDithering = nAttr.create( "dithering", "dth", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aDithering);


	aMapIndices = nAttr.create( "mapIndices", "mpi", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aMapIndices);





	st = attributeAffects(aDithering, aOutput );
	st = attributeAffects(aMapIndices, aOutput );

	return MS::kSuccess;
}


MStatus cImgIndex::process(MDataBlock &data, const CImg<unsigned char> &image,
                           CImg<unsigned char> &result)

{


	bool dithering = data.inputValue(aDithering).asBool();
	bool mapIndices = !data.inputValue(aMapIndices).asBool();


	int spectrum = image.spectrum();
	if (image.spectrum() == 1) {
		CImg<unsigned char> colormap(8, 1, 1, 1, 0, 32, 64, 96, 128, 160, 192, 224);
		result = image.get_index(colormap, dithering, mapIndices);
		return MS::kSuccess;
	}

	CImg<unsigned char> colormap(8, 1, 1, 3,
	                             0, 32, 64, 96, 128, 160, 192, 224,
	                             0,  0,  0,  0,   0,   0,   0,   0,
	                             0, 32, 64, 96, 128, 160, 192, 224);
	result = image.get_index(colormap, dithering, mapIndices);


	return MS::kSuccess;

}
