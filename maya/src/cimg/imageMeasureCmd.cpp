
// #include "lodepng.h"
// #include <iostream>


#include <maya/MString.h>
#include <maya/MGlobal.h>
#include <maya/MPlug.h>
#include <maya/MFnPluginData.h>
#include <maya/MArgDatabase.h>
#include <maya/MSyntax.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MItSelectionList.h>

#include <maya/MArgList.h>


#include <errorMacros.h>


#define cimg_display 0
#include "CImg.h"


#include "imageMeasureCmd.h"


using namespace cimg_library;


//	static
void *imageMeasureCmd::creator()
{
	return new imageMeasureCmd();
}

MSyntax imageMeasureCmd::newSyntax()
{
	MSyntax syn;

	syn.addFlag(kAxisFlag, kAxisFlagL, MSyntax::kString);

	syn.addFlag(kSliceFlag, kSliceFlagL, MSyntax::kLong);
	syn.makeFlagMultiUse	(	kSliceFlag)	;



	syn.addFlag(kThresholdFlag, kThresholdFlagL, MSyntax::kLong);
	syn.addFlag(kInvertFlag, kInvertFlagL );

	// syn.addFlag(kPercentageFlag, kPercentageFlagL );

	syn.addFlag(kExtentsFlag, kExtentsFlagL, MSyntax::kLong,  MSyntax::kLong);

	syn.addFlag(kFilenameFlag, kFilenameFlagL, MSyntax::kString);


	syn.addFlag(kScaleFlag, kScaleFlagL, MSyntax::kDouble);


	syn.setObjectType(MSyntax::kSelectionList, 1, 1);
	syn.useSelectionAsDefault(false);


	return syn;

}


MStatus imageMeasureCmd::doIt( const MArgList &args )
{
	MStatus st;
	MSelectionList list;
	MArgDatabase  argData(syntax(), args);
	argData.getObjects(list);
	MString msg("Must provide a valid cImg plug");

	MItSelectionList cImgNodeIter( list, MFn::kPluginDependNode);
	if (cImgNodeIter.isDone()) {
		displayError(msg);
		return MS::kUnknownParameter;
	}

	MObject  cImgObject;
	cImgNodeIter.getDependNode( cImgObject );

	MPlug cImgPlug;
	st = 	cImgNodeIter.getPlug (cImgPlug);


	MObject dCImgData;
	st = cImgPlug.getValue(dCImgData);
	MFnPluginData fnCImgData(dCImgData);

	if (fnCImgData.typeId() != cImgData::id)
	{
		displayError("Plug is not of type: cImgData");
		return MS::kUnknownParameter;
	}

	cImgData *imageData  =  (cImgData *)fnCImgData.data();

	if (! imageData)  {
		displayError("No valid cImgData");
		return MS::kUnknownParameter;
	}
	CImg<unsigned char> *image = imageData->fImg;


	if (! image)  {
		displayError("No valid image");
		return MS::kUnknownParameter;
	}

	int w = image->width();
	int h = image->height();


	MString axis = "y";

	if (argData.isFlagSet(kAxisFlag)) {
		st = argData.getFlagArgument (kAxisFlag, 0, axis);
		if (! (axis == "x" || axis == "y") )
		{
			displayError("Invalid axis. x or y please");
			return MS::kUnknownParameter;
		}
	}


	int slice = 0;
	MIntArray slices;


	unsigned int nSlices = argData.numberOfFlagUses(kSliceFlag);


	// cerr << "NUM SLICES" << nSlices << endl;
	if (nSlices == 0) {
		displayError("No slices");
		return MS::kUnknownParameter;
	}

	for (int i = 0; i < nSlices; ++i)
	{
		/* code */

		// st = argData.getFlagArgument (kSliceFlag, i, slice);

		MArgList argList;
		argData.getFlagArgumentList( kSliceFlag, i, argList );
		slice = argList.asInt(0);

		if (slice < 0) {
			displayError("Invalid slice");
			return MS::kUnknownParameter;
		}

		if (axis == "x") {
			if (! (slice < h)) {
				displayError("Invalid slice");
				return MS::kUnknownParameter;
			}
		}
		else {
			if (! (slice < w)) {
				displayError("Invalid slice");
				return MS::kUnknownParameter;
			}
		}
		slices.append(slice);

	}

	int thresh = 128;
	if (argData.isFlagSet(kThresholdFlag)) {
		st = argData.getFlagArgument (kThresholdFlag, 0, thresh);
		if (thresh < 1 || thresh > 254)
		{
			displayError("Invalid threshold");
			return MS::kUnknownParameter;
		}
	}

	bool invert = false;
	if (argData.isFlagSet(kInvertFlag)) {
		invert = true;
	}




	MDoubleArray result;
	int index = -1;
	bool counting = false;

	int end = h;
	if (axis == "x")
	{
		end = w;
	}
	int start  = 0;
	if (argData.isFlagSet(kExtentsFlagL)) {
		int tmpStart, tmpEnd;
		st = argData.getFlagArgument (kThresholdFlag, 0, tmpStart);
		if (tmpStart < 0) {
			displayError("Invalid start extent");
			return MS::kUnknownParameter;
		}
		st = argData.getFlagArgument (kThresholdFlag, 1, tmpEnd);
		if (tmpEnd > end) {
			displayError("Invalid end extent");
			return MS::kUnknownParameter;
		}
		if (tmpStart >= tmpEnd) {
			displayError("Invalid start and end extent");
			return MS::kUnknownParameter;
		}
		start = tmpStart;
		end = tmpEnd;
	}


	double scale = 1.00;
	if (argData.isFlagSet(kScaleFlag)) {
		argData.getFlagArgument (kScaleFlag, 0, scale);
	}

	if (axis == "x") {
		scale = scale / double(w);
	}
	else {
		scale = scale / double(h);
	}


	bool doFile = false;
	MString filename = "";
	if (argData.isFlagSet(kFilenameFlag)) {
		st = argData.getFlagArgument (kFilenameFlag, 0, filename);
		doFile = true;
	}





	CImg<unsigned char> outImage;
	if (doFile)
	{
		outImage.assign(*image);
	}


	const float red[] = { 255, 150, 0 }, black[] = { 0, 0, 0 };



	for (int sliceIndex = 0; sliceIndex < nSlices; ++sliceIndex)
	{
		int slice = slices[sliceIndex];
		// cerr << "slice" << slice << endl;

		bool inPatch = false;

		for (int pix = start; pix < end; ++pix)
		{
			if (axis == "x") {
				inPatch = (*image)(pix, slice) < thresh ;
			}
			else {
				inPatch =  (*image)(slice, pix) < thresh ;
			}
			if (invert) {
				inPatch = ! inPatch;
			}
			if (inPatch)
			{
				if (!counting) {
					result.append(1.0);
					index++;
					counting = true;
				}
				else
				{
					result[index] += 1.0;
				}
			}
			else // not in patch
			{
				if (counting == true) {

					result[index] *= scale;

					MString val;
					val.set(result[index], 3);

					if (doFile)
					{
						if (axis == "x") {
							outImage.draw_text(pix, (slice - 13), val.asChar(), red, black);
						}
						else
						{
							outImage.draw_text(slice, (pix - 13), val.asChar(), red, black);
						}
					}
					counting = false;
				}
			}
		}
	}

	if (doFile)
	{
		outImage.save(filename.asChar());
	}



	setResult(result);
	return MS::kSuccess;
}



