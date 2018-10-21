#include <map>
#include <vector>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFloatVector.h>
#include <maya/MFnPluginData.h>

#include <maya/MVectorArray.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MArrayDataBuilder.h>


#include "errorMacros.h"
#include "cImgData.h"
#include "cImgDistinct.h"
#include "jMayaIds.h"

#include <attrUtils.h>


struct colorPixel  {
	unsigned char index;
	std::vector<unsigned char> color;
	int x;
	int y;
};




MTypeId     cImgDistinct::id( k_cImgDistinct );
MObject 	cImgDistinct::aInput;
MObject 	cImgDistinct::aOutput;

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


	aPaletteSize = nAttr.create( "paletteSize", "psz", MFnNumericData::kInt);
	nAttr.setReadable (true);
	nAttr.setHidden (false);
	st = addAttribute(aPaletteSize);


	aPalette = tAttr.create("palette", "plt", MFnData::kVectorArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aPalette );


	st = attributeAffects(aInput, aPalette);
	st = attributeAffects(aInput, aPaletteSize);
	st = attributeAffects(aInput, aOutput );


	return MS::kSuccess;
}


MStatus cImgDistinct::compute( const MPlug &plug, MDataBlock &data ) {

	if (!
	    ( plug == aOutput) ||
	    ( plug == aPaletteSize) ||
	    ( plug == aPalette)
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




	// std::map< std::vector<unsigned char> , int > idxMap;
	// unsigned char index = 0;
	// for (auto pixel : pixelList)
	// {
	// 	auto el = idxMap.insert(std::pair<std::vector<unsigned char>, unsigned int>(pixel.color,
	// 	                        1));
	// 	if (! el.second) // failure, therefore its been seen
	// 	{
	// 		el->first->second++;
	// 	}
	// }



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


	MVectorArray palette;
	for (auto el : idxMap ) {

		int index = int(el.second);

		double red = double(el.first[0]) / 255.0;
		double green;
		double blue  ;
		if (spectrum == 3) {
			green = double(el.first[1]) / 255.0;
			blue = double(el.first[2]) / 255.0;
		}
		else {
			green = blue = red;
		}
		palette.append(MVector(red, green, blue));
	}
	st = outputData(cImgDistinct::aPalette, data, palette ); mser;

	MDataHandle hSize = data.outputValue(aPaletteSize);
	hSize.set(int(idxMap.size()));
	hSize.setClean();




	return MS::kSuccess;

}





// MStatus cImgDistinct::process(MDataBlock &data, const CImg<unsigned char> &image,
//                               CImg<unsigned char> &result)

// {
// 	MStatus st;

// 	cerr << "cImgDistinct::process:" << endl;;
// 	std::vector<colorPixel> pixelList;

// 	int spectrum = image.spectrum() ;
// 	int w = image.width() ;
// 	int h = image.height() ;

// 	for (int x = 0; x < w; ++x)
// 	{
// 		for (int y = 0; y < h; ++y)
// 		{
// 			std::vector<unsigned char> color;
// 			for (int c = 0; c < spectrum; ++c)
// 			{
// 				unsigned char val = image(x, y, c);
// 				color.push_back(val);
// 			}
// 			colorPixel pixel = { .color = color, .index = 255, .x = x, .y = y};
// 			pixelList.push_back(pixel);
// 		}
// 	}





// 	std::map< std::vector<unsigned char>, unsigned char > idxMap;
// 	unsigned char index = 0;
// 	for (auto pixel : pixelList)
// 	{
// 		if (index == 255) {
// 			cerr << "Index went to 255 - breaking out of the rest" << endl;
// 			break;
// 		}
// 		auto el = idxMap.insert(std::pair<std::vector<unsigned char>, unsigned char>(pixel.color,
// 		                        index));
// 		if (el.second) // success - therefore its not been seen before
// 		{
// 			index++;
// 		}
// 	}


// 	result.assign(w, h);
// 	// assign all white
// 	result = 255;

// 	for (auto pixel : pixelList)
// 	{
// 		auto it = idxMap.find(pixel.color);
// 		if (it != idxMap.end()) {
// 			result(pixel.x, pixel.y) = it->second;
// 		}
// 	}


// 	MVectorArray palette;
// 	for (auto el : idxMap ) {

// 		int index = int(el.second);

// 		double red = double(el.first[0]) / 255.0;
// 		double green;
// 		double blue  ;
// 		if (spectrum == 3) {
// 			green = double(el.first[1]) / 255.0;
// 			blue = double(el.first[2]) / 255.0;
// 		}
// 		else {
// 			green = blue = red;
// 		}
// 		palette.append(MVector(red, green, blue));
// 	}
// 	st = outputData(cImgDistinct::aPalette, data, palette ); mser;

// 	MDataHandle hSize = data.outputValue(aPaletteSize);
// 	hSize.set(int(idxMap.size()));
// 	hSize.setClean();




// 	return MS::kSuccess;

// }
