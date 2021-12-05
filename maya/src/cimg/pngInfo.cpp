
// #include "lodepng.h"
// #include <iostream>


#include <maya/MString.h>
#include <maya/MGlobal.h>

#include <maya/MArgDatabase.h>
#include <maya/MSyntax.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MRenderUtil.h>

#include <errorMacros.h>


#define cimg_display 0
#include "CImg.h"


#include "pngInfo.h"


using namespace cimg_library;


//	static
void *pngInfo::creator()
{
	return new pngInfo();
}

MSyntax pngInfo::newSyntax()
{
	MSyntax syn;
	syn.addFlag(kPaletteFlag, kPaletteFlagL, MSyntax::kBoolean  );
	syn.addFlag(kPaletteSizeFlag, kPaletteSizeFlagL, MSyntax::kBoolean  );

	syn.setObjectType(MSyntax::kStringObjects, 1, 1);


	return syn;

}


MStatus pngInfo::doIt( const MArgList &args )
{
	MStatus st;

	MArgDatabase  argData(syntax(), args);

	MStringArray argStrings;
	argData.getObjects(argStrings);
	MString filename = argStrings[0];

	if (argData.isFlagSet(kPaletteFlag)) {
		return handlePaletteFlag(filename);
	}

	if (argData.isFlagSet(kPaletteSizeFlag)) {
		return handlePaletteSizeFlag(filename);
	}

	return MS::kSuccess;
}



MStatus pngInfo::readImage(const MString &filename, lodepng::State &state, unsigned &w,
                           unsigned &h, std::vector<unsigned char> &image)
{

	std::vector<unsigned char> buffer;
	unsigned error;
	error = lodepng::load_file(buffer, filename.asChar() );
	if (error) {
		MGlobal::displayError("Can't read PNG file: " + filename);
		return MS::kFailure;
	}
	error = lodepng::decode(image, w, h, state, buffer);
	if (error) {
		MGlobal::displayError("Can't decode PNG file: " + filename);
		return MS::kFailure;
	}
	return MS::kSuccess;
}


MStatus pngInfo::handlePaletteSizeFlag(const MString &filename) {

	MStatus st;
	MIntArray result;
	lodepng::State state;
	unsigned w, h;
	std::vector<unsigned char> image;
	st = readImage(filename, state, w, h, image);  msert;

	LodePNGColorMode &color = state.info_png.color;
	if (color.colortype != LCT_PALETTE) {
		MGlobal::displayError("Image is not an indexed PNG with separate palette: " + filename);
		return MS::kFailure;
	}
	setResult(int(color.palettesize));
	return MS::kSuccess;
}




MStatus pngInfo::handlePaletteFlag(const MString &filename) {

	MStatus st;
	MIntArray result;
	lodepng::State state;
	unsigned w, h;
	std::vector<unsigned char> image;
	st = readImage(filename, state, w, h, image); msert;

	LodePNGColorMode &color = state.info_png.color;
	if (color.colortype != LCT_PALETTE) {
		MGlobal::displayError("Image is not an indexed PNG with separate palette: " + filename);
		return MS::kFailure;
	}

	unsigned char *palette = color.palette;
	size_t j = 0;
	for (int i = 0; i < color.palettesize; ++i)
	{
		result.append(int(palette[j])); j++;
		result.append(int(palette[j])); j++;
		result.append(int(palette[j])); j++;
		result.append(int(palette[j])); j++;
	}
	setResult(result);
	return MS::kSuccess;
}




