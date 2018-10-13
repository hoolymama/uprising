
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgProcess.h"
#include "jMayaIds.h"

MTypeId     cImgProcess::id( k_cImgProcess );

MObject 	cImgProcess::aInput;
MObject 	cImgProcess::aOutput;



cImgProcess::cImgProcess() {}

cImgProcess::~cImgProcess() {}

void *cImgProcess::creator()
{
	return new cImgProcess();
}

MStatus cImgProcess::initialize()
{
	MStatus st;

	MFnTypedAttribute tAttr;

	aInput = tAttr.create("input", "in", cImgData::id ) ;
	tAttr.setStorable(false);
	st = addAttribute( aInput ); mser;

	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);

	st = attributeAffects(aInput, aOutput ); mser;

	return MS::kSuccess;
}

MStatus cImgProcess::compute( const MPlug &plug, MDataBlock &data ) {

	if ( plug != aOutput) { return ( MS::kUnknownParameter ); }

	MStatus st = MS::kSuccess;

	MDataHandle hImageData = data.inputValue(aInput, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st);
	if (! st.error()) {

		cImgData *imageData = (cImgData *)fnImageData.data();
		CImg<unsigned char> *inImage = imageData->fImg;



		MDataHandle hOutput = data.outputValue(aOutput);
		MFnPluginData fnOut;
		MTypeId kdid(cImgData::id);
		MObject dOut = fnOut.create(kdid , &st ); mser;

		cImgData *newData = (cImgData *)fnOut.data(&st); mser;

		CImg<unsigned char> *outImage = newData->fImg;


		// ((*image) *= -1) += 255;

		process(data, *inImage, *outImage);

		// outImage->assign(*image);

		hOutput.set(newData);
		data.setClean( plug );
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;

}


MStatus cImgProcess::process(MDataBlock &data, const CImg<unsigned char> &image,
                             CImg<unsigned char> &result)
{
	return MS::kUnknownParameter;
}

