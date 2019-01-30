#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MAnimControl.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MGlobal.h>



#include <maya/MFnVectorArrayData.h>
#include <maya/MVectorArray.h>
#include <maya/MFloatMatrix.h>

#include <maya/MFloatArray.h>

#include <maya/MPlugArray.h>
#include "lodepng.h"

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgPngFile.h"
#include "jMayaIds.h"



// pngInfo: /Users/julian/projects/robot/sourceimages/ben_2_32.png
// Filesize: 110339 (107K)
// Width: 600
// Height: 800
// Num pixels: 480000
// Top left pixel color: r: 148 g: 88 b: 48 a: 255
// Compression method: 0
// Filter method: 0
// Interlace method: 0
// Color type: 3
// Bit depth: 8
// Bits per pixel: 8
// Channels per pixel: 1
// Is greyscale type: 0
// Can have alpha: 0
// Palette size: 32
// Has color key: 0
// Texts: 1
// Text: Software: Adobe ImageReady




MTypeId     cImgPngFile::id( k_cImgPngFile );

MObject 	cImgPngFile::aImageFilename;
MObject 	cImgPngFile::aResize;
MObject 	cImgPngFile::aResolution;

MObject 	cImgPngFile::aOutIndexImage;
MObject 	cImgPngFile::aOutColorImage;
MObject 	cImgPngFile::aOutPalette;


cImgPngFile::cImgPngFile() {}

cImgPngFile::~cImgPngFile() {}

void *cImgPngFile::creator()
{
	return new cImgPngFile();
}

MStatus cImgPngFile::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnUnitAttribute uAttr;
	MFnEnumAttribute eAttr;

	aImageFilename = tAttr.create("imageFilename", "im", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setUsedAsFilename(true);
	addAttribute( aImageFilename );


	aResize = nAttr.create( "resize", "rsz", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aResize);

	aResolution = nAttr.create("resolution", "rrg", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aResolution);


	aOutIndexImage = tAttr.create("outIndexImage", "oii", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutIndexImage);

	aOutColorImage = tAttr.create("outColorImage", "oci", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutColorImage);

	aOutPalette = tAttr.create("outPalette", "oplt", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutPalette);


	st = attributeAffects(aImageFilename, aOutIndexImage ); mser;
	st = attributeAffects(aResize, aOutIndexImage ); mser;
	st = attributeAffects(aResolution, aOutIndexImage ); mser;

	st = attributeAffects(aImageFilename, aOutColorImage ); mser;
	st = attributeAffects(aResize, aOutColorImage ); mser;
	st = attributeAffects(aResolution, aOutColorImage ); mser;

	st = attributeAffects(aImageFilename, aOutPalette ); mser;
	st = attributeAffects(aResize, aOutPalette ); mser;
	st = attributeAffects(aResolution, aOutPalette ); mser;




	return MS::kSuccess;
}


unsigned validatePng(lodepng::State state)
{
	const LodePNGInfo &info = state.info_png;
	const LodePNGColorMode &colorMode = info.color;

	if (colorMode.colortype != LCT_PALETTE) {return 1;}
	if (colorMode.bitdepth != 8) {return 1;}
	if (lodepng_get_bpp(&colorMode) != 8) {return 1;}
	if (lodepng_get_channels(&colorMode) != 1) {return 1;}
	return 0;
}


MStatus cImgPngFile::createColorImage(const MPlug &plug, MDataBlock &data,
                                      const std::vector<unsigned char> &buffer)
{
	MStatus st;
	std::vector<unsigned char> pngImage;


	unsigned w, h;
	lodepng::State state;
	unsigned error = lodepng::decode(pngImage, w, h, state, buffer);
	if (error) { 	return  MS::kUnknownParameter; }

	error = validatePng(state);
	if (error) {
		return  MS::kUnknownParameter;
	}

	CImg<unsigned char> image(w, h, 1, 3);
	int c = 0;
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			image(x, y, 0, 0) = pngImage[c]; c++;
			image(x, y, 0, 1) = pngImage[c]; c++;
			image(x, y, 0, 2) = pngImage[c]; c++;
			c++; // skip past 4th val (alpha)
		}
	}

	if (data.inputValue(aResize).asBool()) {
		const int2 &resolution = data.inputValue( aResolution ).asInt2();
		int xres = resolution[0];
		int yres = resolution[1];
		if (xres > 4096) { xres = 4096;}
		if (yres > 4096) {yres = 4096;}
		image.resize(xres, yres, -100, -100, 1 );
	}

	MDataHandle hOutput = data.outputValue(aOutColorImage);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid , &st ); mser;
	cImgData *newImageData = (cImgData *)fnOut.data(&st); mser;
	CImg<unsigned char> *outImage = newImageData->fImg;
	outImage->assign(image);

	hOutput.set(newImageData);
	data.setClean( aOutColorImage );
	return MS::kSuccess;

}


MStatus cImgPngFile::createIndexImage(const MPlug &plug, MDataBlock &data,
                                      const std::vector<unsigned char> &buffer)
{
	MStatus st;
	std::vector<unsigned char> pngImage;


	unsigned w, h;
	lodepng::State state;
	state.decoder.color_convert = false;

	unsigned error = lodepng::decode(pngImage, w, h, state, buffer);
	if (error) { 	return  MS::kUnknownParameter; }

	error = validatePng(state);
	if (error) {
		return  MS::kUnknownParameter;
	}

	CImg<unsigned char> image(w, h, 1, 1);
	int c = 0;
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			image(x, y, 0, 0) = pngImage[c]; c++;
		}
	}

	if (data.inputValue(aResize).asBool()) {
		const int2 &resolution = data.inputValue( aResolution ).asInt2();
		int xres = resolution[0];
		int yres = resolution[1];
		if (xres > 4096) { xres = 4096;}
		if (yres > 4096) {yres = 4096;}
		image.resize(xres, yres, -100, -100, 1 );
	}

	MDataHandle hOutput = data.outputValue(aOutIndexImage);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid , &st ); mser;
	cImgData *newImageData = (cImgData *)fnOut.data(&st); mser;
	CImg<unsigned char> *outImage = newImageData->fImg;
	outImage->assign(image);

	hOutput.set(newImageData);
	data.setClean( aOutIndexImage );
	return MS::kSuccess;

}


MStatus cImgPngFile::createPaletteImage(const MPlug &plug, MDataBlock &data,
                                        const std::vector<unsigned char> &buffer)
{
	MStatus st;
	std::vector<unsigned char> pngImage;


	unsigned w, h;
	lodepng::State state;
	state.decoder.color_convert = false;

	unsigned error = lodepng::decode(pngImage, w, h, state, buffer);
	if (error) { 	return  MS::kUnknownParameter; }

	const LodePNGInfo &info = state.info_png;
	const LodePNGColorMode &colorMode = info.color;
	unsigned palettesize = colorMode.palettesize;
	unsigned char *palette = colorMode.palette;

	CImg<unsigned char> image(palettesize, 1, 1, 3);
	size_t j = 0;
	for (int x = 0; x < palettesize; ++x)
	{
		image(x, 0, 0, 0) = int(palette[j]); j++;
		image(x, 0, 0, 1) = int(palette[j]); j++;
		image(x, 0, 0, 2) = int(palette[j]); j++;
		j++;
	}


	MDataHandle hOutput = data.outputValue(aOutPalette);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid , &st ); mser;
	cImgData *newImageData = (cImgData *)fnOut.data(&st); mser;
	CImg<unsigned char> *outImage = newImageData->fImg;
	outImage->assign(image);

	hOutput.set(newImageData);
	data.setClean( aOutPalette );
	return MS::kSuccess;

}

MStatus cImgPngFile::compute( const MPlug &plug, MDataBlock &data ) {

	if (!
	    ( (plug == aOutColorImage) ||
	      (plug == aOutIndexImage) ||
	      (plug == aOutPalette) )

	   ) { return ( MS::kUnknownParameter ); }

	MStatus st;

	unsigned error = false;


	MString imageFilename = data.inputValue(aImageFilename).asString();

	std::vector<unsigned char> buffer;

	error = lodepng::load_file(buffer, imageFilename.asChar());
	if (error) {
		return  MS::kUnknownParameter;
	}

	if (plug == aOutColorImage) {
		st = createColorImage(plug, data, buffer); mser;
	}
	if (plug == aOutIndexImage) {
		st = createIndexImage(plug, data, buffer); mser;
	}
	if (plug == aOutPalette) {
		st = createPaletteImage(plug, data, buffer); mser;
	}




	return MS::kSuccess;
}
