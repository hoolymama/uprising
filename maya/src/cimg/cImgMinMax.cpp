
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgMinMax.h"
#include "jMayaIds.h"


#include <maya/MFnEnumAttribute.h>

MTypeId     cImgMinMax::id( k_cImgMinMax );

MObject cImgMinMax::aOperation;

cImgMinMax::cImgMinMax() {}

cImgMinMax::~cImgMinMax() {}

void *cImgMinMax::creator()
{
	return new cImgMinMax();
}

MStatus cImgMinMax::initialize()
{
	MStatus st;
	MFnEnumAttribute eAttr;

	inheritAttributesFrom("cImgMerge");

	aOperation = eAttr.create("operation", "op", cImgMinMax::kMinimum);
	eAttr.addField( "minimum",   cImgMinMax::kMinimum);
	eAttr.addField( "maximum",   cImgMinMax::kMaximum);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	st = addAttribute( aOperation );




	attributeAffects(aOperation, aOutput);

	return MS::kSuccess;
}



MStatus cImgMinMax::process(MDataBlock &data,
                            const CImgList<unsigned char> &images,
                            CImg<unsigned char> &result)

{


	if (!images.size())
	{
		return MS::kUnknownParameter;
	}


	cImgMinMax::Operation op = cImgMinMax::Operation(data.inputValue(aOperation).asShort());

	int w = images.front().width();
	int h = images.front().height();
	int s = images.front().spectrum();


	// float recip = 1.0f / 255;
	unsigned char startVal = 0;
	CImgList<unsigned char>::const_iterator citer;
	if (op == cImgMinMax::kMinimum)
	{
		result.assign(w, h, 1, s, 255);
		for (citer = images.begin(); citer != images.end(); citer++)
		{
			result.min( citer->get_resize(w, h));
		}
	}
	else {
		result.assign(w, h, 1, s, 0);
		for (citer = images.begin(); citer != images.end(); citer++)
		{
			result.max( citer->get_resize(w, h));
		}
	}

	return MS::kSuccess;
}


