#include <map>
#include <vector>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFloatVector.h>
#include <maya/MFnPluginData.h>

#include <maya/MArrayDataHandle.h>
#include <maya/MArrayDataBuilder.h>


#include "errorMacros.h"
#include "cImgData.h"
#include "cImgDistinct.h"
#include "jMayaIds.h"


struct colorPixel  {
	unsigned char index;
	std::vector<unsigned char> color;
	int x;
	int y;
};




MTypeId     cImgDistinct::id( k_cImgDistinct );
MObject 	cImgDistinct::aInput;
MObject 	cImgDistinct::aOutput;
MObject 	cImgDistinct::aPaletteR;
MObject 	cImgDistinct::aPaletteG;
MObject 	cImgDistinct::aPaletteB;

MObject 	cImgDistinct::aPalette;
MObject 	cImgDistinct::aPaletteSize;

cImgDistinct::cImgDistinct() {}

cImgDistinct::~cImgDistinct() {}

void *cImgDistinct::creator()
{
	return new cImgDistinct();
}

MStatus cImgDistinct::initialize()
{
	MStatus st;



	// inheritAttributesFrom("cImgProcess");

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	aInput = tAttr.create("input", "in", cImgData::id ) ;
	tAttr.setReadable(false);
	st = addAttribute( aInput ); mser;

	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);


	// aPalette = nAttr.create( "palette", "plt", MFnNumericData::k3Float);

	// nAttr.setReadable( true );
	// nAttr.setStorable( false );
	// nAttr.setArray( true );
	// nAttr.setUsesArrayDataBuilder( true );
	// st = addAttribute(aPalette);


	aPaletteSize = nAttr.create( "paletteSize", "psz", MFnNumericData::kInt);
	nAttr.setStorable (false);
	nAttr.setWritable (false);
	nAttr.setReadable (true);
	nAttr.setHidden (false);
	st = addAttribute(aPaletteSize);

	///////////////////////////////////////////////////////////////////////
	aPaletteR = nAttr.create("paletteR", "pltr", MFnNumericData::kFloat);
	aPaletteG = nAttr.create("paletteG", "pltg", MFnNumericData::kFloat);
	aPaletteB = nAttr.create("paletteB", "pltb", MFnNumericData::kFloat);
	aPalette = nAttr.create("palette", "plt", aPaletteR, aPaletteG, aPaletteB);
	nAttr.setReadable( true );
	nAttr.setStorable( false );
	nAttr.setArray( true );
	nAttr.setUsesArrayDataBuilder( true );
	st = addAttribute( aPalette ); mser;






	// st = attributeAffects(aInput, aPaletteR);
	// st = attributeAffects(aInput, aPaletteG);
	// st = attributeAffects(aInput, aPaletteB);
	st = attributeAffects(aInput, aPalette);
	st = attributeAffects(aInput, aPaletteSize);
	st = attributeAffects(aInput, aOutput );


	return MS::kSuccess;
}


MStatus cImgDistinct::compute( const MPlug &plug, MDataBlock &data ) {

	if (!
	    ( plug == aOutput) ||
	    ( plug == aPaletteSize) ||
	    ( plug == aPalette) ||
	    ( plug.parent() == aPalette)
	   )
	{
		return ( MS::kUnknownParameter );
	}

	cerr << "plug.name():" << plug.name() << endl;;
	MStatus st;

	MDataHandle hImageData = data.inputValue(aInput, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st);
	if (st.error()) {
		return MS::kUnknownParameter;
	}
	cImgData *imageData = (cImgData *)fnImageData.data();
	CImg<unsigned char> *inImage = imageData->fImg;

	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid , &st ); mser;
	cImgData *newData = (cImgData *)fnOut.data(&st); mser;
	CImg<unsigned char> *outImage = newData->fImg;
	if (data.inputValue( state).asShort()) {
		outImage->assign(*inImage);
	}
	else {
		process(data, *inImage, *outImage);
	}
	hOutput.set(newData);
	data.setClean( plug );
	return MS::kSuccess;

}

MStatus cImgDistinct::process(MDataBlock &data, const CImg<unsigned char> &image,
                              CImg<unsigned char> &result)

{
	MStatus st;

	cerr << "cImgDistinct::process:" << endl;;
	std::vector<colorPixel> pixelList;

	int spectrum = image.spectrum() ;
	int w = image.width() ;
	int h = image.height() ;

	for (int x = 0; x < w; ++x)
	{
		for (int y = 0; y < h; ++y)
		{
			std::vector<unsigned char> color;
			for (int c = 0; c < spectrum; ++c)
			{
				unsigned char val = image(x, y, c);
				color.push_back(val);
			}
			colorPixel pixel = { .color = color, .index = 255, .x = x, .y = y};
			pixelList.push_back(pixel);
		}
	}

	std::map< std::vector<unsigned char>, unsigned char > idxMap;
	unsigned char index = 0;
	for (auto pixel : pixelList)
	{
		if (index == 255) {
			cerr << "Index went to 255 - breaking out of the rest" << endl;
			break;
		}
		auto el = idxMap.insert(std::pair<std::vector<unsigned char>, unsigned char>(pixel.color,
		                        index));
		if (el.second) // success - therefore its not been seen before
		{
			index++;
		}
	}


	result.assign(w, h);
	// assign all white
	result = 255;

	for (auto pixel : pixelList)
	{
		auto it = idxMap.find(pixel.color);
		if (it != idxMap.end()) {
			result(pixel.x, pixel.y) = it->second;
		}
	}


	MArrayDataHandle  hPalette = data.outputArrayValue( aPalette, &st); mser;
	MArrayDataBuilder bPalette = hPalette.builder(&st); mser;

	for (auto el : idxMap ) {
		// cerr << "int(el.second)" << int(el.second) << endl;
		int index = int(el.second);
		MDataHandle hColor = bPalette.addElement(index, &st); mser;
		float3 &out = hColor.asFloat3();
		// MFloatVector &outColor = hColor.asFloatVector();
		float red = float(el.first[0]) / 255.0;
		float green;
		float blue  ;
		if (spectrum == 3) {
			green = float(el.first[1]) / 255.0;
			blue = float(el.first[2]) / 255.0;
		}
		else {
			green = blue =  red;
		}
		// cerr << "Col-" << int(el.second) << " --: " << red << " " << green << " " << blue << endl;
		// hColor.set(MFloatVector(red, green, blue));
		out[0] = red;
		out[1] = green;
		out[2] = blue;

	}
	hPalette.setAllClean();


	MDataHandle hSize = data.outputValue(aPaletteSize);
	hSize.set(int(idxMap.size()));
	hSize.setClean();




	return MS::kSuccess;

}
